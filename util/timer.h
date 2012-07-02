// timer.h
// Gregory Rosenblatt
// 5/11/05

#ifndef Starscape_Timer_H_
#define Starscape_Timer_H_

#include "system/timing.h"

namespace Starscape {

	/** Used to calculate elapsed time within a certain interval. */
	class Timer {
		unsigned int	current_;
	public:
		static unsigned int TotalTime()	{ return System::TotalTime(); }
		Timer() : current_(TotalTime())	{}
		/** Retrieves the time elapsed since this operator's last call. */
		unsigned int operator()() {
			unsigned int old = current_;
			current_ = TotalTime();
			return current_-old;
		}
	};
}

#endif
