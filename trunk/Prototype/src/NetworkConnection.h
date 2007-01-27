#ifndef __networkconnection_h__
#define __networkconnection_h__

#include <deque>
#include <vector>

#include "common.h"
#include "messages.h"

namespace Prototype
{
	typedef std::vector<TCPsocket> Clients;

	class NetworkConnection
	{
	public:
		static const int port = 12333;

		NetworkConnection() 
			: set(0), clientSocket1(0), clientSocket2(0), serverClientSocket(0), serverSocket(0) 
		{}

		// CLIENT
		void openClientConnection1();
		void openClientConnection2();
		void openClientConnection(TCPsocket socket);
		void closeClientConnection();
		
		
		// SERVER
		void startServer();
		void createSocketSet();
		
		// returns true if client got connected
		bool waitForClient();
		void closeServer();

	private:
		// CLIENT
		TCPsocket clientSocket1;
		TCPsocket clientSocket2;
		
		// SERVER
		TCPsocket serverClientSocket; // temp use, client sockets are stored in clients
		TCPsocket serverSocket;

		SDLNet_SocketSet set;

		Clients clients;
		size_t numberOfClients;
	};

	// -------------------
	// NOT YET IMPLEMENTED
	// -------------------
	class NetworkMessageSender : public MessageSender
	{
	public:
		NetworkMessageSender() : MessageSender()	{}
		~NetworkMessageSender()						{}

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
		NetworkMessageReciever() : MessageReciever()	{}
		~NetworkMessageReciever()						{}

		bool hasMessageOnQueue();
		
		Message popMessage();

		void setSimulatedLag(int lag);
	};
}

#endif