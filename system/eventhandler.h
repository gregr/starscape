// eventhandler.h
// Gregory Rosenblatt
// 5/7/05

#ifndef Starscape_System_EventHandler_H_
#define Starscape_System_EventHandler_H_

#include "input/keyinfo.h"

namespace Starscape {

	namespace System {

		/** A base event handler to be derived from for specialization.
		 * Only events requiring special handling have to be overloaded.
		 * Event handlers are intended to be compile-time polymorphic.
		 * @sa Context
		 */
		class EventHandler {
		public:
			/** Handles the user's attempt to quit.
			 * @return true to quit, false to abort.
			 */
			bool OnQuit() const	{ return true; }
			/** Handles movement of the mouse.
			 * @param x The new mouse x-coordinate.
			 * @param y The new mouse y-coordinate.
			 * @param xrel The x-distance moved.
			 * @param yrel The y-distance moved.
			 */
			void OnMouseMotion(int x, int y, int xrel, int yrel) const	{}
			/** Handles the pressing of a mouse button.
			 * @param button The id of the pressed button.
			 */
			void OnMouseButtonDown(int button) const	{}
			/** Handles the release of a mouse button.
			 * @param button The id of the released button.
			 */
			void OnMouseButtonUp(int button) const	{}
			/** Handles a key being pressed.
			 * @param sym The key symbol pressed.
			 * @param mod Flag containing the key modifiers currently pressed.
			 */
			void OnKeyDown(const Input::KeyInfo& key) const	{}
			/** Handles a key being released.
			 * @param sym The key symbol released.
			 * @param mod Flag containing the key modifiers currently pressed.
			 */
			void OnKeyUp(const Input::KeyInfo& key) const	{}
			/** Handles the resizing of the main application window.
			 * Width and Height should be accessed using DisplayMetrics.
			 */
			void OnResize() const	{}
			/** Handles the application's gain or loss of an active state.
			 * Minimization or restoration of the window triggers this event.
			 * @param isActive Determines if the window is visible/active.
			 */
			void OnActivate(bool isActive) const	{}
		};
	}
}

#endif
