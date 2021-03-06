#ifndef __server_h__
#define __server_h__

#include <vector>

#include "messages.h"
#include "ServerPlayers.h"
#include "ServerWorldModel.h"
#include "ServerTimeHandler.h"
#include "StateCmds.h"
#include "Cmds.h"

#include "ViewportHandler.h"
#include "WorldRenderer.h"

#include "ConfigHandler.h"

namespace Prototype
{
	namespace ServerPhase
	{
		enum
		{
			WAIT_INITCLIENT,
			WAIT_PING
		};
	};

	class Server : public ServerGlobalAccess
	{
	public:
		Server();

		~Server();

		void logic();

		bool clientConnected(MessageSender *messageSender, MessageReciever *messageReciever);

		// @return The PlayerId that the new player got
		PlayerId addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever);

		void addPlayerObj(PlayerId playerId, const Pos &playerPos);

		// start game, no more clients can join
		void startGame();

		// (server debug rendering)
		inline void setViewport(int x, int y, int w, int h)
		{
			viewportHandler.screenRenderPos.x = x;
			viewportHandler.screenRenderPos.y = y;
			viewportHandler.screenRenderSize.x = w;
			viewportHandler.screenRenderSize.y = h;
		}
		
		inline void useViewport()						{ viewportHandler.useViewport(); }
		void render();

		inline bool getRequestRender()					{ return requestRender; }

		//debug
		void debugPrintState();

	private:

		static const int SERVER_PRINT_TIMEOUT_DEBUG_DEFAULT = 0;
		static const int SERVER_PRINT_TICK_DEBUG_DEFAULT = 0;

		static const double PREDICTION_AMOUNT_MODIFIER_DEFAULT;
		static const int PREDICTION_AMOUNT_ADD_TIME_DEFAULT;
		
		ServerWorldModel worldModel;
		ServerPlayers players;

		ServerGlobalObj serverGlobalObj;
		int lastUpdateTime;

		// stores viewport parameters (server debug rendering)
		ViewportHandler viewportHandler;

		// worldrenderer (server debug rendering)
		WorldRenderer worldRenderer;
		bool requestRender;

		ConfigHandler configHandler;

	};
};

#endif
