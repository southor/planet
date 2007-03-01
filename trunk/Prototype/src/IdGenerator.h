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


	// ---------------- Alternative Implementation, only one IdGenerator ---------------- 
	// --------------------- used by both server and client -----------------------------

	//class IdGenerator
	//{
	//protected:		
	//	static const PlayerId SERVER_CREATOR_ID = static_cast<PlayerId>(0);
	//	static const PlayerId FIRST_CLIENT_PLAYER_ID = static_cast<PlayerId>(1);

	//	
	//	PlayerId creatorId; // used by client and server
	//	uint nextId; // used by client and server

	//	PlayerId nextPlayerId; // used by server
	//
	//public:

	//	/**
	//	 * Will create an IdGenerator ready to be used by the server.
	// 	 * When the client uses this class, generatePlayerId must be called before rest of the class is used.
	//	 */
	//	inline IdGenerator() : creatorId(SERVER_CREATOR_ID), nextId(1), nextPlayerId(FIRST_CLIENT_PLAYER_ID)
	//	{}

	//	// Used by client and server
	//	inline GameObjId generateGameObjId()
	//	{
	//		assert(creatorId != INVALID_CREATOR_ID); // creator Id must be set!
	//		return GameObjId(creatorId, nextId++);
	//	}

	//	// Used only by server
	//	inline PlayerId generatePlayerId()
	//	{
	//		assert(creatorId != INVALID_CREATOR_ID); // creator Id must be correct!
	//		return nextPlayerId++;
	//	}

	//	/**
	//	 * Used only by client
	//	 * This function must be called before generateId is called!
	//	 */
	//	inline void setPlayerId(PlayerId playerId)			{ this->creatorId = playerId; }
	//};

};

#endif
