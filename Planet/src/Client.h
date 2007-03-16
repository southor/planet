#ifndef __Client_h__
#define __Client_h__

#include "common.h"
#include "Link.h"
#include "UserInputHandler.h"
#include "PlanetBody.h"
#include "Ship.h"
#include "Camera.h"
#include "Sight.h"
#include "ClientTimeHandler.h"

namespace Planet
{
	namespace ClientPhase
	{
		enum
		{
			SEND_INITCLIENT = 0,
			WAIT_WELCOME_CLIENT,
			SYNC_SEND_PING,
			SYNC_GET_PONG,
			GET_ADDPLAYEROBJ,
			GET_ADDOBSTACLE,
			RUNNING
		};
	};

	class Client
	{
	public:
		static const int MAX_N_PLAYERS = 2;

		Client();
		~Client();
		
		void init();


		void runStep();

		bool initConnection();
		void renderAndUpdate();

		void setConnection(MessageSender *messageSender, MessageReciever *messageReciever);
		UserInputHandler* getUserInputHandler()	{ return &userInputHandler; }
		Camera* getCamera() { return &camera; }	

		void setPlayerId(PlayerId playerId)				{ this->playerId = playerId; }
		void setColor(Color color)						{ this->color = color; }

		Link link;
		size_t connectionPhase;
		ClientTimeHandler timeHandler;

		UserInputHandler userInputHandler;



		PlayerId playerId;
		Color color;	// should this exist or should the client have an Player object before we connect to server?

	
		bool requestRender;


		PlanetBody planetBody;
		Ship ship;
		Camera camera;
		Sight sight;
	};
};

#endif

