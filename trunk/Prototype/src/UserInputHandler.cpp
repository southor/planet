#include "UserInputHandler.h"

namespace Prototype
{

	void UserInputHandler::setStateCmdKey(int stateCmd, int key) // keyboard key
	{
		stateCmdKeyMap[key] = stateCmd;
	}
	
	void UserInputHandler::setActionCmdKey(int actionCmd, int key) // keyboard key
	{
		actionCmdKeyMap[key] = actionCmd;
	}

	size_t UserInputHandler::getNActionCmdsOnQueue()
	{
		return actionCmdQueue.size();
	}
	
	bool UserInputHandler::hasActionCmdOnQueue()
	{ 
		return getNActionCmdsOnQueue() > 0;
	}
	
	int UserInputHandler:: popActionCmd()
	{
		int actionCmd = actionCmdQueue.front();
		actionCmdQueue.pop_front();
		return actionCmd;
	}

	void UserInputHandler::pushActionCmd(int actionCmd)
	{
		actionCmdQueue.push_back(actionCmd);
	}
	
	void UserInputHandler::pushInput(const SDL_Event &event)
	{
		int key = event.key.keysym.sym;
	
		switch(event.type)
		{
		case SDL_KEYDOWN:
			// set state command bit in currentStates to true
			if (stateCmdKeyMap.count(key) > 0)
				setCurrentState(stateCmdKeyMap[key], true);

			// push action command if action key pressed
			if (actionCmdKeyMap.count(key) > 0)
				pushActionCmd(actionCmdKeyMap[key]);

			break;

		case SDL_KEYUP:
			// set state command bit in currentStates to false
			if (stateCmdKeyMap.count(key) > 0)
				setCurrentState(stateCmdKeyMap[key], false);

			break;
		}
	}
	
};
