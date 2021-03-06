#ifndef __Server_h__
#define __Server_h__

#include <vector>
#include <string>

//#include "messages.h"
#include "ServerPlayers.h"
#include "ServerPlanet.h"
#include "ServerTimeHandler.h"
#include "IdGenerator.h"
#include "StateCmds.h"
#include "Cmds.h"
#include "MessageSender.h"
#include "MessageReciever.h"
#include "Link.h"
#include "ServerGlobalAccess.h"

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

	class Server : public ServerGlobalAccess
	{
	public:
		Server();
		~Server();
		
		void init();

		void run(bool &runningServer, int numberOfClients, std::string map);

		bool clientConnected(MessageSender *messageSender, MessageReciever *messageReciever);

		void setMap(const std::string &map) { currentMap = map; }

	private:
		static const double PREDICTION_AMOUNT_MODIFIER;
		static const int PREDICTION_AMOUNT_ADD_TIME;
		
		// @return The PlayerId that the new player got
		PlayerId addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever);

		void addPlayerObj(PlayerId playerId, const Pos &playerPos);

		void startGame();

		void logic();

		ServerPlayers players;
		ServerGlobalObj serverGlobalObj;
		ServerPlanet planet;
		int lastUpdateTime;

		std::string currentMap;

		//debug
		void debugPrintState();
	};
};

#endif
