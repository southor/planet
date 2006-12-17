#ifndef __client_h__
#define __client_h__

#include "common.h"
#include "messages.h"
#include "worldmodel.h"
#include "Player.h"
#include "worldrenderer.h"

namespace Prototype
{
	class Client
	{
	public:

		static const int MAX_N_PLAYERS = 2;

		Client();

		void logic();
		void render();
		
		void sendMessage(Message message);
		void recieveMessages();

		//TODO not the correct way for players to be added
		void addPlayer(const Color &playerColor, const Pos &playerPos);

		void setConnection(MessageSender *messageSender, MessageReciever *messageReciever);

	private:
		MessageSender *messageSender;
		MessageReciever *messageReciever;

		WorldModel worldModel;
		WorldRenderer worldRenderer;
		Players players;
	};
};

#endif
