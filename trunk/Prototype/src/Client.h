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
#include "ClientGlobalAccess.h"
#include "ConfigHandler.h"

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

	class Client : public ClientGlobalAccess
	{
	public:

		static const bool DEBUG_SHOOTING = true;

		Client();
		~Client();

		void getCurrentUserCmd(UserCmd &userCmd);
		void handleServerMessages();
		void runStep();

		bool initConnection();
		void renderAndUpdate();
		
		void addPlayer(PlayerId playerId, const Color &playerColor, const Pos &playerPos, int tick);

		void setConnection(MessageSender *messageSender, MessageReciever *messageReciever);
		KeyHandler* getKeyHandler();
		inline UserInputHandler* getUserInputHandler()	{ return &userInputHandler; }
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
		
		inline const ConfigHandler& getConfigHandler()	{ return configHandler; }

		//debug
		void debugPrintState();

	private:

		static const double OBJECT_LAG_MODIFIER_DEFAULT;
		static const int OBJECT_LAG_ADD_TIME_DEFAULT;
		static const int OBJECT_LAG_ADD_TICK_DEFAULT;

		static const int CLIENT_REMOVE_PROJECTILE_DEFAULT = 1;
		static const int CLIENT_CREATE_PROJECTILE_DEFAULT = 1;
		
		ClientGlobalObj clientGlobalObj;

		Link link;
		size_t connectionPhase;

		KeyHandler kh;
		UserInputHandler userInputHandler;


		ClientWorldModel worldModel;
		WorldRenderer worldRenderer;
		ClientPlayers players;
		PredictionHandler *predictionHandler;
		int currentObjLag; // controls the interpolation of objects

		PlayerId playerId;
		Color color;	// should this exist or should the client have an Player object before we connect to server?

		// stores viewport parameters
		ViewportHandler viewportHandler;
		bool requestRender;

		ConfigHandler configHandler;

		/**
		 * Will calculate the player obj aiming angle from the mouse position.		
		 * @param mouseScreenPos The mouse position in screen coordinates
		 */
		Angle calcPlayerObjAngle(Vec2<int> mouseScreenPos);

		Angle calcPlayerObjAngle(Angle preAngle, StateCmds stateCmds);

		int calcCurrentObjLag() const;

		void projectileHit(GameObjId projectileId);

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

	};
};

#endif

