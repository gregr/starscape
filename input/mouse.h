// mouse.h
// Gregory Rosenblatt
// 3/23/05

#ifndef Starscape_Input_Mouse_H_
#define Starscape_Input_Mouse_H_

#include "SDL.h"

namespace Starscape {

	namespace Input {

		/** Implements handling of the mouse input device.
		 * - ListenerType interface requirements:
		 * 	- int PosX();
		 * 	- int PosY();
		 * 	- Warp(unsigned int, unsigned int);
		 * 	- OnMove(int, int);
		 * 	- OnButtonDown(int);
		 * 	- OnButtonUp(int);
		 */
		template <class ListenerType>
		class Mouse {
		public:
			/** Creates a mouse that is bound to a listening object.
			 * @param listener The object that the mouse conveys its messages to.
			 */
			Mouse(ListenerType& listener)
				: listener_(listener), grabbed_(false) {}
			/** The current x-position of the cursor. */
			int PosX() const	{ return listener_.PosX(); }
			/** The current y-position of the cursor. */
			int PosY() const	{ return listener_.PosY(); }
			/** Handles mouse movement. */
			void OnMotion(int xpos, int ypos, int xrel, int yrel) {
				xabs_ = xpos;
				yabs_ = ypos;
				listener_.OnMove(xrel, yrel);
			}
			/** Handles a mouse button being pressed. */
			void OnButtonDown(int button) {
				if (!grabbed_)
					SDL_WM_GrabInput(SDL_GRAB_ON);
				grabbed_ = true;
				listener_.OnButtonDown(button);
			}
			/** Handles a mouse button being released. */
			void OnButtonUp(int button) {
				if (grabbed_) {
					grabbed_ = false;
					SDL_WM_GrabInput(SDL_GRAB_OFF);
					Restore();
				}
				listener_.OnButtonUp(button);
			}
		private:
			// resynchronizes the system mouse position with the listener's internal data
			void Restore() {
				SDL_WarpMouse(PosX(), PosY());
				listener_.Warp(xabs_, yabs_);
			}
			ListenerType&	listener_;
			int				xabs_, yabs_;
			bool			grabbed_;
		};
	}
}

#endif
