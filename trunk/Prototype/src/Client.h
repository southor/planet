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

namespace Prototype
{
	class Client
	{
	public:

		static const int MAX_N_PLAYERS = 2;

		enum AimMode
		{
			KEYBOARD,
			MOUSE			
		};

		Client();

		void handleEvents();
		void logic();
		bool initConnection();
		void render();
		
		void addPlayer(PlayerId playerId, const Color &playerColor, const Pos &playerPos);

		void setConnection(MessageSender *messageSender, MessageReciever *messageReciever);
		KeyHandler* getKeyHandler();
		UserInputHandler* getUserInput()	{ return &userInput; }
		ClientTimeHandler* getTimeHandler() { return &timeHandler; }
		
		void setPlayerId(PlayerId playerId) { this->playerId = playerId; }
		void setColor(Color color) { this->color = color; }
		inline void setViewport(int x, int y, int w, int h)
		{
			viewportHandler.screenRenderPos.x = x;
			viewportHandler.screenRenderPos.y = y;
			viewportHandler.screenRenderSize.x = w;
			viewportHandler.screenRenderSize.y = h;
		}
		inline void useViewport()			{ viewportHandler.useViewport(); }
		void setCurrentMousePos(Vec2<int> mouseScreenPos);
		inline void setAimMode(AimMode aimMode)				{ this->aimMode = aimMode; }
		

	private:

		
		Pos mousePos; // mouse position in game coordinates
		bool mousePosChanged;
		AimMode aimMode; // aiming for shooting, mouse or keyboard

		Link link;
		size_t connectionPhase;
		ClientTimeHandler timeHandler;


		KeyHandler kh;
		UserInputHandler userInput;


		ClientWorldModel worldModel;
		WorldRenderer worldRenderer;
		ClientPlayers players;

		PlayerId playerId;
		Color color;	// should this exist or should the client have an Player object before we connect to server?

		// stores viewport parameters
		ViewportHandler viewportHandler;

		// will set player object angle if player aiming is mouse controlled
		void updatePlayerObjAngle();
	};
};

#endif

