// terminal.cpp
// Gregory Rosenblatt
// 5/11/05

#include "net/terminal.h"
#include <fstream>

namespace Starscape {

	namespace Net {

		const char* const SERVER_CONNECTION_FILE	= "serverconnection.txt";

		Log::Logger outputLogger;

		Log::Logger& GetOutputLogger() {
			return outputLogger;
		}

		void Terminal::ConnectClient() {
			if (!connected_) {
				std::ifstream fin(SERVER_CONNECTION_FILE);
				assert(fin);
				PooledString ip;
				std::getline(fin, ip);
				//client_.Connect(ip.c_str());
			}
			connected_ = true;
		}

		void Terminal::ExecuteString(const char* str) {
			//client_.SendMessage(str);
			OutputString(((PooledString("<reset>")
						+str)+"\n").c_str());
		}
	}
}
