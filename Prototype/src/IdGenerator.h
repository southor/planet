#ifndef __idgenerator_h__
#define __idgenerator_h__

namespace Prototype
{

	class IdGenerator
	{
	protected:
		static const uint INVALID_CREATOR_ID = static_cast<uint>(-1);
		uint creatorId;
		
		uint nextId;	
	
	public:

		inline IdGenerator() : creatorId(INVALID_CREATOR_ID), nextId(1) // zero is taken for PlayerObjId
		{}

		inline GameObjId generateId()
		{
			assert(creatorId != INVALID_CREATOR_ID); // creator Id must be set!
			return GameObjId(creatorId, nextId++);
		}
	};

	class ClientIdGenerator : public IdGenerator
	{
	public:
		
		// This function must be called before generateId is called!
		inline void setPlayerId(size_t playerId)
		{
			creatorId = static_cast<uint>(playerId);
		}
		
	};

	class ServerIdGenerator : public ClientIdGenerator
	{
	private:
		static const uint SERVER_CREATOR_ID = static_cast<uint>(-2);
	public:
		ServerIdGenerator()
		{
			creatorId = SERVER_CREATOR_ID;
		}
	};

};

#endif
