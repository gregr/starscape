// chatclient.h
// Gregory Rosenblatt
// 5/5/05

#ifndef Starscape_Net_ChatClient_H_
#define Starscape_Net_ChatClient_H_

#include "log/logger.h"
#include "pooledstring.h"
//#include "RakClientInterface.h"
//#include "RakNetworkFactory.h"
//#include "RakServerInterface.h"
#include "PacketEnumerations.h"
#include "BitStream.h"
#include "StringCompressor.h"
#include "Multiplayer.h"

namespace Starscape {

	namespace Net {

		const int SERVER_PORT = 60000;
		const int MAX_STRLEN = 256;
		enum { ID_CHAT_MESSAGE = (ID_RESERVED9+1) };

		inline PooledString ReadMessage(RakNet::BitStream& bs) {
			char buffer[MAX_STRLEN];
			unsigned char typeId;
			bs.Read(typeId);
			StringCompressor::Instance()->DecodeString(buffer, MAX_STRLEN, &bs);
			return PooledString(buffer);
		}

		class ChatClient : public Multiplayer<RakClientInterface> {
		public:
			ChatClient(Log::Logger& logger)
				: iface_(RakNetworkFactory::GetRakClientInterface())
				,logger_(logger)
			{ assert(iface_); }
			~ChatClient() {
				RakNetworkFactory::DestroyRakClientInterface(iface_);
			}
			void Connect(const char* ip, unsigned int port = SERVER_PORT) {
				if (iface_->Connect((char*)ip, port, 0, 0, 0))
					OutputMessage("Client started:\n");
				else
					OutputMessage("Failed to start client.\n");
			}
			void Process()	{ ProcessPackets(iface_); }
			void ProcessUnhandledPacket(Packet* packet
										,unsigned char packetIdentifier
										,RakClientInterface* iface) {
				RakNet::BitStream bs((const char*)packet->data
									,packet->length, false);
				PooledString message = ReadMessage(bs);
				OutputMessage(message.c_str());
			}
			void SendMessage(const char* msg) {
				RakNet::BitStream bs;
				unsigned char typeId = ID_CHAT_MESSAGE;
				bs.Write(typeId);
				StringCompressor::Instance()->EncodeString((char*)msg
															,MAX_STRLEN, &bs);
				iface_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
			}
			void OutputMessage(const char* msg) {
				logger_ << "<ff>chat:<reset> " << msg << "\n";
				logger_.Flush();
			}
		private:
			RakClientInterface*	iface_;
			Log::Logger&		logger_;
		};
	}
}

#endif
