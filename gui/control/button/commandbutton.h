// commandbutton.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_CommandButton_H_
#define Starscape_Gui_CommandButton_H_

#include "gui/control/button/button.h"
#include "gui/command.h"
#include "gui/config/inputconstants.h"

namespace Starscape {

	namespace Gui {

		class CommandButton : public Button {
		public:
			CommandButton(const ButtonBrush& brush
						,const Command& command
						,const char* caption = "")
				: Button(brush, caption), command_(command)	{}
			void OnButtonDown(int button) {
				if (button == STARSCAPE_BUTTON_LEFT)
					pressed_ = true;
			}
			void OnButtonUp(int button) {
				if ((button == STARSCAPE_BUTTON_LEFT) && IsPressed()) {
					pressed_ = false;
					ExecuteCommand();
				}
			}
			void OnMouseLeave(CursorBrushState& cursor) {
				Button::OnMouseLeave(cursor);
				pressed_ = false;
			}
		private:
			Command	command_;
		protected:
			void ExecuteCommand()	{ if (command_) command_(); }
		};
	}
}

#endif
