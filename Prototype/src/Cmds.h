#ifndef __Cmds_h__
#define __Cmds_h__

namespace Prototype
{
	namespace Cmds
	{
		// State Commands
		enum
		{
			// Used by server and client
			LEFT = 0,
			RIGHT,
			FORWARD,
			BACKWARD,
			
			// Only used by client
			ROTATE_LEFT,
			ROTATE_RIGHT,
		};

		//Action commands
		enum
		{			
			SWITCH_WEAPON = 0,
			START_SHOOTING,
			STOP_SHOOTING
		};

		static const int N_STATE_CMDS = 6;
		static const int N_ACTION_CMDS = 3;

	};
};


#endif
