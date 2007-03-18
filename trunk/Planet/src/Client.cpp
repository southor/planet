#include "Client.h"
#include "Game.h"
#include <string>
#include <algorithm>

namespace Planet
{

	Client::Client() : ClientGlobalAccess(&clientGlobalObj), connectionPhase(0) {}

	void Client::init()
	{
		planet.init();		
		ship.setPlanetBody(planet.getPlanetBody());
		sight.setCamera(&camera);
		sight.setPlanetBody(planet.getPlanetBody());
		
		getTimeHandler()->reset();
	}


	Client::~Client()	{}

	void Client::runStep()
	{
		ship.moveUp = userInputHandler.getCurrentState(Cmds::TMP_UP);
		ship.moveDown = userInputHandler.getCurrentState(Cmds::TMP_DOWN);
		ship.moveLeft = userInputHandler.getCurrentState(Cmds::TMP_LEFT);
		ship.moveRight = userInputHandler.getCurrentState(Cmds::TMP_RIGHT);
			
		// Logic
		ship.logic(sight.position);

		camera.update(ship.position, ship.reference);

		sight.update(userInputHandler.getMouseScreenPos(), Game::WINDOW_SIZE.x, Game::WINDOW_SIZE.y);
	}

	bool Client::initConnection()
	{
		if (connectionPhase == ClientPhase::SEND_INITCLIENT)
		{
			// send init package to server
			InitClient initClient = InitClient(color);
			link.pushMessage(initClient, getTimeHandler()->getTime(), static_cast<int>(getStepTick()));
			link.transmit();

			connectionPhase++;
		}

		if (connectionPhase == ClientPhase::WAIT_WELCOME_CLIENT)
		{
			link.retrieve(getTimeHandler()->getTime());
			if (link.hasMessageOnQueue())
			{
				int messageType = link.popMessage();
				if (messageType == WELCOME_CLIENT)
				{
					WelcomeClient *welcomeClient = link.getPoppedData<WelcomeClient>();
				
					setPlayerId(welcomeClient->playerId);

					connectionPhase++;
				}
				else
				{
					assert(false);
				}
			}
		}

		if (connectionPhase == ClientPhase::SYNC_SEND_PING)
		{
			// send ping to server with current client time
			SyncPing syncPing(playerId, getTimeHandler()->getTime());
			link.pushMessage(syncPing, getTimeHandler()->getTime(), static_cast<int>(getStepTick()));
			link.transmit();

			connectionPhase++;
		}

		if (connectionPhase == ClientPhase::SYNC_GET_PONG)
		{
			// get pong pack from server with server time and time when ping was sent
			link.retrieve(getTimeHandler()->getTime());
			if (link.hasMessageOnQueue())
			{
				int messageType = link.popMessage();
				if (messageType == SYNC_PONG)
				{
					SyncPong *syncPong = link.getPoppedData<SyncPong>();
					
					int clientTime = getTimeHandler()->getTime();
					int serverTime = syncPong->time;

					int pingTime = clientTime - syncPong->pingSendTime;
					int serverClientDiff = (serverTime + pingTime/2) - clientTime;

					printf("Adjusting client time with diff: %d\n", serverClientDiff);

					// Modify client time to match server time
					getTimeHandler()->incrementTime(serverClientDiff);
 
					connectionPhase++;

					return true;
				}
				else
				{
					assert(false);
				}
			}
		}

		return false;
	}

	void Client::renderAndUpdate()
	{
		//render
		planet.getPlanetBody()->render();

		// Disable lights for ship and sight rendering
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		ship.render();
		sight.render();

		planetRenderer.render(ship.position, ship.direction);

		requestRender = false;


	}

	void Client::setConnection(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		link.setMessageSender(messageSender);
		link.setMessageReciever(messageReciever);
	}

};
