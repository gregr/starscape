// log.h
// Gregory Rosenblatt
// 4/24/05

#ifndef Starscape_Log_Log_H_
#define Starscape_Log_Log_H_

#include "log/logger.h"

namespace Starscape {

	namespace Log {

		/** Retrieves the primary logger. */
		Logger& Get();

		/** Logs the given message. */
		template <typename MessageType>
		void Message(const MessageType& msg) {
			Get().Insert(msg);
			Get().Flush();
		}
	}
}

#endif
