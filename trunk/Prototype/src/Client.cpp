#include "Client.h"
//#include <iostream>
#include <string>

namespace Prototype
{

	Client::Client() : messageSender(0), messageReciever(0), worldRenderer(WorldRenderer::HOLE_WORLD)
	{
		players.reserve(MAX_N_PLAYERS);
		//(worldModel.getPlayerObjs()).reserve(MAX_N_PLAYERS);
		//worldRenderer.setupProjection();
	}

	void Client::logic()
	{
	}

	void Client::render()
	{
		worldRenderer.setupProjection();
		worldRenderer.render(worldModel, players, 0);
	}

	void Client::sendMessage(Message message)
	{
		std::cout << "sending message, type: " << message.type << ", data: " << *((std::string*)message.data) << std::endl;
		
		message.time = timeHandler.getTime();
		
		messageSender->pushMessage(message);
		messageSender->transmit();
	}
	
	void Client::recieveMessages()
	{
		while (messageReciever->getNMessages() != 0)
		{
			Message message = messageReciever->popMessage();
		
			std::cout << "message recieved, type: " << message.type << ", data: " << *((std::string*)message.data) << std::endl;
		}
	}

	void Client::addPlayer(const Color &playerColor, const Pos &playerPos)
	{
		int playerId = static_cast<int>(players.size());
		players.push_back(Player(playerColor));
		worldModel.getPlayerObjs().push_back(new PlayerObj(playerId, playerPos));
	}

	void Client::setConnection(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		this->messageSender = messageSender;
		this->messageReciever = messageReciever;
	}


};
