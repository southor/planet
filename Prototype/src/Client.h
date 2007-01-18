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
		bool initConnection();
		void render();
		
		//TODO not the correct way for players to be added
		void addPlayer(const Color &playerColor, const Pos &playerPos);

		void setConnection(MessageSender *messageSender, MessageReciever *messageReciever);
		KeyHandler* getKeyHandler();
		
		void setPlayerId(size_t playerId) { this->playerId = playerId; }
		void setColor(Color color) { this->color = color; }

	private:
		Link link;
		size_t connectionPhase;


		KeyHandler kh;

		ClientWorldModel worldModel;
		WorldRenderer worldRenderer;
		ClientPlayers players;

		size_t playerId;
		Color color;	// should this exist or should the client have an Player object before we connect to server?
	};
};

#endif
