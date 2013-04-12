// terminal.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Console_Terminal_H_
#define Starscape_Console_Terminal_H_

#include "gui/control/edit/editline.h"
#include "gui/control/scroll/scrollfixture.h"
#include "log/logger.h"
#include "pooledstringstream.h"

namespace Starscape {

	namespace Console {

		class Terminal;

		class TerminalChannel : public Log::Channel {
		public:
			TerminalChannel(Terminal& terminal) : terminal_(terminal)	{}
			void Output(const char* str);
		private:
			Terminal&	terminal_;
		};

		class TerminalInputHistory {
		public:
			TerminalInputHistory(unsigned int numEntries
								= Log::History::DEFAULT_MAXLENGTH)
				: history_(numEntries), choice_(history_.End())	{}
			void AddString(const char* str);
			const char* GetPreviousString(const char* current);
			const char* GetNextString(const char* current);
		private:
			Log::History			history_;
			PooledString			currentInput_;
			Log::History::Iterator	choice_;
		};

		class Terminal : public Gui::Window {
		public:
			enum {
				DEFAULT_BACKLOG_LENGTH = 64
				,DEFAULT_HISTORY_LENGTH = 16
				,DEFAULT_SEPARATOR_SIZE = 10
			};
			Terminal(const Gui::ScrollBarBrush& vScrollBrush
					,const Gui::ScrollBarBrush& hScrollBrush
					,const Gui::EditBoxBrush& editBrush
					,Log::Logger& logger
					,unsigned int backlogLength = DEFAULT_BACKLOG_LENGTH
					,unsigned int inputHistoryLength = DEFAULT_HISTORY_LENGTH
					,const SizeScalar& separatorSize = DEFAULT_SEPARATOR_SIZE)
				: textOutView_(Gui::EditBox(editBrush, "", true)
								,vScrollBrush, hScrollBrush)
				,textEntry_(editBrush, "", false
						,Gui::EditLineController(
							boost::bind(&Terminal::EnterString, this)
							,boost::bind(&Terminal::GetPrevInput, this)
							,boost::bind(&Terminal::GetNextInput, this)))
				,inputHistory_(inputHistoryLength)
				,backlogLength_(backlogLength)
				,channel_(logger.AddChannel(TerminalChannel(*this)
											,true, backlogLength_))
				,separatorSize_(separatorSize)
			{
				AddChild(textOutView_);
				textOutView_.SetPos(0, 0);
				AddChild(textEntry_);
				textEntry_.SetPos(0, textOutView_.Height()+separatorSize_);
				SetSize(textOutView_.Width()
								,textOutView_.Height()
								+textEntry_.Height()+separatorSize_);
				textEntry_.SetSize(Width(), textEntry_.Height());
			}
			virtual ~Terminal()	{}
			void OutputString(const char* str);
			void EnterString();
			void GetPrevInput();
			void GetNextInput();
			void OnResize();
			void EnableTextWrapping() {
				textOutView_.GetChild().EnableTextWrapping();
			}
			void DisableTextWrapping() {
				textOutView_.GetChild().DisableTextWrapping();
			}
		private:
			virtual void ExecuteString(const char* str) = 0;
			void OnRender()	{}
			Gui::ScrollFixture<Gui::EditBox>	textOutView_;
			Gui::EditLine						textEntry_;
			TerminalInputHistory				inputHistory_;
			unsigned int						backlogLength_;
			Log::Logger::ChannelHandle			channel_;
			SizeScalar							separatorSize_;
		};
	}
}

#endif
