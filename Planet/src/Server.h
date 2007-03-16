#ifndef __Server_h__
#define __Server_h__

#include <vector>

//#include "messages.h"
#include "ServerPlayers.h"
//#include "ServerPlanet.h"
#include "ServerTimeHandler.h"
#include "IdGenerator.h"
#include "StateCmds.h"
#include "Cmds.h"
#include "MessageSender.h"
#include "MessageReciever.h"
#include "Link.h"

namespace Planet
{
	namespace ServerPhase
	{
		enum
		{
			WAIT_INITCLIENT,
			WAIT_PING
		};
	};

	class Server //: public ServerGlobalAccess
	{
	public:
		Server();
		~Server();

		void run(bool &runningServer, int numberOfClients);

	bool clientConnected(MessageSender *messageSender, MessageReciever *messageReciever);

	private:
	// @return The PlayerId that the new player got
	PlayerId addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever);


	public:


		ServerTimeHandler* getTimeHandler() { return &timeHandler; }
		ServerIdGenerator* getIdGenerator() { return &idGenerator; }


		ServerPlayers players;


	

		ServerTimeHandler timeHandler;
		ServerIdGenerator idGenerator;
	};
};

#endif
