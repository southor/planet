#ifndef __Cmds_h__
#define __Cmds_h__

namespace Planet
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
			
			TMP_LEFT,
			TMP_RIGHT,
			TMP_UP,
			TMP_DOWN
		};

		//Action commands
		enum
		{
			SHOOT = 0,
			SWITCH_WEAPON
		};

		static const int N_STATE_CMDS = 10;
		static const int N_ACTION_CMDS = 2;
	};
};

#endif
