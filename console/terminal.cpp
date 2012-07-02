// terminal.cpp
// Gregory Rosenblatt
// 5/14/05

#include "console/terminal.h"

namespace Starscape {

	namespace Console {

		void TerminalChannel::Output(const char* str) {
			terminal_.OutputString(str);
		}

		void TerminalInputHistory::AddString(const char* str) {
			history_.AddString(str);
			choice_ = history_.End();
		}

		const char* TerminalInputHistory::GetPreviousString(
												const char* current) {
			if (choice_ == history_.End())
				currentInput_ = current;
			if (choice_ != history_.Begin())
				--choice_;
			if (choice_ == history_.End())
				return currentInput_.c_str();
			return choice_->c_str();
		}

		const char* TerminalInputHistory::GetNextString(
												const char* current) {
			if (choice_ == history_.End())
				currentInput_ = current;
			if (choice_ != history_.End())
				++choice_;
			if (choice_ == history_.End())
				return currentInput_.c_str();
			return choice_->c_str();
		}

		void Terminal::OutputString(const char* str) {
			Gui::EditBox& textOut = textOutView_.GetChild();
			int diff = textOut.OffsetYTotal()-textOut.OffsetY();
			bool watchingBottom = (diff <= 0);
			textOut.AppendString(str);
			if (diff < 0)
				watchingBottom = (textOut.OffsetYTotal() > 0);
			int linesToErase = textOut.GetNumLines() - backlogLength_;
			for (; linesToErase > 0; --linesToErase)
				textOut.EraseTopLine();
			if (watchingBottom)
				textOut.SetOffsetY(textOut.OffsetYTotal());
		}

		void Terminal::EnterString() {
			ExecuteString(textEntry_.GetText().c_str());
			inputHistory_.AddString(textEntry_.GetText().c_str());
			textEntry_.SetText("");
//			textEntry_.SetOffsetX(0);
		}

		void Terminal::GetPrevInput() {
			textEntry_.SetText(inputHistory_.GetPreviousString(
										textEntry_.GetText().c_str()));
			textEntry_.MoveCaretToEnd();
		}

		void Terminal::GetNextInput() {
			textEntry_.SetText(inputHistory_.GetNextString(
										textEntry_.GetText().c_str()));
			textEntry_.MoveCaretToEnd();
		}

		void Terminal::OnResize() {
			textOutView_.SetSize(Width()
							,Height()-(textEntry_.Height()+separatorSize_));
			textOutView_.OnResize();
			textEntry_.SetSize(Width(), textEntry_.Height());
			textEntry_.SetPos(0, textOutView_.Height()+separatorSize_);
		}
	}
}
