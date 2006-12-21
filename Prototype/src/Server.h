#ifndef __server_h__
#define __server_h__

#include <vector>

#include "messages.h"
#include "ServerPlayer.h"
#include "serverworldmodel.h"

namespace Prototype
{

	class Server
	{
	public:
		Server() { }

		void logic();

		// @return The PlayerId that the new player got
		size_t addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever);

		// @return The PlayerObjId that the new player object got
		size_t Server::addPlayerObj(size_t playerId, const Pos &playerPos);

	private:
		
		
		//size_t addPlayer(const ServerPlayer &player);



		ServerWorldModel worldModel;
		ServerPlayers players;

		//std::vector<ServerClient> clients;

	};
};

#endif