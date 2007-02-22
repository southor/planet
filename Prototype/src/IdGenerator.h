#ifndef __idgenerator_h__
#define __idgenerator_h__

#include "GameObjId.h"

namespace Prototype
{

	class IdGenerator
	{
	protected:
		static const PlayerId INVALID_CREATOR_ID = GameObjId::INVALID_PLAYER_ID;
		PlayerId creatorId;
				
		uint nextId;	
	
	public:

		inline IdGenerator() : creatorId(INVALID_CREATOR_ID), nextId(1) // zero is taken for PlayerObjId
		{}

		inline GameObjId generateGameObjId()
		{
			assert(creatorId != INVALID_CREATOR_ID); // creator Id must be set!
			return GameObjId(creatorId, nextId++);
		}
	};

	class ClientIdGenerator : public IdGenerator
	{
	public:
		
		// This function must be called before generateId is called!
		inline void setPlayerId(PlayerId playerId)		{ this->creatorId = playerId; }		
		
	};

	class ServerIdGenerator : public ClientIdGenerator
	{
	private:
		static const PlayerId SERVER_CREATOR_ID = static_cast<PlayerId>(0);
		static const PlayerId FIRST_CLIENT_PLAYER_ID = static_cast<PlayerId>(1);

		PlayerId nextPlayerId;
	public:
		
		ServerIdGenerator() : nextPlayerId(FIRST_CLIENT_PLAYER_ID)
		{
			creatorId = SERVER_CREATOR_ID;
		}

		inline PlayerId generatePlayerId()
		{
			assert(creatorId != INVALID_CREATOR_ID); // creator Id must be correct!
			return nextPlayerId++;
		}
	};

};

#endif
