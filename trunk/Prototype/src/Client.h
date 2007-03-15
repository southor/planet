#ifndef __client_h__
#define __client_h__

#include "common.h"
#include "Link.h"
#include "ClientWorldModel.h"
#include "ClientPlayer.h"
#include "WorldRenderer.h"
#include "KeyHandler.h"
#include "UserInputHandler.h"
#include "ClientTimeHandler.h"
#include "ViewportHandler.h"
#include "PredictionHandler.h"

namespace Prototype
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

		static const bool DEBUG_SHOOTING = false;

		static const int MAX_N_PLAYERS = 2;



		Client();

		~Client();

		
		//void handleEvents();
		//void logic();		
		void getCurrentUserCmd(UserCmd &userCmd);
		void handleServerMessages();
		void runStep();

		bool initConnection();
		void renderAndUpdate();
		
		
		void addPlayer(PlayerId playerId, const Color &playerColor, const Pos &playerPos, int tick);

		void setConnection(MessageSender *messageSender, MessageReciever *messageReciever);
		KeyHandler* getKeyHandler();
		inline UserInputHandler* getUserInputHandler()	{ return &userInputHandler; }
		inline ClientTimeHandler* getTimeHandler()		{ return &timeHandler; }
		inline bool getRequestRender()					{ return requestRender; }
		
		void setPlayerId(PlayerId playerId)				{ this->playerId = playerId; }
		void setColor(Color color)						{ this->color = color; }
		inline void setViewport(int x, int y, int w, int h)
		{
			viewportHandler.screenRenderPos.x = x;
			viewportHandler.screenRenderPos.y = y;
			viewportHandler.screenRenderSize.x = w;
			viewportHandler.screenRenderSize.y = h;
		}
		inline void useViewport()						{ viewportHandler.useViewport(); }
		//void setCurrentMousePos(Vec2<int> mouseScreenPos);
		//inline void setAimMode(UserInputHandler::AimMode aimMode)				{ userInputHandler. }
		

	private:

		static const double OBJECT_LAG_MODIFIER;
		static const int OBJECT_LAG_ADD_TIME;
		static const int OBJECT_LAG_ADD_TICK;
		

		Link link;
		size_t connectionPhase;
		ClientTimeHandler timeHandler;


		KeyHandler kh;
		UserInputHandler userInputHandler;


		ClientWorldModel worldModel;
		WorldRenderer worldRenderer;
		ClientPlayers players;
		PredictionHandler predictionHandler;
		int currentObjLag; // controls the interpolation of objects

		PlayerId playerId;
		Color color;	// should this exist or should the client have an Player object before we connect to server?

		// stores viewport parameters
		ViewportHandler viewportHandler;
		bool requestRender;

		///**
		// * will set player object angle if player aiming is mouse controlled
		// * @param mouseScreenPos The mouse position in screen coordinates
		// */
		//void updatePlayerObjAngle(Vec2<int> mouseScreenPos);

		/**
		 * Will calculate the player obj aiming angle from the mouse position.		
		 * @param mouseScreenPos The mouse position in screen coordinates
		 */
		Angle calcPlayerObjAngle(Vec2<int> mouseScreenPos);

		Angle calcPlayerObjAngle(Angle preAngle, StateCmds stateCmds);

		//debug
		bool isConsistent()
		{
			//just test one thing for now
			return predictionHandler.isConsistent();
		}

	};
};

#endif

