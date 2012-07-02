// log.cpp
// Gregory Rosenblatt
// 4/24/05

#include "log/logger.h"
#include <iostream>

namespace Starscape {

	namespace Log {

		// the primary logger automatically sends to std::cout
		class MainLogger : public Logger {
		public:
			MainLogger()
				: errChannel_(AddChannel(StreamChannel(std::cout))) {}
		private:
			Logger::ChannelHandle	errChannel_;
		};

		MainLogger logger;	// the primary logger

		Logger& Get()	{ return logger; }	// public interface
	}
}
