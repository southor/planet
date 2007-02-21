#ifndef __UserInputHandler_h__
#define __UserInputHandler_h__

#include "Cmds.h"
#include "StateCmds.h"
#include "common.h"

#include <deque>
#include <map>

namespace Prototype
{
	class UserInputHandler
	{
	private:
		// current state of the state commands
		StateCmds currentStates;

		// queue of action commands
		std::deque<int> actionCmdQueue;
		
		// holds the key of the state command
		std::map<int, int> stateCmdKeyMap;

		// holds the key of the action command
		std::map<int, int> actionCmdKeyMap;


	public:
		UserInputHandler() {}


		// ------- set State cmd keys and mouse buttons -------
		void setStateCmdKey(int stateCmd, int key); // keyboard key
		void setStateCmdButton(int stateCmd, int button); // mouse button

		// ------- set Action cmd keys and mouse buttons -------
		void setActionCmdKey(int actionCmd, int key); // keyboard key
		void setActionCmdButton(int actionCmd, int button); // mouse button


		// ------- get state cmd input -------
		inline StateCmds getCurrentStates()					{ return currentStates; }

		bool getCurrentState(int stateCmd)				{ currentStates.getCurrentState(stateCmd); }
		void setCurrentState(int stateCmd, bool state)	{ currentStates.setCurrentState(stateCmd, state); }


		// ------- get action cmd input -------
		size_t getNActionCmdsOnQueue();
		inline bool hasActionCmdOnQueue();
		int popActionCmd();
		void pushActionCmd(int actionCmd);
		
		void pushInput(const SDL_Event &event);
	};
};

#endif
