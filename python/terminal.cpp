// terminal.cpp
// Gregory Rosenblatt
// 5/11/05

#include "terminal.h"

namespace Starscape {

	namespace Python {

		void Terminal::ExecuteString(const char* str) {
			if (!isSecondaryInput_) {
				PrintString(PROMPT_PRIMARY);
				PrintString((PooledString(str)+"\n").c_str());
				if (CheckPrimaryInput(PooledString(str))) {
					executeBuffer_ << str;
					isSecondaryInput_ = true;
				}
				else
					ExecuteInteractiveString(str);
			}
			else {
				PrintString(PROMPT_SECONDARY);
				PrintString((PooledString(str)+"\n").c_str());
				if (CheckSecondaryInput(PooledString(str)))
					executeBuffer_ << '\n' << str;
				else {
					ExecuteInteractiveMultilineString(
										executeBuffer_.str().c_str());
					executeBuffer_.str("");
					isSecondaryInput_ = false;
				}
			}
		}
	}
}
