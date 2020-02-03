//
//  ConcentricClock.cc
//  concentric-clock
//
//  Created by Morgan Davis on 8/9/19.
//  Copyright © 2019 Morgan K Davis. All rights reserved.
//


#include "ConcentricClock.h"

//#include <algorithm>
#include <ctime>
#include <chrono>
#include <ctime>
//#include <iomanip>
#include <iostream>
//#include <math.h>
#include <limits.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "Beeper.h"
#include "RingsController.h"
#include "SensorHub.h"


using namespace std;
using namespace concentric_clock;


/**************************************************************************************
     Constants
 **************************************************************************************/

//constexpr unsigned ACONST =				AVALUE;

/**************************************************************************************
     Types
 **************************************************************************************/

/*enum class THING {
	ONE,
	TWO,
	THREE
};*/

enum class RINGS: unsigned {

//	MILLISECONDS = 0,
//	SECONDS = 1,
//	MINUTES = 2,
//	HOURS = 3

	SECONDS = 0,
	MINUTES = 1,
	HOURS = 2,
	WEEK = 3,
	MONTH = 4,
	YEAR = 5,
	CENTURY = 6
};

/**************************************************************************************
     Static Prototypes
 **************************************************************************************/

static float Time();
static string TimeString();
static void Sleep(unsigned milliseconds);
static int Uniform(int min, int max);
static float Uniform(float min, float max);
static void GetTimeComponents(unsigned* h, unsigned* m, unsigned* s, unsigned* ms);

/**************************************************************************************
     Lifecycle
 **************************************************************************************/

ConcentricClock::ConcentricClock():
	m_sensorHub(make_shared<SensorHub>()),
	m_ringsController(make_shared<RingsController>()),
	m_beeper(make_shared<Beeper>()),
	m_stop(false) {

}

ConcentricClock::~ConcentricClock() {

}

/**************************************************************************************
     Public
 **************************************************************************************/

int ConcentricClock::start(const vector<string>& args) {

        while (!m_stop) {
                update();
        }

        shutdown();
}

void ConcentricClock::stop() {
        cout << "ConcentricClock::stop()" << endl;

        m_stop = true;
}


/**************************************************************************************
     Private
 **************************************************************************************/

void ConcentricClock::update() {

	//cout << "update()" << endl;

	static bool setBrightness = false;
	if (!setBrightness) m_ringsController->setBrightness(0.25f);
	setBrightness = true;

	static RingsController::Color msColor = { 150, 150, 150, 255 };
	static RingsController::Color sColor = { 95, 0, 172, 255 }; // purple
	static RingsController::Color mColor = { 16, 161, 0, 255 }; // green
	static RingsController::Color hColor = { 0, 9, 255, 255 }; // blue

	unsigned h, m, s, ms;
	GetTimeComponents(&h, &m, &s, &ms);


//	{ // OG -- works
//
//	static unsigned lastH = INT_MAX;
//	static unsigned lastM = INT_MAX;
//	static unsigned lastS = INT_MAX;
//	static unsigned lastMS = INT_MAX;
//
////	if (ms != lastMS) m_ringsController->setPercentage((unsigned)RINGS::MILLISECONDS,
////			((float)ms)/1000.0f, msColor, false);
//	if (s != lastS) m_ringsController->setPercentage((unsigned)RINGS::SECONDS,
//			((float)s)/60.0f, sColor, true);
//	if (m != lastM) m_ringsController->setPercentage((unsigned)RINGS::MINUTES,
//			((float)m)/60.0f, mColor, true);
//	if (h != lastH) m_ringsController->setPercentage((unsigned)RINGS::HOURS,
//			((float)h)/12.0f, hColor, true);
//
//		lastH = h;
//		lastM = m;
//		lastS = s;
//		lastMS = ms;
//	}


	{ // new - fractional

		float sFraction = ((float)s + ms / 1000.0f) / 60.0f;
		float mFraction = ((float)m + sFraction) / 60.0f;
		float hFraction = ((float)h + mFraction) / 12.0f;

		m_ringsController->setPercentage((unsigned)RINGS::SECONDS,
										 sFraction, sColor, true);
		m_ringsController->setPercentage((unsigned)RINGS::MINUTES,
										 mFraction, mColor, true);
		m_ringsController->setPercentage((unsigned)RINGS::HOURS,
										 hFraction, hColor, true);
	}

//	{ // new - fractional + milliseconds
//
//		float msFraction = (float)ms / 1000.0f;
//		float sFraction = ((float)s + ms / 1000.0f) / 60.0f;
//		float mFraction = ((float)m + sFraction) / 60.0f;
//		float hFraction = ((float)h + mFraction) / 12.0f;
//
//		m_ringsController->setPercentage((unsigned) RINGS::MILLISECONDS,
//										 msFraction, msColor, true);
//		m_ringsController->setPercentage((unsigned) RINGS::SECONDS,
//										 sFraction, sColor, true);
//		m_ringsController->setPercentage((unsigned)RINGS::MINUTES,
//										 mFraction, mColor, true);
//		m_ringsController->setPercentage((unsigned)RINGS::HOURS,
//										 hFraction, hColor, true);
//	}

	m_ringsController->render();


//	auto lux = m_sensorHub->ambientLight();
//	cout << "lux: " << lux << endl;
//
//
//
//	static bool beeping = false;
//	if (beeping) {
//		m_beeper->off();
//		beeping = false;
//	}
//	else {
//		m_beeper->on();
//		beeping = true;
//	}

	Sleep(1000 / 144); // Hz
}

void ConcentricClock::shutdown() {
        cout << "ConcentricClock:::shutdown()" << endl;

}


/**************************************************************************************
     Static
 **************************************************************************************/

float Time() {
	
	static auto startDate = chrono::high_resolution_clock::now();
	auto nowDate = chrono::high_resolution_clock::now();
	return (chrono::duration<float>(nowDate - startDate)).count();
}

string TimeString() {
	
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];
	
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	
	strftime(buffer, sizeof(buffer), "%l:%M", timeinfo);
	
	return string(buffer);
}

void Sleep(unsigned milliseconds) {
	
	usleep(milliseconds * 1000);
}

int Uniform(int min, int max) {
	static random_device rd;
	static mt19937 gen(rd());
	uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

float Uniform(float min, float max) {
	static random_device rd;
	static mt19937 gen(rd());
	uniform_real_distribution<> dis(min, max);
	return dis(gen);
}

void GetTimeComponents(unsigned* h, unsigned* m, unsigned* s, unsigned* ms) {

	timeval now;
	gettimeofday(&now, NULL); // gettimeofday() is POSIX
	int milli = now.tv_usec / 1000;

	char hBuf[32];
	char mBuf[32];
	char sBuf[32];
	char msBuf[32];

	strftime(hBuf, sizeof(hBuf), "%H", localtime(&now.tv_sec));
	strftime(mBuf, sizeof(mBuf), "%M", localtime(&now.tv_sec));
	strftime(sBuf, sizeof(sBuf), "%S", localtime(&now.tv_sec));
	snprintf(msBuf, sizeof(msBuf), "%d", milli);

	*h = atol(hBuf);
	if (*h >= 12) *h -= 12;
	*m = atol(mBuf);
	*s = atol(sBuf);
	*ms = atol(msBuf);
}

