#ifndef __networkconnection_h__
#define __networkconnection_h__

#include <deque>

#include "common.h"
#include "messages.h"

namespace Prototype
{
	class NetworkConnection
	{
	public:
		static const int port = 12333;
	
		// CLIENT
		void openClientConnection();
		void closeClientConnection();
		
		
		// SERVER
		void startServer();
		// returns true if client got connected
		bool waitForClient();
		void closeServer();

	private:
		// CLIENT
		TCPsocket clientSocket;
		
		// SERVER
		TCPsocket serverClientSocket;
		TCPsocket serverSocket;
	};

	// -------------------
	// NOT YET IMPLEMENTED
	// -------------------
	class NetworkMessageSender : public MessageSender
	{
	public:
		NetworkMessageSender() : MessageSender() { }
		~NetworkMessageSender()					{}

		void pushMessage(const Message &message);

		// Get number of messages queued to be transmitted.
		int getNMessages();
		
		// Transmits queued messages.
		void transmit();
				
	private:
		std::deque<Message> sendDeque;
	};

	class NetworkMessageReciever : public MessageReciever
	{
	public:
		NetworkMessageReciever() : MessageReciever(), lag(50) { }
		~NetworkMessageReciever() {}

		// Get number of messages recieved.
		int getNMessages();		
		
		Message popMessage();

		void setConnectionLag(int lag);

	private:
		std::deque<Message> recieveDeque;
		
		int lag;
	};
}

#endif