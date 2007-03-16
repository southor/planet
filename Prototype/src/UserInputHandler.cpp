#include "UserInputHandler.h"
#include "Game.h"

namespace Prototype
{

	void UserInputHandler::setStateCmdKey(int stateCmd, int key)
	{
		stateCmdKeyMap[key] = stateCmd;
	}
	
	void UserInputHandler::setActionCmdKey(int actionCmdPress, int actionCmdRelease, int key)
	{
		Action pressAction(key, true);
		Action releaseAction(key, false);
		actionCmdKeyMap[pressAction] = actionCmdPress;
		actionCmdKeyMap[releaseAction] = actionCmdRelease;
	}

	size_t UserInputHandler::getNActionCmdsOnQueue()
	{
		return actionCmdQueue.size();
	}
	
	bool UserInputHandler::hasActionCmdOnQueue()
	{ 
		return getNActionCmdsOnQueue() > 0;
	}
	
	int UserInputHandler::popActionCmd()
	{
		int actionCmd = actionCmdQueue.front();
		actionCmdQueue.pop_front();
		return actionCmd;
	}

	void UserInputHandler::pushActionCmd(int actionCmd)
	{
		actionCmdQueue.push_back(actionCmd);
	}

	void UserInputHandler::pushActionCmdIf(int key, bool press)
	{
		Action action(key, press);
		if (actionCmdKeyMap.count(action) > 0)
		{
			int actionCmd = actionCmdKeyMap[action];
			if (actionCmd != NO_ACTION_CMD) pushActionCmd(actionCmd);
		}
	}
	
	void UserInputHandler::pushInput(const SDL_Event &event)
	{
		int key;
		int button;

		//event.
	
		switch(event.type)
		{
		case SDL_KEYDOWN:
			key = event.key.keysym.sym;

			// set state command bit in currentStates to true
			if (stateCmdKeyMap.count(key) > 0)
				setCurrentState(stateCmdKeyMap[key], true);

			// push if there is a corresponding actionCmd
			pushActionCmdIf(key, true);

			break;

		case SDL_KEYUP:
			key = event.key.keysym.sym;

			// set state command bit in currentStates to false
			if (stateCmdKeyMap.count(key) > 0)
				setCurrentState(stateCmdKeyMap[key], false);

			// push action command if action key released
			pushActionCmdIf(key, false);

			break;
			
		case SDL_MOUSEBUTTONDOWN:
			button = event.button.button;

			// set state command bit in currentStates to true
			if (stateCmdKeyMap.count(button) > 0)
				setCurrentState(stateCmdKeyMap[button], true);

			// push action command if action key pressed
			pushActionCmdIf(button, true);
			
			break;
		case SDL_MOUSEBUTTONUP:
			button = event.button.button;

			// set state command bit in currentStates to false
			if (stateCmdKeyMap.count(button) > 0)
				setCurrentState(stateCmdKeyMap[button], false);

			// push action command if action key released
			pushActionCmdIf(button, false);

			break;

		case SDL_MOUSEMOTION:
			{
				Vec2<int> tmpMouseScreenPos(event.motion.x, event.motion.y);
				tmpMouseScreenPos.y = Game::WINDOW_SIZE.y - tmpMouseScreenPos.y; // convert from SDL to GL position
				//setCurrentMousePos(mouseScreenPos);
				//mousePosChanged = true;
				this->mouseScreenPos = tmpMouseScreenPos;			
			}
			break;

		}
	}

	//void UserInputHandler::setCurrentMousePos(Vec2<int> mouseScreenPos)
	//{
	//	mousePosChanged = true;
		//if (playerObj)
		//{			
			//viewportHandler.renderArea.size = WorldRenderer::RENDER_SIZE; // set current render area
			//viewportHandler.renderArea.setCenterPos(playerObj->pos); // set current render area
			//mousePos = viewportHandler.screenToGame(mouseScreenPos); // mouse position in game
		//}
		
	//}
	
};
