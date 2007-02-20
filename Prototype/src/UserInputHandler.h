#ifndef __UserInputHandler_h__
#define __UserInputHandler_h__

#include "Cmds.h"
#include "common.h"

#include <deque>
#include <map>

namespace Prototype
{
	class UserInputHandler
	{
	private:
		// current state of the state commands
		int currentStates;

		// queue of action commands
		std::deque<int> actionCmdQueue;
		
		// holds the key of the state command
		std::map<int, int> stateCmdKeyMap;

		// holds the key of the action command
		std::map<int, int> actionCmdKeyMap;


	public:

		UserInputHandler() : currentStates(0)
		{}


		// ------- set State cmd keys and mouse buttons -------
		void setStateCmdKey(int stateCmd, int key) // keyboard key
		{
			stateCmdKeyMap[key] = stateCmd;
		}
		
		void setStateCmdButton(int stateCmd, int button); // mouse button

		// ------- set Action cmd keys and mouse buttons -------
		void setActionCmdKey(int actionCmd, int key) // keyboard key
		{
			actionCmdKeyMap[key] = actionCmd;
		}
		
		void setActionCmdButton(int actionCmd, int button); // mouse button



		// ------- get state cmd input -------
		inline int getCurrentStates()				{ return currentStates; }
		inline bool getCurrentState(int stateCmd)	{ return static_cast<bool>((1 >> stateCmd) & getCurrentStates()); }
		inline void setCurrentState(int stateCmd, bool state) { /* TODO */ }

		// ------- get action cmd input -------
		size_t getNActionCmdsOnQueue()					{ return actionCmdQueue.size(); }
		inline bool hasActionCmdOnQueue()			{ return getNActionCmdsOnQueue() > 0; }
		int popActionCmd()
		{
			int actionCmd = actionCmdQueue.front();
			actionCmdQueue.pop_front();
			return actionCmd;
		}
		void pushActionCmd(int actionCmd)			{ actionCmdQueue.push_back(actionCmd); }
		
		
		void pushInput(const SDL_Event &event)
		{
			int key = event.key.keysym.sym;
		
			switch(event.type)
			{
			case SDL_KEYDOWN:
				if (stateCmdKeyMap.count(key) > 0)
					setCurrentState(stateCmdKeyMap[key], true);

				if (actionCmdKeyMap.count(key) > 0)
					pushActionCmd(actionCmdKeyMap[key]);
				
				break;

			case SDL_KEYUP:
				if (stateCmdKeyMap.count(key) > 0)
					setCurrentState(stateCmdKeyMap[key], false);
			
				break;

			}

		}
		
	};
};

#endif
