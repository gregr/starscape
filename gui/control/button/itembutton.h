// itembutton.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_Gui_ItemButton_H_
#define Starscape_Gui_ItemButton_H_

#include "gui/control/button/button.h"
#include "gui/command.h"
#include "gui/config/inputconstants.h"

namespace Starscape {

	namespace Gui {

		class ItemButton : public Button {
		public:
			ItemButton(const ButtonBrush& brush, const Command& command
					,const char* caption = "")
				: Button(brush, caption), command_(command)	{}
			void OnButtonDown(int button) {
				if (button == STARSCAPE_BUTTON_LEFT) {
					pressed_ = true;
					ExecuteCommand();
				}
			}
			void Select()	{ pressed_ = true; }
			void Deselect()	{ pressed_ = false; }
			const char* GetName() const	{ return GetCaption().c_str(); }
			bool operator<(const ItemButton& b) const {
				return (GetCaption() < b.GetCaption());
			}
		private:
			Command	command_;
		protected:
			void ExecuteCommand()	{ if (command_) command_(); }
		};
	}
}

#endif
