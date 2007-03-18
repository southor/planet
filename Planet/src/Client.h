#ifndef __Client_h__
#define __Client_h__

#include "common.h"
#include "Link.h"
#include "UserInputHandler.h"
#include "Planet.h"
#include "Ship.h"
#include "Camera.h"
#include "Sight.h"
#include "ClientTimeHandler.h"
#include "ClientGlobalAccess.h"
#include "PlanetRenderer.h"

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

	class Client : public ClientGlobalAccess
	{
	public:

		static const bool DEBUG_SHOOTING = true;

		static const int MAX_N_PLAYERS = 2;



		Client();

		~Client();
		void init();

			
		//void getCurrentUserCmd(UserCmd &userCmd);
		void handleServerMessages();
		void runStep();

		bool initConnection();
		void renderAndUpdate();
		
		
		void addPlayer(PlayerId playerId, const Color &playerColor, const Pos &playerPos, int tick);

		void setConnection(MessageSender *messageSender, MessageReciever *messageReciever);
		//KeyHandler* getKeyHandler();
		Camera* getCamera() { return &camera; }	
		inline UserInputHandler* getUserInputHandler()	{ return &userInputHandler; }
		inline bool getRequestRender()					{ return requestRender; }
		
		void setPlayerId(PlayerId playerId)				{ this->playerId = playerId; }
		void setColor(Color color)						{ this->color = color; }
		//inline void setViewport(int x, int y, int w, int h)
		//{
		//	viewportHandler.screenRenderPos.x = x;
		//	viewportHandler.screenRenderPos.y = y;
		//	viewportHandler.screenRenderSize.x = w;
		//	viewportHandler.screenRenderSize.y = h;
		//}
		//inline void useViewport()						{ viewportHandler.useViewport(); }
		

	private:

		static const double OBJECT_LAG_MODIFIER;
		static const int OBJECT_LAG_ADD_TIME;
		static const int OBJECT_LAG_ADD_TICK;
		
		ClientGlobalObj clientGlobalObj;

		Link link;
		size_t connectionPhase;


		UserInputHandler userInputHandler;

		Planet planet;
		PlanetRenderer planetRenderer;


		//ClientWorldModel worldModel;
		//WorldRenderer worldRenderer;
		//ClientPlayers players;
		//PredictionHandler predictionHandler;
		//int currentObjLag; // controls the interpolation of objects

		PlayerId playerId;
		Color color;	// should this exist or should the client have an Player object before we connect to server?

		// stores viewport parameters
		//ViewportHandler viewportHandler;
		bool requestRender;

		/**
		 * Will calculate the player obj aiming angle from the mouse position.		
		 * @param mouseScreenPos The mouse position in screen coordinates
		 */
		//Angle calcPlayerObjAngle(Vec2<int> mouseScreenPos);

	public:
		//PlanetBody planetBody;
		Ship ship;
		Camera camera;
		Sight sight;
		//Angle calcPlayerObjAngle(Angle preAngle, StateCmds stateCmds);

		//debug
		//bool isConsistent()
		//{
		//	//just test one thing for now
		//	return predictionHandler.isConsistent();
		//}

	};
};

#endif

