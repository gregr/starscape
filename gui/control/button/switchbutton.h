// switchbutton.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_SwitchButton_H_
#define Starscape_Gui_SwitchButton_H_

#include "gui/control/button/button.h"
#include "gui/command.h"
#include "gui/config/inputconstants.h"

namespace Starscape {

	namespace Gui {

		/** A button that switches between two states when pressed. */
		class SwitchButton : public Button {
		public:
			SwitchButton(const ButtonBrush& brush
						,const SwitchCommand& command
						,const char* caption = "")
				: Button(brush, caption), command_(command)	{}
			// events
			void OnButtonDown(int button) {
				if (button == STARSCAPE_BUTTON_LEFT) {
					ToggleState();
					ExecuteCommand();
				}
			}
			/** Retrieve the current state. */
			bool GetState() const	{ return IsPressed(); }
			/** Directly changes the current state. */
			void SetState(bool state)	{ pressed_ = state; }
			/** Switches the button state. */
			void ToggleState()	{ pressed_ = !pressed_; }
		private:
			SwitchCommand	command_;
		protected:
			void ExecuteCommand()	{ if (command_) command_(IsPressed()); }
		};
	}
}

#endif
