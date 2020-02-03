//
//  SensorHub.cc
//  concentric-clock
//
//  Created by Morgan Davis on 8/9/19.
//  Copyright © 2019 Morgan K Davis. All rights reserved.
//


#include "SensorHub.h"

#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>


using namespace std;
using namespace concentric_clock;


/**************************************************************************************
     Static Prototypes
 **************************************************************************************/

static unsigned AmbientLight();
static void Sleep(unsigned milliseconds);

/**************************************************************************************
     Lifecycle
 **************************************************************************************/

SensorHub::SensorHub():
	m_updateThread(thread(&SensorHub::updateLoop, this)),
	m_stop(nullptr),
	m_ambientLight(0) {
	
		//m_updateThread = thread(&SensorHub::updateLoop, this);
}

SensorHub::~SensorHub() {
	
}

/**************************************************************************************
     Public
 **************************************************************************************/

unsigned SensorHub::ambientLight() {
	lock_guard<mutex> guard(m_mtx);
	
	return m_ambientLight;
}

void SensorHub::shutdown() {
	
	m_mtx.lock();
	m_stop = true;
	m_mtx.unlock();
	cout << "Waiting for sensor poll loop to complete..." << endl;
	m_updateThread.join();
}

/**************************************************************************************
     Private
 **************************************************************************************/

void SensorHub::updateLoop() {
	
	while (m_mtx.lock(), !m_stop) {
		m_mtx.unlock();
		
		auto lux = AmbientLight();
		m_mtx.lock();
		m_ambientLight = lux;
		m_mtx.unlock();
		
		Sleep(500);
	}
}                  

/**************************************************************************************
     Static
 **************************************************************************************/

// TSL2591
unsigned AmbientLight() {

	// SUPER HELPFUL
	// https://github.com/maxlklaxl/python-tsl2591/blob/master/tsl2591/read_tsl.py
	
	static const char *BUS_LOCATION = (char *)"/dev/i2c-1";
	static const unsigned char I2C_ADDR = 0x29;
	
	// This code is designed to work with the TSL2561_I2CS I2C Mini Module available from ControlEverything.com.
	// https://www.controleverything.com/content/Light?sku=TSL2561_I2CS#tabs-0-product_tabset-2
	
	// Create I2C bus
	static int fd = -1;
	if (fd == -1) {
		//static const char *bus = (char *)"/dev/i2c-0";
		if ((fd = open(BUS_LOCATION, O_RDWR)) < 0)  {
			printf("Failed to open TSL2561 I2C bus.\n");
			exit(1); // TODO: don't kill the whole process.
		}
		// get I2C device, TSL2561 I2C address is 0x39(57)
		if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {
			printf("Failed to get TSL2561 I2C device port.\n");			
			close(fd);
			exit(1); // TODO: don't kill the whole process.
		}
		
		// Select control register(0x00 | 0x80)
		// Power ON mode(0x03)
		char config[2] = {0};
		config[0] = 0xA0 | 0x00; // CHANGED
		//config[1] = 0x03;
		config[1] = 0x01 | 0x02 | 0x10; // CHANGED
		write(fd, config, 2);
		// Select timing register(0x01 | 0x80)
		// Nominal integration time = 402ms(0x02)
		config[0] = 0xA0 | 0x01; // CHANGED
		config[1] = 0x02; // 300ms
		write(fd, config, 2);
		//sleep(1);
	}

/*
// get full spectrum lux

	int full, ir;

	unsigned char reg[1] = {0xA0 | 0x14};
	write(fd, reg, 1);

	char data[4] = {0};
	if (read(fd, data, 4) != 4) {
		printf("TSL2561 I2C input/output error.\n");
	}
	else {
		full = (data[3] * 256*3, data[2] * 256*2, data[1] * 256*1 + data[0]);
	}

// get it

	reg[1] = {0xA0 | 0x16};
	write(fd, reg, 1);

	data[4] = {0};
	if (read(fd, data, 4) != 4) {
		printf("TSL2561 I2C input/output error.\n");
	}
	else {
		ir = (data[3] * 256*3, data[2] * 256*2, data[1] * 256*1 + data[0]);
	}

	int visible = full - ir;

	cout << "full: " << full << endl;
	cout << "ir: " << ir << endl;
	cout << "visible: " << visible << endl;

	return visible;
	*/

	// Read 4 bytes of data from register(0x0C | 0x80)
	// ch0 lsb, ch0 msb, ch1 lsb, ch1 msb
	unsigned char reg[1] = {0xA0 | 0x14};
	write(fd, reg, 1);

	char data[4] = {0};
	if (read(fd, data, 4) != 4) {
		printf("TSL2561 I2C input/output error.\n");
	}
	else {
		float real = (data[3] * 256*3, data[2] * 256*2, data[1] * 256*1 + data[0]);

		float full = (data[1] * 256 + data[0]);
		float ir = (data[3] * 256 + data[2]);
		float vis = full - ir;
		
//		cout << "real: " << real << endl; // lux
//		cout << "full: " << full << endl;
//		cout << "ir: " << ir << endl;
//		cout << "vis: " << vis << endl;

		return vis;
	}

	
	return 0;
}

// TSL2561
/*unsigned AmbientLight() {
	
	static const char *BUS_LOCATION = (char *)"/dev/i2c-1";

	// "The ADDR pin can be used if you have an i2c address conflict, to change the address.
	// Connect it to ground to set the address to 0x29, connect it to 3.3V (vcc) to se t the
	// address to 0x49 or leave it floating (unconnected) to use address 0x39."
	// https://learn.adafruit.com/tsl2561/wiring-the-tsl2561-sensor
	static const unsigned char I2C_ADDR = 0x29;
	
	// This code is designed to work with the TSL2561_I2CS I2C Mini Module available from ControlEverything.com.
	// https://www.controleverything.com/content/Light?sku=TSL2561_I2CS#tabs-0-product_tabset-2
	
	// Create I2C bus
	static int fd = -1;
	if (fd == -1) {
		//static const char *bus = (char *)"/dev/i2c-0";
		if ((fd = open(BUS_LOCATION, O_RDWR)) < 0)  {
			printf("Failed to open TSL2561 I2C bus.\n");
			exit(1); // TODO: don't kill the whole process.
		}
		// get I2C device, TSL2561 I2C address is 0x39(57)
		if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {
			printf("Failed to get TSL2561 I2C device port.\n");			
			close(fd);
			exit(1); // TODO: don't kill the whole process.
		}
		
		// Select control register(0x00 | 0x80)
		// Power ON mode(0x03)
		char config[2] = {0};
		config[0] = 0x00 | 0x80;
		config[1] = 0x03;
		write(fd, config, 2);
		// Select timing register(0x01 | 0x80)
		// Nominal integration time = 402ms(0x02)
		config[0] = 0x01 | 0x80;
		config[1] = 0x02;
		write(fd, config, 2);
		//sleep(1);
	}
	
	// Read 4 bytes of data from register(0x0C | 0x80)
	// ch0 lsb, ch0 msb, ch1 lsb, ch1 msb
	unsigned char reg[1] = {0x0C | 0x80};
	write(fd, reg, 1);
	char data[4] = {0};
	if (read(fd, data, 4) != 4) {
		printf("TSL2561 I2C input/output error.\n");
	}
	else {
		float full = (data[1] * 256 + data[0]);
		float inf = (data[3] * 256 + data[2]);
		float vis = full - inf;
		
		return vis;
	}
	
	return 0;
}*/

void Sleep(unsigned milliseconds) {
	
	usleep(milliseconds * 1000);
}

