//
//  ConcentricClock.h
//  concentric-clock
//
//  Created by Morgan Davis on 8/9/19.
//  Copyright © 2019 Morgan K Davis. All rights reserved.
//


#ifndef ConcentricClock_h
#define ConcentricClock_h


#include <memory>
#include <string>
#include <vector>


#define DEBUGGING


// TODO: add config file


namespace concentric_clock {
	
	
	class Beeper;
	class RingsController;
	class SensorHub;

	class ConcentricClock {
		
	public:
		
		/**************************************************************************************
		     Lifecycle
		 **************************************************************************************/
		
		ConcentricClock();
		~ConcentricClock();
		
		/**************************************************************************************
		     Public
		 **************************************************************************************/
		
		int start(const std::vector<std::string>& args);
		void stop();
		
	private:
		
		/**************************************************************************************
		     Private
		 **************************************************************************************/

		void update();
		void shutdown();

		std::shared_ptr<RingsController>	m_ringsController;
		std::shared_ptr<SensorHub>			m_sensorHub;
		std::shared_ptr<Beeper>				m_beeper;

		bool								m_stop;
	};
}


#endif /* ConcentricClock_h */

