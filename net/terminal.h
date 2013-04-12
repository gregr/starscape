// terminal.h
// Gregory Rosenblatt
// 5/11/05

#ifndef Starscape_Net_ChatTerminal_H_
#define Starscape_Net_ChatTerminal_H_

#include "console/terminal.h"
//#include "net/chatclient.h"

namespace Starscape {

	namespace Net {

		Log::Logger& GetOutputLogger();

		class Terminal : public Console::Terminal {
		public:
			Terminal(const Gui::ScrollBarBrush& vScrollBrush
					,const Gui::ScrollBarBrush& hScrollBrush
					,const Gui::EditBoxBrush& editBrush
					,Log::Logger& logger
					,unsigned int backlogLength = DEFAULT_BACKLOG_LENGTH
					,unsigned int inputHistoryLength = DEFAULT_HISTORY_LENGTH
					,const SizeScalar& separatorSize = DEFAULT_SEPARATOR_SIZE)
				: Console::Terminal(vScrollBrush, hScrollBrush, editBrush
								,logger, backlogLength, inputHistoryLength
								,separatorSize)
				//,client_(logger),
        ,connected_(false)
			{ ConnectClient(); }
			void ConnectClient();
			void OnUpdate(unsigned int deltaTime)	{ }//client_.Process(); }
		private:
			void ExecuteString(const char* str);
			//ChatClient							client_;
			bool								connected_;
		};
	}
}

#endif
