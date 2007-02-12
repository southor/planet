#ifndef __UserInputHandler_h__
#define __UserInputHandler_h__

#include "Cmds.h"

namespace Prototype
{
	class UserInputHandler
	{
	public:

		// ------- set State cmd keys and mouse buttons -------
		void setStateCmdKey(int stateCmd, int key); // keyboard key
		void setStateCmdButton(int stateCmd, int button); // mouse button

		// ------- set Action cmd keys and mouse buttons -------
		void setActionCmdKey(int actionCmd, int key); // keyboard key
		void setActionCmdButton(int actionCmd, int button); // mouse button



		// ------- get state cmd input -------
		bool getCurrentState(int stateCmd);

		// ------- get action cmd input -------
		int getNActionCmdsOnQueue();
		inline bool hasActionCmdOnQueue()			{ return getNActionCmdsOnQueue() > 0; }
		int popActionCmd();
	};


};

#endif
