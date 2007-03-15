#ifndef __Server_h__
#define __Server_h__

#include <vector>

//#include "messages.h"
#include "ServerPlayers.h"
//#include "ServerPlanet.h"
#include "ServerTimeHandler.h"
#include "IdGenerator.h"
#include "StateCmds.h"
#include "Cmds.h"
#include "MessageSender.h"
#include "MessageReciever.h"
#include "Link.h"

//#include "ViewportHandler.h"
//#include "WorldRenderer.h"

namespace Planet
{
	namespace ServerPhase
	{
		enum
		{
			WAIT_INITCLIENT,
			WAIT_PING
		};
	};

	class Server //: public ServerGlobalAccess
	{
	public:
		Server();
		~Server();

		void run(bool &runningServer, int numberOfClients);

	//	void logic();

	bool clientConnected(MessageSender *messageSender, MessageReciever *messageReciever);

	private:
	// @return The PlayerId that the new player got
	PlayerId addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever);

	//	void addPlayerObj(PlayerId playerId, const Pos &playerPos);

	public:
	//	// start game, no more clients can join
	//	void startGame();

	//	// (server debug rendering)
	//	inline void setViewport(int x, int y, int w, int h)
	//	{
	//		viewportHandler.screenRenderPos.x = x;
	//		viewportHandler.screenRenderPos.y = y;
	//		viewportHandler.screenRenderSize.x = w;
	//		viewportHandler.screenRenderSize.y = h;
	//	}

	//	inline void useViewport()			 { viewportHandler.useViewport(); }
	//	void render();

		ServerTimeHandler* getTimeHandler() { return &timeHandler; }
		ServerIdGenerator* getIdGenerator() { return &idGenerator; }

	//private:

	//	static const double PREDICTION_AMOUNT_MODIFIER;
	//	static const int PREDICTION_AMOUNT_ADD_TIME;
	//	
	//	ServerPlanet worldModel;
		ServerPlayers players;

	//	ServerGlobalObj serverGlobalObj;
	//	int lastUpdateTime;

	//	// stores viewport parameters (server debug rendering)
	//	ViewportHandler viewportHandler;

	//	// worldrenderer (server debug rendering)
	//	WorldRenderer worldRenderer;

	//	bool requestRender;
	

		ServerTimeHandler timeHandler;
		ServerIdGenerator idGenerator;
	};
};

#endif
