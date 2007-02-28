#ifndef __networkconnection_h__
#define __networkconnection_h__

#include <deque>
#include <vector>

#include "common.h"
#include "MessageSender.h"
#include "MessageReciever.h"

namespace Prototype
{
	class NetworkMessageSender : public MessageSender
	{
	public:
		NetworkMessageSender() : MessageSender()	{}
		~NetworkMessageSender()						{}

		void pushMessage(const Message &message);

		// Get number of messages queued to be transmitted.
		int getNMessages();
		
		// Transmits queued messages.
		void transmit() ;
		
		void setSocket(TCPsocket socket) { this->socket = socket; }
		
	private:
		std::deque<Message> sendDeque;
		
		TCPsocket socket;
	};

	class NetworkMessageReciever : public MessageReciever
	{
	public:
		NetworkMessageReciever() 
			: MessageReciever(), retrieveMessagePhase(0), retrieveSize(0), retrieveTime(0), retrieveTick(0), retrieveType(0)	{}
		~NetworkMessageReciever()																			{}

		//Message popMessage();

		void setSocket(TCPsocket socket);

		void retrieve(int currentTime);
		
		//bool hasMessageOnQueue() { retrieve(0); return MessageReciever::hasMessageOnQueue(); }


	private:
		int readData(void *data, int len);
	
		TCPsocket socket;
		SDLNet_SocketSet set;
		
		unsigned char retrieveMessagePhase;
		int retrieveType;
		int retrieveTime;
		int retrieveTick;
		int retrieveSize;
	};




	struct NetworkServerClient
	{
		TCPsocket socket;
	
		NetworkMessageSender sender;
		NetworkMessageReciever reciever;
	};

	typedef std::vector<NetworkServerClient*> Clients;

	class NetworkServer
	{
	public:
		NetworkServer() : socket(0), set(0), numberOfClients(0)	{}
		
		void start();
		NetworkServerClient* checkForNewClient();
		void close();
		
	private:
		void createSocketSet();
	
		TCPsocket socket;
		SDLNet_SocketSet set;
		Clients clients;
		size_t numberOfClients;
	};

	class NetworkClient
	{
	public:
		NetworkClient() : socket(0)	{}
		
		bool openConnection(std::string &host);
		void close();

		MessageSender* getMessageSender()		{ return &sender; }
		MessageReciever* getMessageReciever()	{ return &reciever; }

	private:
		TCPsocket socket;
		
		NetworkMessageSender sender;
		NetworkMessageReciever reciever;
	};
}

#endif
