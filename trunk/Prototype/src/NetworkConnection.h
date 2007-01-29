#ifndef __networkconnection_h__
#define __networkconnection_h__

#include <deque>
#include <vector>

#include "common.h"
#include "messages.h"

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
		NetworkMessageReciever() : MessageReciever()	{}
		~NetworkMessageReciever()						{}

		bool hasMessageOnQueue();
		
		Message popMessage();

		void setSocket(TCPsocket socket);

	private:
		void retrieve();
	
		TCPsocket socket;
		SDLNet_SocketSet set;
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
		
		bool openConnection();
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