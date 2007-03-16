#include "Client.h"
#include "Game.h"
#include <string>
#include <algorithm>

namespace Planet
{

	Client::Client() : planetBody(5.0f), connectionPhase(0) {}

	void Client::init()
	{
		planetBody.init();
		ship.setPlanet(&planetBody);
		sight.setCamera(&camera);
		sight.setPlanet(&planetBody);
		
		timeHandler.reset();
	}


	Client::~Client()	{}

	void Client::runStep()
	{
		ship.moveUp = userInputHandler.getCurrentState(Cmds::TMP_UP);
		ship.moveDown = userInputHandler.getCurrentState(Cmds::TMP_DOWN);
		ship.moveLeft = userInputHandler.getCurrentState(Cmds::TMP_LEFT);
		ship.moveRight = userInputHandler.getCurrentState(Cmds::TMP_RIGHT);
			
		// Logic
		ship.logic();

		ship.lookAt(sight.position);

		camera.update(ship.position, ship.reference);

		sight.update(userInputHandler.getMouseScreenPos(), Game::WINDOW_SIZE.x, Game::WINDOW_SIZE.y);
	 

		

	}

	bool Client::initConnection()
	{
		if (connectionPhase == ClientPhase::SEND_INITCLIENT)
		{
			// send init package to server
			InitClient initClient = InitClient(color);
			link.pushMessage(initClient, timeHandler.getTime(), static_cast<int>(timeHandler.getStepTick()));
			link.transmit();

			connectionPhase++;
		}

		if (connectionPhase == ClientPhase::WAIT_WELCOME_CLIENT)
		{
			link.retrieve(timeHandler.getTime());
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
			SyncPing syncPing(playerId, timeHandler.getTime());
			link.pushMessage(syncPing, timeHandler.getTime(), static_cast<int>(timeHandler.getStepTick()));
			link.transmit();

			connectionPhase++;
		}

		if (connectionPhase == ClientPhase::SYNC_GET_PONG)
		{
			// get pong pack from server with server time and time when ping was sent
			link.retrieve(timeHandler.getTime());
			if (link.hasMessageOnQueue())
			{
				int messageType = link.popMessage();
				if (messageType == SYNC_PONG)
				{
					SyncPong *syncPong = link.getPoppedData<SyncPong>();
					
					int clientTime = timeHandler.getTime();
					int serverTime = syncPong->time;

					int pingTime = clientTime - syncPong->pingSendTime;
					int serverClientDiff = (serverTime + pingTime/2) - clientTime;

					printf("Adjusting client time with diff: %d\n", serverClientDiff);

					// Modify client time to match server time
					timeHandler.incrementTime(serverClientDiff);
 
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
		planetBody.render();

		// Disable lights for ship and sight rendering
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		ship.render();
		sight.render();



		requestRender = false;


	}

	void Client::setConnection(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		link.setMessageSender(messageSender);
		link.setMessageReciever(messageReciever);
	}

};
