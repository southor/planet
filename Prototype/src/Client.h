#ifndef __client_h__
#define __client_h__

#include "common.h"
#include "Link.h"
#include "clientworldmodel.h"
#include "ClientPlayer.h"
#include "worldrenderer.h"
#include "keyhandler.h"

namespace Prototype
{
	class Client
	{
	public:

		static const int MAX_N_PLAYERS = 2;

		Client();

		void handleEvents();
		void logic();
		void render();
		
		//TODO not the correct way for players to be added
		void addPlayer(const Color &playerColor, const Pos &playerPos);

		void setConnection(MessageSender *messageSender, MessageReciever *messageReciever);
		KeyHandler* getKeyHandler();
		
		void setPlayerId(size_t playerId) { this->playerId = playerId; }

	private:
		//MessageSender *messageSender;
		//MessageReciever *messageReciever;
		Link link;

		KeyHandler kh;

		ClientWorldModel worldModel;
		WorldRenderer worldRenderer;
		ClientPlayers players;

		size_t playerId;
	};
};

#endif
