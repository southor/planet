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

		//// @return The PlayerObjId that the new player object got
		//GameObjId addPlayerObj(size_t playerId, const Pos &playerPos);

		void addPlayerObj(PlayerId playerId, const Pos &playerPos);


		// @return The ObstacleId that the new obstacle got		 
		//size_t addObstacle(const Rectangle &obstacleArea);

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
		inline void useViewport()					{ viewportHandler.useViewport(); }
		void render();

		inline bool getRequestRender()				{ return requestRender; }

		//debug
		void debugPrintState();

	private:

		static const double PREDICTION_AMOUNT_MODIFIER;
		static const int PREDICTION_AMOUNT_ADD_TIME;
		
		
		
		//size_t addPlayer(const ServerPlayer &player);

		

		ServerWorldModel worldModel;
		ServerPlayers players;

		//ServerTimeHandler timeHandler;
		ServerGlobalObj serverGlobalObj;
		int lastUpdateTime;

		//std::vector<ServerClient> clients;


		// stores viewport parameters (server debug rendering)
		ViewportHandler viewportHandler;
		// worldrenderer (server debug rendering)
		WorldRenderer worldRenderer;
		bool requestRender;

		ConfigHandler configHandler;

	};
};

#endif
