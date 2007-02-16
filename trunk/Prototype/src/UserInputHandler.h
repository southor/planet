#ifndef __UserInputHandler_h__
#define __UserInputHandler_h__

#include "Cmds.h"

namespace Prototype
{
	class UserInputHandler
	{
	private:

		// current state of the state commands
		int currentStates;



	public:

		UserInputHandler() : currentStates(0)
		{}


		// ------- set State cmd keys and mouse buttons -------
		void setStateCmdKey(int stateCmd, int key); // keyboard key
		void setStateCmdButton(int stateCmd, int button); // mouse button

		// ------- set Action cmd keys and mouse buttons -------
		void setActionCmdKey(int actionCmd, int key); // keyboard key
		void setActionCmdButton(int actionCmd, int button); // mouse button



		// ------- get state cmd input -------
		inline int getCurrentStates()				{ return states; }
		inline bool getCurrentState(int stateCmd)	{ return static_cast<bool>((1 >> stateCmd) & getCurrentStates()); }

		// ------- get action cmd input -------
		int getNActionCmdsOnQueue();
		inline bool hasActionCmdOnQueue()			{ return getNActionCmdsOnQueue() > 0; }
		int popActionCmd();
	};


};

#endif
