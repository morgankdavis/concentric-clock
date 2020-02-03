//
//  RingsController.h
//  concentric-clock
//
//  Created by Morgan Davis on 8/18/19.
//  Copyright © 2019 Morgan K Davis. All rights reserved.
//


#ifndef RingsController_h
#define RingsController_h


#include <memory>
//#include <mutex>
//#include <thread>

#include "ws2811.h"


// TODO: allow defining GPIO pin
// TODO: wrap ws2811_led_t and ws2811_t in shared_ptrs


namespace concentric_clock {

	class RingsController {

	public:

		/**************************************************************************************
		     Types
		 **************************************************************************************/

		typedef struct {
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t w;
		} Color;

		/**************************************************************************************
		     Lifecycle
		 **************************************************************************************/

		RingsController();
		RingsController(RingsController&& other) = delete; // move initialization
		RingsController(const RingsController& other) = delete; // copy initialization
		RingsController& operator= (RingsController&& other) = delete; // move assignment
		RingsController& operator= (const RingsController& other) = delete; // copy assignment
		~RingsController();

		/**************************************************************************************
		     Public
		 **************************************************************************************/

		void setPercentage(unsigned ring, float percentage, Color color, bool fill = false);
		void setBrightness(float brightness);
		void render();

	private:

		/**************************************************************************************
		     Private
		 **************************************************************************************/

		//void updateLoop();
		bool initMatrix();
		//void render();
		//void freeMatrix();

//		std::mutex						m_mtx;
//		std::thread						m_updateThread;
		bool							m_stop;

		//std::shared_ptr<ws2811_led_t>	m_matrix;
		ws2811_led_t*					m_matrix; // TODO: rename 'm_ledBuffer'
		ws2811_t* 						m_ledString; // TODO: rename 'm_ws2811'

		//bool	m_beeping;
	};
}


#endif /* RingsController.h */

