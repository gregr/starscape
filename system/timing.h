// timing.h
// Gregory Rosenblatt
// 2/17/05

#ifndef Starscape_System_Timing_H_
#define Starscape_System_Timing_H_

#include "SDL.h"

namespace Starscape {

	namespace System {

		/** Prevents an application from using excessive processing time. */
		inline void ShortPause()	{ SDL_Delay(5); }
		// 10 ms is the normal timer resolution for most platforms.
		// Because of this, 5 ms has a special property associated
		// with it that makes it the optimal delay time to use to
		// avoid hurting application performance.

		/** The total running time of the application. */
		inline unsigned int TotalTime() {
			return SDL_GetTicks();
		}
	}
}

#endif
