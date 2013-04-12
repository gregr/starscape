// output.cpp
// Gregory Rosenblatt
// 4/24/05

//#include <iostream>
#include "log/logger.h"

namespace Starscape {

	namespace Python {

		Log::Logger outputLogger;

		Log::Logger& GetOutputLogger() {
			return outputLogger;
		}

		void PrintString(const char* str) {
			outputLogger.Insert(str);
			outputLogger.Flush();
		}
	}
}
