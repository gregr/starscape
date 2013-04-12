// windowmanagement.h
// Gregory Rosenblatt
// 5/6/05

#ifndef Starscape_System_WindowManagement_H_
#define Starscape_System_WindowManagement_H_

#include "exception.h"
#include "SDL.h"

namespace Starscape {

	namespace System {

		const char* const ERR_POST_QUIT_EVENT =
			"System: Unable to post quit event.";
		const char* const ERR_MINIMIZE_WINDOW =
			"System: Unable to minimize window.";

		const char* const DEFAULT_WINDOW_CAPTION =	"Starscape Window";

		/** Sends a Quit event to the application message queue. */
		inline void PostQuitEvent() {
			SDL_Event event;
			event.type = SDL_QUIT;
			if (SDL_PushEvent(&event) < 0)
				throw Exception(ERR_POST_QUIT_EVENT, SDL_GetError());
		}

		/** Makes the system cursor visible. */
		inline void ShowCursor()	{ SDL_ShowCursor(SDL_ENABLE); }

		/** Makes the system cursor invisible. */
		inline void HideCursor()	{ SDL_ShowCursor(SDL_DISABLE); }

		/** Sets the caption for the main application window. */
		inline void SetCaption(const char* caption=DEFAULT_WINDOW_CAPTION
							,const char* icon=0) {
			SDL_WM_SetCaption(caption, icon);
		}

		/** Minimizes the main application window. */
		inline void Minimize() {
			if (!SDL_WM_IconifyWindow())
				throw Exception(ERR_MINIMIZE_WINDOW, SDL_GetError());
		}
	}
}

#endif
