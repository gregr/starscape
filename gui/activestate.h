// activestate.h
// Gregory Rosenblatt
// 5/20/05

#ifndef Starscape_Gui_ActiveState_H_
#define Starscape_Gui_ActiveState_H_

#include "gui/window.h"

namespace Starscape {

	namespace Gui {

		/** References the active and keyboard focus windows. */
		class ActiveState {
		public:
			ActiveState(Window& owner)
				: activeFocus_(&owner), keyboardFocus_(&owner)	{}
			/** Provides the currently active window. */
			Window& GetActiveFocus()	{ return *activeFocus_; }
			/** Provides the window that has keyboard focus. */
			Window& GetKeyboardFocus()	{ return *keyboardFocus_; }
			/** Sets which window is currently active. */
			void SetActiveFocus(Window& focus) {
				if (activeFocus_ != &focus) {
					activeFocus_->OnLoseFocus();
					activeFocus_ = &focus;
				}
				GetActiveFocus().OnKeyboardGrab(*this);
				GetActiveFocus().MoveToTop();
			}
			/** Sets which window has keyboard focus. */
			void SetKeyboardFocus(Window& focus) {
				if (keyboardFocus_ != &focus) {
					keyboardFocus_->OnKeyboardRelease();
					keyboardFocus_ = &focus;
				}
			}
		private:
			Window	*activeFocus_, *keyboardFocus_;
		};
	}
}

#endif
