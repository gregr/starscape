// pushbutton.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_PushButton_H_
#define Starscape_Gui_PushButton_H_

#include "gui/control/button/commandbutton.h"

namespace Starscape {

	namespace Gui {

		/** Continues to execute its command while it is held down.
		 * Execution does not wait for release.
		 */
		class PushButton : public CommandButton {
		public:
			enum { REPEAT_INTERVAL = 30, REPEAT_DELAY = 500 };
			PushButton(const ButtonBrush& brush, const Command& command
					,const char* caption = "")
				: CommandButton(brush, command, caption)
				,timer_(0), repeating_(false)	{}
			// event handling
			void OnButtonDown(int button) {
				if (button == STARSCAPE_BUTTON_LEFT) {
					pressed_ = true;
					ExecuteCommand();
				}
			}
			void OnButtonUp(int button) {
				if ((button == STARSCAPE_BUTTON_LEFT) && IsPressed()) {
					pressed_ = false;
					repeating_ = false;
					timer_ = 0;
				}
			}
			void OnUpdate(unsigned int deltaTime) {
				if (IsPressed()) {
					timer_ += deltaTime;
					if (repeating_) {
						if (timer_ > REPEAT_INTERVAL) {
							ExecuteCommand();
							timer_ -= REPEAT_INTERVAL;
						}
					}
					else if (timer_ > REPEAT_DELAY) {
						repeating_ = true;
						timer_ -= REPEAT_DELAY;
					}
				}
			}
		private:
			unsigned int	timer_;
			bool			repeating_;
		};
	}
}

#endif
