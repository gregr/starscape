// editline.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_EditLine_H_
#define Starscape_Gui_EditLine_H_

#include "gui/control/edit/editbox.h"
#include "gui/command.h"
#include "gui/config/inputconstants.h"
#include "getfirstline.h"

namespace Starscape {

	namespace Gui {

		/** Collection of commands that may be executed by an EditLine. */
		class EditLineController {
		public:
			EditLineController(const Command& enterCmd = Command()
							,const Command& upCmd = Command()
							,const Command& downCmd = Command())
				: enter(enterCmd), up(upCmd), down(downCmd)	{}
			void OnEnter() const	{ if (enter) enter(); }
			void OnUp() const	{ if (up) up(); }
			void OnDown() const	{ if (down) down(); }
		private:
			Command	enter, up, down;
		};

		/** An interface control used for entering a line of text.
		 * An optional command may also be provided which will execute
		 * upon receiving an event for a press of the enter key.
		 */
		class EditLine : public EditBox {
		public:
			EditLine(const EditBoxBrush& brush, const char* text=""
					,bool readOnly = false
					,const EditLineController& controller = EditLineController()
					,unsigned int numCharsPerLine
								= EditBox::DEFAULT_CHARS_PER_LINE)
				: EditBox(brush, GetFirstLine(text).c_str()
						,readOnly, numCharsPerLine, 1)
				,controller_(controller)	{}
			/** Sets the text content. */
			void SetText(const char* str) {
				EditBox::SetText(GetFirstLine(str).c_str());
			}
			/** Appends the given string to the end of the document. */
			void AppendString(const char* str) {
				EditBox::AppendString(GetFirstLine(str).c_str());
			}
			/** Special handling of the enter or return key. */
			void OnKeyDown(const Input::KeyInfo& key) {
				switch (key.sym) {
				case STARSCAPE_KEY_UP:
					controller_.OnUp();
					break;
				case STARSCAPE_KEY_DOWN:
					controller_.OnDown();
					break;
				case STARSCAPE_KEY_RETURN:
				case STARSCAPE_KEY_KP_ENTER:
					controller_.OnEnter();
					break;
				default:
					EditBox::OnKeyDown(key);
				}
			}
		protected:
			void Paste(const PooledString& str) {
				EditBox::Paste(GetFirstLine(str));
			}
			EditLineController	controller_;
		};
	}
}

#endif
