// logger.h
// Gregory Rosenblatt
// Update 5/11/05

#ifndef Starscape_Log_Logger_H_
#define Starscape_Log_Logger_H_

#include "pooleddeque.h"
#include "pooledlist.h"
#include "pooledstringstream.h"
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <ostream>
#include <algorithm>
//#include <iostream>	// debug

namespace Starscape {

	namespace Log {

		/** Stores each line sent to a logger for future reference.
		 * Provides an interface for retrieving a string by its line number.
		 */
		class History {
			typedef PooledDeque<PooledString>::Type	StringArray;
			StringArray		history_;
			unsigned int	maxLength_;
		public:
			enum { DEFAULT_MAXLENGTH=256 };
			History(unsigned int maxLength = DEFAULT_MAXLENGTH)
				: maxLength_(maxLength)	{}
			/** Gets the string at the given line number.
			 * @param line line number in the history buffer
			 * @return string found at that line number
			 */
			const char* GetString(unsigned int line) const {
				if (line >= Length())
					return "";
				return history_[line].c_str();
			}
			/** The number of lines contained in the history buffer. */
			unsigned int Length() const	{ return history_.size(); }
			typedef StringArray::const_iterator	Iterator;
			void AddString(const char* str) {
				history_.push_back(str);
				int overflow = Length() - maxLength_;
				for (; overflow > 0; --overflow)
					history_.pop_front();
			}
			void Clear()	{ history_.clear(); }
			Iterator Begin() const {
				return history_.begin();
			}
			Iterator End() const {
				return history_.end();
			}
		};

		/** A base class for channels that listen for logger updates. */
		class Channel {
		public:
			virtual ~Channel()	{}
			virtual void Output(const char* str)	= 0;
		};

		/** A channel that provides direct output to a stream */
		class StreamChannel : public Channel {
		public:
			StreamChannel(std::ostream& outputStream) : out_(outputStream)	{}
			void Output(const char* str)	{ out_ << str << std::flush; }
		private:
			std::ostream&	out_;
		};

		/** Records and broadcasts text-input. */
		class Logger {
			typedef boost::shared_ptr<Channel>	ChannelPtr;
			typedef PooledList<ChannelPtr>::Type	ChannelList;
			friend struct ChannelInfo;
			struct ChannelInfo {
				ChannelInfo(Logger& l, ChannelList::iterator itr)
					: logger(l), channel(itr)	{}
				~ChannelInfo()	{ logger.channelList_.erase(channel); }
				Logger&					logger;
				ChannelList::iterator	channel;
			};
			PooledOutputStringStream	ss_;
			History						logHistory_;
			ChannelList					channelList_;
		public:
			/** A self-managing handle to a Channel. */
			typedef boost::shared_ptr<ChannelInfo>	ChannelHandle;
			/** Adds an output channel to the logger's broadcast list.
			 * @param channel The Channel on which to broadcast messages.
			 * @param writeHistory Whether to write back logs to the channel?
			 * @return A handle to the output channel that MUST be stored.
			 */
			template <class ChannelType>
			ChannelHandle AddChannel(const ChannelType& channel
									,bool writeHistory=false
									,unsigned int backlogLength = 0) {
				ChannelPtr ptr(new ChannelType(channel));
				return AddChannelPtr(ptr, writeHistory, backlogLength);
			}
			/** Empties the logging history buffer. */
			void ClearHistory()	{ logHistory_.Clear(); }
			/** Inserts data into the pending data buffer. */
			template <typename ArgType>
			void Insert(const ArgType& arg) {
				ss_ << arg;
			}
			/** Sends all pending data to the history and channels. */
			void Flush() {
				OutputString(ss_.str().c_str());
				ss_.str("");
			}
		private:
			// Allows for the direct inclusion of a raw channel pointer.
			ChannelHandle AddChannelPtr(const ChannelPtr& ptr
										,bool writeHistory
										,unsigned int backlogLength) {
				ChannelList::iterator itr
					= channelList_.insert(channelList_.end(), ptr);
				if (writeHistory)
					WriteHistoryToChannel(*ptr, backlogLength);
				return ChannelHandle(new ChannelInfo(*this, itr));
			}
			// writes all data currently in the history buffer to a channel
			void WriteHistoryToChannel(Channel& channel
									,unsigned int numLines) const {
				History::Iterator itr = logHistory_.Begin()
								,end = logHistory_.End();
				if (numLines != 0) {
					int offset = logHistory_.Length()-numLines;
					if (offset > 0)
						itr += offset;
				}
				for (; itr != end; ++itr)
					channel.Output(itr->c_str());
			}
			// records a string in the history buffer, and broadcasts it
			void OutputString(const char* str) {
				logHistory_.AddString(str);
				BroadcastString(str);
			}
			// broadcasts a string across all listed channels
			void BroadcastString(const char* str) {
				std::for_each(channelList_.begin(), channelList_.end()
					,boost::bind(&Channel::Output, _1, str));
			}
		};

		/** Gives ostream-like semantics to the Logger. */
		template <typename ArgType>
		inline Logger& operator<<(Logger& logger, const ArgType& arg) {
			logger.Insert(arg);
			return logger;
		}
	}
}

#endif
