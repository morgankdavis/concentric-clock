//
//  Beeper.h
//  concentric-clock
//
//  Created by Morgan Davis on 8/9/19.
//  Copyright © 2019 Morgan K Davis. All rights reserved.
//


#ifndef Beeper_h
#define Beeper_h


namespace concentric_clock {

	class Beeper {
		
	public:
		
		/**************************************************************************************
		     Lifecycle
		 **************************************************************************************/
		
		Beeper();
		Beeper(Beeper&& other) = delete; // move initialization
		Beeper(const Beeper& other) = delete; // copy initialization
		Beeper& operator= (Beeper&& other) = delete; // move assignment
		Beeper& operator= (const Beeper& other) = delete; // copy assignment
		~Beeper();
		
		/**************************************************************************************
		     Public
		 **************************************************************************************/
		
		void on();
		void off();
		
		bool beeping() const;
		
	private:

		/**************************************************************************************
		     Private
		 **************************************************************************************/
		
		bool	m_beeping;
	};
}


#endif /* Beeper_h */

