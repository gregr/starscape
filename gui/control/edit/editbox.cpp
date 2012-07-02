// editbox.cpp
// Gregory Rosenblatt
// 5/16/05

#include "gui/control/edit/editbox.h"
#include "gui/activestate.h"
#include "gui/cursorbrushstate.h"
#include "gui/clipboardstring.h"
#include "gui/config/inputconstants.h"
//#include <iostream>

namespace Starscape {

	namespace Gui {

		const Window* EditBox::OnMouseGrab(int button) {
			if (button == STARSCAPE_BUTTON_LEFT)
				return this;
			return 0;
		}
		
		bool EditBox::OnMouseRelease(int button) {
			if (button == STARSCAPE_BUTTON_LEFT)
				return true;
			return false;
		}

		void EditBox::OnMouseOver(CursorBrushState& cursor) {
			cursor.SetBrush(brush_.GetCursorBrush());
		}

		void EditBox::OnMouseLeave(CursorBrushState& cursor) {
			cursor.ResetBrush();
		}

		void EditBox::OnKeyboardGrab(ActiveState& state) {
			state.SetKeyboardFocus(*this);
		}
		
		void EditBox::OnKeyboardRelease() {
			Input::DisableKeyRepeat();
			caretPtr_ = 0;
			isSelecting_ = false;
		}
		
		void EditBox::OnKeyDown(const Input::KeyInfo& key) {
			bool acceptKeyPress = true;
			if (Input::IsCtrlPressed()) {
				if (key.sym == 'c') {
					Copy();
					acceptKeyPress = false;
				}
				else if (key.sym == 'x') {
					Cut();
					acceptKeyPress = false;
				}
				else if (key.sym == 'v') {
					Paste(GetClipboardString());
					acceptKeyPress = false;
				}
			}
			if (acceptKeyPress)
				HandleKeyPress(key);
			AlertParentOfStateChange();
		}
		
		void EditBox::OnButtonDown(int button) {
			Input::EnableKeyRepeat();
			caretPtr_ = &caret_;
			if (button == STARSCAPE_BUTTON_LEFT) {
				isSelecting_ = true;
				MoveCaretToMousePos();
				if (!Input::IsShiftPressed())
					caret_.MarkCurrent();
			}
		}
		
		void EditBox::OnButtonUp(int button) {
			if (button == STARSCAPE_BUTTON_LEFT)
				isSelecting_ = false;
		}
		
		void EditBox::OnMouseMove(const Coord& delta) {
			if (isSelecting_) {
				MoveCaretToMousePos();
				if ((CursorX() == static_cast<CoordScalar>(Width()-1))
					&& ((content_.GetText()[caret_.GetPosition()] != '\n')
						&& (caret_.GetPosition()
							< GetDocumentLength(content_)))) {
					MoveCaretAdvance(content_, caret_);
					int offset = brush_.GetDocumentBrush().PosToCoord(content_
									,caret_.GetPosition()).x
									+brush_.GetDocumentBrush().GetCaretWidth()
									-Width();
					if (offset > OffsetX())
						SetOffsetX(offset);
				}
				else if ((CursorX() == 0) && (OffsetX() > 0)) {
					if (caret_.GetPosition() > 0)
						MoveCaretRetreat(content_, caret_);
					SetOffsetX(brush_.GetDocumentBrush().PosToCoord(content_
								,caret_.GetPosition()).x);
				}
				if ((CursorY() == static_cast<CoordScalar>(Height()-1))
					&& (static_cast<CoordScalar>(GetDocumentNumLines(content_)
						* brush_.GetDocumentBrush().GetLineHeight())
						> (Height()+OffsetY()))) {
					SetOffsetY((GetDocumentLineAtPos(content_, caret_)+1)
							* brush_.GetDocumentBrush().GetLineHeight()
							+ brush_.GetDocumentBrush().GetCharHeight()
							- Height());
					MoveCaretToMousePos();
				}
				else if ((CursorY() == 0) && (OffsetY() > 0)) {
					SetOffsetY((GetDocumentLineAtPos(content_, caret_)-1)
							* brush_.GetDocumentBrush().GetLineHeight());
					MoveCaretToMousePos();
				}
				AlertParentOfStateChange();
			}
		}

		void EditBox::OnRender() {
			brush_.Draw(GetRenderingRect(), content_, caretPtr_
					,GetRenderingRect().x-OffsetX()
					,GetRenderingRect().y-OffsetY()
					,(!HasFocus() || isReadOnly_));
		}

		void EditBox::HandleKeyPress(const Input::KeyInfo& key) {
//			std::cout << "handle keypress" << std::endl;
			switch (key.sym) {
			case STARSCAPE_KEY_DELETE:
				if (isReadOnly_)
					return;
				if (CaretHasSelection(caret_))
					content_.ReplaceSelection(caret_, "", 0);
				else if (caret_.GetPosition() < GetDocumentLength(content_))
					content_.RemoveCharAtPos(caret_, caret_.GetPosition());
				break;
			case STARSCAPE_KEY_BACKSPACE:
				if (isReadOnly_)
					return;
				if (CaretHasSelection(caret_))
					content_.ReplaceSelection(caret_, "", 0);
				else if (caret_.GetPosition() > 0)
					content_.RemoveCharAtPos(caret_, caret_.GetPosition()-1);
				break;
			case STARSCAPE_KEY_HOME:
				MoveCaretToStartOfLine(content_, caret_);
				if (!Input::IsShiftPressed())
					caret_.MarkCurrent();
				break;
			case STARSCAPE_KEY_END:
				MoveCaretToEndOfLine(content_, caret_);
				if (!Input::IsShiftPressed())
					caret_.MarkCurrent();
				break;
			case STARSCAPE_KEY_LEFT:
				if (caret_.GetPosition() > 0) {
					if (Input::IsCtrlPressed())
						MoveCaretToPreviousWord(content_, caret_);
					else
						MoveCaretRetreat(content_, caret_);
				}
				if (!Input::IsShiftPressed())
					caret_.MarkCurrent();
				break;
			case STARSCAPE_KEY_RIGHT:
				if (caret_.GetPosition() < GetDocumentLength(content_)) {
					if (Input::IsCtrlPressed())
						MoveCaretToNextWord(content_, caret_);
					else
						MoveCaretAdvance(content_, caret_);
				}
				if (!Input::IsShiftPressed())
					caret_.MarkCurrent();
				break;
			case STARSCAPE_KEY_UP:
				MoveCaretUp(content_, caret_);
				if (!Input::IsShiftPressed())
					caret_.MarkCurrent();
				break;
			case STARSCAPE_KEY_DOWN:
				MoveCaretDown(content_, caret_);
				if (!Input::IsShiftPressed())
					caret_.MarkCurrent();
				break;
			case STARSCAPE_KEY_TAB:
				if (isReadOnly_)
					return;
				content_.ReplaceSelection(caret_, "\t", 1);
				break;
			case STARSCAPE_KEY_RETURN:
			case STARSCAPE_KEY_KP_ENTER:
				if (isReadOnly_)
					return;
				content_.ReplaceSelection(caret_, "\n", 1);
				break;
			default:
				if (isReadOnly_)
					return;
				if (Input::IsUnicodeAscii(key)) {
					char ch = Input::GetUnicode(key);
					content_.ReplaceSelection(caret_, &ch, 1);
				}
				else return;
			}
			ScrollToCaret();
		}

		void EditBox::Copy() {
			if (CaretHasSelection(caret_))
				SetClipboardString(GetSelectedSubString(content_, caret_));
		}

		void EditBox::Cut() {
			Copy();
			if (isReadOnly_)
				return;
			content_.ReplaceSelection(caret_, "", 0);
			ScrollToCaret();
		}

		void EditBox::Paste(const PooledString& str) {
			if (isReadOnly_)
				return;
			content_.ReplaceSelection(caret_, str.c_str(), str.length());
			ScrollToCaret();
		}
	}
}
