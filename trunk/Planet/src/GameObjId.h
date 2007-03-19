#ifndef __gameobjid_h__
#define __gameobjid_h__

#include "basic.h"
#include <limits>
//#include "PlayerId.h"
//#define max(x) max(x)

namespace Planet
{

	class GameObjId
	{
	private:
		PlayerId playerId;
		uint generatedId;
	public:

		static const PlayerId INVALID_PLAYER_ID = static_cast<uint>(-1);

		// Creates an invalid PlayerObjId
		inline GameObjId() : playerId(INVALID_PLAYER_ID)
		{}
		
		// Creates a general GameoObjId
		inline GameObjId(PlayerId playerId, uint generatedId) : playerId(playerId), generatedId(generatedId)
		{}

		// Creates a playerObjId from a playerId
		inline GameObjId(PlayerId playerId) : playerId(playerId), generatedId(0)
		{}

		// debugging
		bool isPlayerObjId() const
		{
			return generatedId == 0;
		}

		// debugging
		bool isValid() const
		{
			return (playerId != INVALID_PLAYER_ID);
		}

		// From PlayerObjId To PlayerId
		operator PlayerId () const
		{
			assert(isPlayerObjId()); // should be a playerObjId
			return playerId;
		}

		bool operator ==(const GameObjId &gameObjId) const
		{
			return this->playerId == gameObjId.playerId &&
					this->generatedId == gameObjId.generatedId;
		}

		bool operator !=(const GameObjId &gameObjId) const
		{
			return this->playerId != gameObjId.playerId ||
					this->generatedId != gameObjId.generatedId;
		}

		bool operator >(const GameObjId &gameObjId) const
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

		bool operator <(const GameObjId &gameObjId) const
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

		inline bool operator >=(const GameObjId &gameObjId) const
		{
			return !(*this < gameObjId);
		}

		inline bool operator <=(const GameObjId &gameObjId) const
		{
			return !(*this > gameObjId);
		}

		GameObjId operator +(int n) const
		{
			return GameObjId(playerId, static_cast<uint>(static_cast<int>(generatedId) + n));
		}

		const GameObjId& operator +=(int n)
		{
			generatedId += n;
			return *this;
		}



		bool isConsistent() const
		{
			if ((playerId < 0) || (playerId >= MAX_N_PLAYERS)) return false;			
			//return generatedId < static_cast<uint>(std::numeric_limits<int>::max());
			return generatedId < INT_MAX;
		}

	};

	bool playerIdIsConsistent(PlayerId playerId);

};

#endif
