#ifndef __UserInputHandler_h__
#define __UserInputHandler_h__

#include "Cmds.h"
#include "StateCmds.h"
#include "common.h"
#include "basic.h"

#include <deque>
#include <map>

namespace Planet
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


		// mouse position in screen coordinates
		Vec2i mouseScreenPos;
		//bool mousePosChanged;
		

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
		//void setStateCmdKey(int stateCmd, int key); // keyboard key
		//void setStateCmdButton(int stateCmd, int button); // mouse button

		// ------- set Action cmd keys and mouse buttons -------
		void setActionCmdKey(int actionCmd, int key);
		//void setActionCmdKey(int actionCmd, int key); // keyboard key
		//void setActionCmdButton(int actionCmd, int button); // mouse button


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
		inline Vec2i& getMouseScreenPos()		{ return mouseScreenPos; }
		//inline bool hasMousePosChanged()
		//{
		//	bool tmp = mousePosChanged;
		//	mousePosChanged = false;
		//	return tmp;
		//}		
		
		// ------ primitive event input --------
		void pushInput(const SDL_Event &event);

	private:

		

		//void setCurrentMousePos(Vec2<int> mouseScreenPos, const ViewportHandler &viewportHandler);

	};
};

#endif
