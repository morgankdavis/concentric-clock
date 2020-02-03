//
//  SensorHub.h
//  concentric-clock
//
//  Created by Morgan Davis on 8/9/19.
//  Copyright © 2019 Morgan K Davis. All rights reserved.
//


#ifndef SensorHub_h
#define SensorHub_h


#include <mutex>
#include <thread>


namespace concentric_clock {

	class SensorHub {
		
	public:
		
		/**************************************************************************************
		     Lifecycle
		 **************************************************************************************/
		
		SensorHub();
		SensorHub(SensorHub&& other) = delete; // move initialization
		SensorHub(const SensorHub& other) = delete; // copy initialization
		SensorHub& operator= (SensorHub&& other) = delete; // move assignment
		SensorHub& operator= (const SensorHub& other) = delete; // copy assignment
		~SensorHub();
		
		/**************************************************************************************
		     Public
		 **************************************************************************************/

		unsigned ambientLight();
		void shutdown();
		
	private:
		
		/**************************************************************************************
		     Private
		 **************************************************************************************/

		void updateLoop();
		
		std::mutex			m_mtx;
		std::thread			m_updateThread;
		bool				m_stop;
		
		unsigned 			m_ambientLight;
	};
}


#endif /* SensorHub_h */

