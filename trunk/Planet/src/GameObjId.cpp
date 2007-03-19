#include "GameObjId.h"

namespace Planet
{
	bool playerIdIsConsistent(PlayerId playerId)
	{		
		GameObjId gameObjId(playerId);		
		return (gameObjId.isConsistent() && gameObjId.isPlayerObjId());
	}
};
