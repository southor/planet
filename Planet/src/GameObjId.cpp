#include "GameObjId.h"

namespace Planet
{
	
	bool GameObjId::isConsistent() const
	{
		//int a = max(6, 9);
		if ((playerId < 0) || (playerId >= MAX_N_PLAYERS)) return false;			
		//return generatedId < static_cast<uint>(std::numeric_limits<int>::max());		
		//return generatedId < INT_MAX;
		return generatedId < 2147483648;
	}
	
	bool playerIdIsConsistent(PlayerId playerId)
	{		
		GameObjId gameObjId(playerId);		
		return (gameObjId.isConsistent() && gameObjId.isPlayerObjId());
	}


};
