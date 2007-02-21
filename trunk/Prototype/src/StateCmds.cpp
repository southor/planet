#include "StateCmds.h"

namespace Prototype
{
	bool StateCmds::getCurrentState(int stateCmd)
	{ 
		return static_cast<bool>((1 << stateCmd) & states); 
	}
	
	void StateCmds::setCurrentState(int stateCmd, bool state)
	{
		int mask = (1 << stateCmd);
	
		if (getCurrentState(stateCmd) != state)
		{
			// swap the stateCmd bit
			states = states ^ mask;
		}
	}
};
