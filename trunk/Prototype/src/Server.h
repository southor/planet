#ifndef __server_h__
#define __server_h__

#include <vector>

#include "messages.h"
#include "ServerPlayers.h"
#include "serverworldmodel.h"

namespace Prototype
{

	class Server
	{
	public:
		Server();

		void logic();

		bool clientConnected(MessageSender *messageSender, MessageReciever *messageReciever);

		// @return The PlayerId that the new player got
		size_t addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever);

		//// @return The PlayerObjId that the new player object got
		//size_t addPlayerObj(size_t playerId, const Pos &playerPos);

		void addPlayerObj(size_t playerId, const Pos &playerPos);


		// @return The ObstacleId that the new obstacle got		 
		//size_t addObstacle(const Rectangle &obstacleArea);

		// start game, no more clients can join
		void startGame();

	private:
		
		
		//size_t addPlayer(const ServerPlayer &player);



		ServerWorldModel worldModel;
		ServerPlayers players;

		//std::vector<ServerClient> clients;

	};
};

#endif