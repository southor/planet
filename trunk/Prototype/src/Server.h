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
		inline void useViewport()			 { viewportHandler.useViewport(); }
		void render()
		{
			worldRenderer.setupProjection();			
			PlayerId playerIdFollow(1);
			if (worldModel.getPlayerObjs().exists(playerIdFollow))
			{
				worldRenderer.render(worldModel, players, (worldModel.getPlayerObjs())[playerIdFollow]);//, static_cast<Tickf>(getTimeHandler()->getTick()));
			}
		}

	private:
		
		
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

	};
};

#endif
