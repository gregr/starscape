// terminal.h
// Gregory Rosenblatt
// 5/11/05

#ifndef Starscape_Python_Terminal_H_
#define Starscape_Python_Terminal_H_

#include "console/terminal.h"
#include "python/interpreter.h"
#include "pooledstringstream.h"

namespace Starscape {

	namespace Python {

		class Terminal : public Console::Terminal {
		public:
			Terminal(const Gui::ScrollBarBrush& vScrollBrush
					,const Gui::ScrollBarBrush& hScrollBrush
					,const Gui::EditBoxBrush& editBrush
					,Log::Logger& logger
					,unsigned int backlogLength
						= Terminal::DEFAULT_BACKLOG_LENGTH
					,unsigned int inputHistoryLength
						= Terminal::DEFAULT_HISTORY_LENGTH
					,const SizeScalar& separatorSize
						= Terminal::DEFAULT_SEPARATOR_SIZE)
				: Console::Terminal(vScrollBrush, hScrollBrush
									,editBrush, logger
									,backlogLength, inputHistoryLength
									,separatorSize)
				,isSecondaryInput_(false)
			{}
		private:
			void ExecuteString(const char* str);
			PooledOutputStringStream			executeBuffer_;
			Interpreter							pythonInterpreter_;
			bool								isSecondaryInput_;
		};
	}
}

#endif
