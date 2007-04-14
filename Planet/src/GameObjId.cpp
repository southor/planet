#include "GameObjId.h"

namespace Planet
{
	
	bool GameObjId::isConsistent() const
	{
		if ((playerId < 0) || (playerId >= MAX_N_PLAYERS)) return false;			
		return generatedId < 2147483648;
	}
	
	bool playerIdIsConsistent(PlayerId playerId)
	{		
		GameObjId gameObjId(playerId);		
		return (gameObjId.isConsistent() && gameObjId.isPlayerObjId());
	}

};
