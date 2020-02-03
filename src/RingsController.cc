//
//  RingsController.cc
//  concentric-clock
//
//  Created by Morgan Davis on 8/18/19.
//  Copyright © 2019 Morgan K Davis. All rights reserved.
//

#include "RingsController.h"

#include <iostream>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "clk.h"
#include "gpio.h"
#include "dma.h"
#include "pwm.h"


using namespace std;
using namespace concentric_clock;


constexpr unsigned TARGET_FREQ =		WS2811_TARGET_FREQ;
constexpr unsigned GPIO_PIN =			18;
constexpr unsigned DMA = 				10;
constexpr unsigned STRIP_TYPE =			WS2811_STRIP_GBR;

constexpr unsigned LED_TOTAL_LENGTH =	1 + 8 + 12 + 16 + 24 + 32 + 40 + 48 + 60;
constexpr unsigned LED_RING_LENGTHS[] = { 60, 48, 40, 32, 24, 16, 12, 8, 1 };
constexpr unsigned NUM_LED_RINGS = 		9;

/**************************************************************************************
     Static Prototypes
 **************************************************************************************/

static void Sleep(unsigned milliseconds);
static ws2811_led_t ColorToLED(RingsController::Color color, bool applyIntensity = false);

/**************************************************************************************
     Lifecycle
 **************************************************************************************/

RingsController::RingsController():
		//m_mtx(make_shared<mutex>()),
		//m_updateThread(nullptr),
		//m_stop(nullptr),
		m_matrix(nullptr) {

//	if (initMatrix()) {
//		m_updateThread = thread(&RingsController::updateLoop, this);
//	}

	if (initMatrix()) {

	}
}

RingsController::~RingsController() {

//	matrix_clear();
//	ws2811_render(&ledstring);

	ws2811_fini(m_ledString);

	if (m_ledString) {
		// m_ledString->channel[0]
		free(m_ledString);
	}

	if (m_matrix) {
		free(m_matrix);
	}
}

/**************************************************************************************
     Public
 **************************************************************************************/

// new -- fractional
void RingsController::setPercentage(unsigned ring, float percentage, Color color, bool fill) {

	int start = 0;
	for (unsigned s = 0; s < ring; ++s) {
		start += LED_RING_LENGTHS[s];
	}
	int end = start + LED_RING_LENGTHS[ring];

	float targetReal = start + ((float)LED_RING_LENGTHS[ring]) * percentage;
	int targetFloor = (unsigned)floorf(targetReal);
	float targetFraction = (targetReal >= 1) ? (targetReal - (float)targetFloor) : (targetReal);

	for (int i = end-1; i >= start; --i) {

		ws2811_led_t led = ColorToLED(color);

		if (i == targetFloor) {

			m_ledString->channel[0].leds[i] = led;

			if (i+1 < end) {
				// TODO: use a logorithmic function here
				ws2811_led_t fractionalLed = ColorToLED({color.r, color.g, color.b,
											 lroundf(255.0f * targetFraction * 0.75f)}, true);
				m_ledString->channel[0].leds[i+1] = fractionalLed;
			}
		}
		else if (i < targetFloor) {
			if (fill) {
				m_ledString->channel[0].leds[i] = led;
			}
			else {
				m_ledString->channel[0].leds[i] = 0;
			}
		}
		else { // l > targetFloor
			m_ledString->channel[0].leds[i] = 0;
		}
	}

	//render();
}

// OG -- WORKS
//void RingsController::setPercentage(unsigned ring, float percentage, Color color, bool fill) {
//
//	cout << "setPercentage(), ring: " << ring << ", percentage: " << percentage << endl;
//
//	int start = 0;
//	for (unsigned s = 0; s < ring; ++s) {
//		start += LED_RING_LENGTHS[s];
//	}
//	int end = start + LED_RING_LENGTHS[ring];
//
//
//	// whole numbers -- WORKS
//
//	unsigned targetLED = start + (unsigned)lroundf(((float)LED_RING_LENGTHS[ring]) * percentage);
//	for (unsigned i = start; i < end; ++i) {
//
//		ws2811_led_t led = ColorToLED(color);
//
//		if (i == targetLED) {
//			m_ledString->channel[0].leds[i] = led;
//		}
//		else if (i < targetLED) {
//			if (fill) {
//				m_ledString->channel[0].leds[i] = led;
//			}
//			else {
//				m_ledString->channel[0].leds[i] = 0;
//			}
//		}
//		else { // l > targetLED
//			m_ledString->channel[0].leds[i] = 0;
//		}
//	}
//
//	//render();
//}

void RingsController::setBrightness(float brightness) {

	m_ledString->channel[0].brightness = lroundf(255.0f * brightness);

	//render();
}

/**************************************************************************************
     Private
 **************************************************************************************/

//void RingsController::updateLoop() {
//
//	ws2811_return_t r;
//
////	while (m_mtx.lock(), !m_stop) {
////		m_mtx.unlock();
//
//
////		matrix_raise();
////		matrix_bottom();
////		matrix_render();
//
////		if ((ret = ws2811_render(m_ledString)) != WS2811_SUCCESS)
////		{
////			fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
////			break;
////		}
//
//		//Sleep(1 * 33);
//
////		auto lux = AmbientLight();
////		m_mtx.lock();
////		m_ambientLight = lux;
////		m_mtx.unlock();
////
////		auto celsius = AmbientTemperature();
////		m_mtx.lock();
////		m_ambientTemperature = celsius;
////		m_mtx.unlock();
////
////		Sleep(25);
////	}
//
////	matrix_clear();
////	ws2811_render(&ledstring);
//
////	ws2811_fini(&ledstring);
//}

bool RingsController::initMatrix() {

	//m_matrix = static_cast<ws2811_led_t*>(malloc(LED_TOTAL_LENGTH), &RingsController::freeMatrix);
	//m_matrix = static_cast<ws2811_led_t*>(malloc(sizeof(ws2811_led_t) * LED_TOTAL_LENGTH));
	m_matrix = (ws2811_led_t*)malloc(sizeof(ws2811_led_t) * LED_TOTAL_LENGTH);

	//m_ledString = static_cast<ws2811_channel_t>(malloc(sizeof(ws2811_channel_t)));

	m_ledString = (ws2811_t*)malloc(sizeof(ws2811_t));

	m_ledString->freq = TARGET_FREQ;
	m_ledString->dmanum = DMA;
	//m_ledString->channel[0] = static_cast<ws2811_channel_t*>(malloc(sizeof(ws2811_channel_t)));
	//m_ledString->channel = (ws2811_channel_t*)malloc(sizeof(ws2811_channel_t));
	m_ledString->channel[0].gpionum = GPIO_PIN;
	m_ledString->channel[0].count = LED_TOTAL_LENGTH;
	m_ledString->channel[0].invert = 0;
	m_ledString->channel[0].brightness = 255;
	m_ledString->channel[0].strip_type = STRIP_TYPE;
//	m_ledString->channel[0] = static_cast<ws2811_channel_t>(malloc(sizeof(ws2811_channel_t)));
//	m_ledString->channel[1].gpionum = 0;
//	m_ledString->channel[1].count = 0;
//	m_ledString->channel[1].invert = 0;
//	m_ledString->channel[1].brightness = 0;

//	m_ledString = new ws2811_t {
//			.freq = TARGET_FREQ,
//			.dmanum = DMA,
//			.channel = {
//					[0] = {
//							.gpionum = GPIO_PIN,
//							.count = LED_TOTAL_LENGTH,
//							.invert = 0,
//							.brightness = 255,
//							.strip_type = STRIP_TYPE,
//					},
//					[1] = {
//							.gpionum = 0,
//							.count = 0,
//							.invert = 0,
//							.brightness = 0,
//					},
//			},
//	};

	ws2811_return_t r;

	if ((r = ws2811_init(m_ledString)) == WS2811_SUCCESS)
	{
		return true;
	}
	else {
		fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(r));
		m_stop = true;
	}

	return false;
}

void RingsController::render() {

	ws2811_return_t r;

	if ((r = ws2811_render(m_ledString)) != WS2811_SUCCESS) {

		fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(r));
	}
}

//void RingsController::freeMatrix() {
//
//	if (m_matrix) {
//		free(m_matrix);
//	}
//}

/**************************************************************************************
     Static
 **************************************************************************************/

void Sleep(unsigned milliseconds) {

	usleep(milliseconds * 1000);
}

//RingsController::Colo AdjustedBrightness(RingsController::Color color, float brightness) {
//
//	return (RingsController::Color color){color.r, color.g, color.b};
//}

//ws2811_led_t ColorToLED(RingsController::Color color) {
//
//	ws2811_led_t led = 0x00000000; // the header says: '0xWWRRGGBB' but we're observed 'WWBBGGRR'
//
//	ws2811_led_t r = color.r << 0;
//	ws2811_led_t g = color.g << 8;
//	ws2811_led_t b = color.b << 16;
//	ws2811_led_t w = color.w << 24;
//
//	led |= r;
//	led |= g;
//	led |= b;
//	led |= w;
//}

ws2811_led_t ColorToLED(RingsController::Color color, bool applyIntensity) {

	ws2811_led_t led = 0x00000000; // the header says: '0xWWRRGGBB' but we're observed 'WWBBGGRR'

	if (applyIntensity) {
		float intensity = (float)color.w / 255.0f;

		ws2811_led_t r = lroundf(((float)color.r) * intensity) << 0;
		ws2811_led_t g = lroundf(((float)color.g) * intensity) << 8;
		ws2811_led_t b = lroundf(((float)color.b) * intensity) << 16;
		//ws2811_led_t w = color.w << 24;
		ws2811_led_t w = 0;

		led |= r;
		led |= g;
		led |= b;
		led |= w;
	}
	else {
		ws2811_led_t r = color.r << 0;
		ws2811_led_t g = color.g << 8;
		ws2811_led_t b = color.b << 16;
		ws2811_led_t w = color.w << 24;

		led |= r;
		led |= g;
		led |= b;
		led |= w;
	}
}
