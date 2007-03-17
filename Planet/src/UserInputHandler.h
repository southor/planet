#ifndef __UserInputHandler_h__
#define __UserInputHandler_h__

#include "Cmds.h"
#include "StateCmds.h"
#include "common.h"
#include "vec2.h"

#include <deque>
#include <map>

namespace Planet
{
	class UserInputHandler
	{
	private:

		struct Action
		{			
			int key;
			bool press;

			Action()												{}
			Action(int key, bool press) : key(key), press(press) 	{}

			bool operator <(Action action) const
			{
				if (this->key < action.key) return true;
				else if (this->key > action.key) return false;
				else return this->press < action.press;
			}
		};

		// current state of the state commands
		StateCmds currentStates;

		// queue of action commands
		std::deque<int> actionCmdQueue;
		
		// holds the key of the state command
		std::map<int, int> stateCmdKeyMap;

		// holds the key of the action command
		std::map<Action, int> actionCmdKeyMap;


		// mouse position in screen coordinates
		Vec2<int> mouseScreenPos;

		static const int NO_ACTION_CMD = -1;
		
		void pushActionCmdIf(int key, bool press);

	public:

		enum AimMode
		{
			KEYBOARD,
			MOUSE
		};

		// aiming for shooting, mouse or keyboard
		AimMode aimMode;
		
		UserInputHandler() : mouseScreenPos(0, 0), aimMode(KEYBOARD) //, mousePosChanged(true)
		{}

		// ------- set State cmd keys and mouse buttons -------
		void setStateCmdKey(int stateCmd, int key);

		// ------- set Action cmd keys and mouse buttons -------
		void setActionCmdKey(int actionCmdPress, int actionCmdRelease, int key);
		inline void setActionCmdKey(int actionCmd, int key)
		{
			setActionCmdKey(actionCmd, NO_ACTION_CMD, key);
		}		


		// ------- get state cmd input -------
		inline StateCmds getCurrentStates()				{ return currentStates; }

		bool getCurrentState(int stateCmd)				{ return currentStates.getCurrentState(stateCmd); }
		void setCurrentState(int stateCmd, bool state)	{ currentStates.setCurrentState(stateCmd, state); }


		// ------- get action cmd input -------
		size_t getNActionCmdsOnQueue();
		bool hasActionCmdOnQueue();
		int popActionCmd();
		void pushActionCmd(int actionCmd);
		

		// ------- mouse move input -----------
		inline Vec2<int>& getMouseScreenPos()		{ return mouseScreenPos; }
		
		// ------ primitive event input --------
		void pushInput(const SDL_Event &event);

	private:

	};
};

#endif
