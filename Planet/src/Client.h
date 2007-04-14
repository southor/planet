#ifndef __Client_h__
#define __Client_h__

#include "common.h"
#include "Link.h"
#include "UserInputHandler.h"
#include "ClientPlanet.h"
#include "Ship.h"
#include "Camera.h"
#include "Sight.h"
#include "ClientTimeHandler.h"
#include "ClientGlobalAccess.h"
#include "PlanetRenderer.h"
#include "ClientPlayer.h"
#include "PredictionHandler.h"

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
			//GET_ADDOBSTACLE,
			RUNNING
		};
	};

	class Client : public ClientGlobalAccess
	{
	public:

		static const bool DEBUG_SHOOTING = true;

		Client();

		~Client();
		void init();

			
		void getCurrentUserCmd(UserCmd &userCmd);
		void handleServerMessages();
		void runStep();

		bool initConnection();
		void renderAndUpdate();
		
		
		void addPlayer(PlayerId playerId, const Color &playerColor, const Pos &playerPos, const Pos &playerAimPos, int tick);

		void setConnection(MessageSender *messageSender, MessageReciever *messageReciever);

		Camera* getCamera() { return &camera; }	
		inline UserInputHandler* getUserInputHandler()	{ return &userInputHandler; }
		inline bool getRequestRender()					{ return requestRender; }

		void setPlayerId(PlayerId playerId)				{ this->playerId = playerId; }
		void setColor(Color color)						{ this->color = color; }

	private:

		static const double OBJECT_LAG_MODIFIER;
		static const int OBJECT_LAG_ADD_TIME;
		static const int OBJECT_LAG_ADD_TICK;

		ClientGlobalObj clientGlobalObj;

		Link link;
		size_t connectionPhase;


		UserInputHandler userInputHandler;
	public:
		ClientPlanet planet;
		PlanetRenderer planetRenderer;
	private:

		ClientPlayers players;
		PredictionHandler *predictionHandler;
		int currentObjLag; // controls the interpolation of objects

		PlayerId playerId;
		Color color;	// should this exist or should the client have an Player object before we connect to server?

		bool requestRender;

	public:
		std::string currentMap;
		Camera camera;
		Sight sight;

		//debug
		bool isConsistent()
		{
			//just test one thing for now
			if (predictionHandler)
			{
				if (!predictionHandler->isConsistent()) return false;
			}
			return true;
		}

		void debugPrintState();

	};
};

#endif

