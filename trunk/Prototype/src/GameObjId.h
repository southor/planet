#ifndef __gameobjid_h__
#define __gameobjid_h__

namespace Prototype
{

	class GameObjId
	{
	private:
		uint playerId;
		uint generatedId;
	public:
		
		// Creates a general GameoObjId
		inline GameObjId(uint playerId, uint generatedId) : playerId(playerId), generatedId(generatedId)
		{}

		// Creates a playerObjId from a playerId
		inline GameObjId(uint playerId) : playerId(playerId), generatedId(0)
		{}

		// debugging
		bool isPlayerObjId()
		{
			return generatedId == 0;
		}

		// From PlayerObjId To PlayerId
		size_t operator (size_t) () const
		{
			assert(isPlayerObjId()); // should be a playerObjId
			return playerId;
		}

		bool operator ==(const GameObjId &gameObjId)
		{
			return this->playerId == gameObjId.playerId &&
					this->generatedId == gameObjId.generatedId;
		}

		bool operator !=(const GameObjId &gameObjId)
		{
			return this->playerId != gameObjId.playerId ||
					this->generatedId != gameObjId.generatedId;
		}

		bool operator >(const GameObjId &gameObjId)
		{
			if (this->playerId > gameObjId.playerId)
			{
				return true;
			}
			else if (this->playerId == gameObjId.playerId)
			{
				return this->generatedId  > gameObjId.generatedId;
			}
			else
			{
				return false;
			}
		}

		bool operator <(const GameObjId &gameObjId)
		{
			if (this->playerId < gameObjId.playerId)
			{
				return true;
			}
			else if (this->playerId == gameObjId.playerId)
			{
				return this->generatedId  < gameObjId.generatedId;
			}
			else
			{
				return false;
			}
		}

		inline bool operator >=(const GameObjId &gameObjId)
		{
			return !(*this < gameObjId);
		}

		inline bool operator <=(const GameObjId &gameObjId)
		{
			return !(*this > gameObjId);
		}

	};

};

#endif
