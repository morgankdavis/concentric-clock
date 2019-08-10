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
//#include <iomanip>
#include <iostream>
//#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


using namespace std;
using namespace cc;


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

/**************************************************************************************
     Static Prototypes
 **************************************************************************************/

static float Time();
static string TimeString();
static void Sleep(unsigned milliseconds);

/**************************************************************************************
     Lifecycle
 **************************************************************************************/

ConcentricClock::ConcentricClock() {

}

ConcentricClock::~ConcentricClock() {

}

/**************************************************************************************
     Public
 **************************************************************************************/

int ConcentricClock::start(const vector<string>& args) {

        cout << "Ready." << endl;

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
