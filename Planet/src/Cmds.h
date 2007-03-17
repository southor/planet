#ifndef __Cmds_h__
#define __Cmds_h__

namespace Planet
{
	namespace Cmds
	{

		static const int N_STATE_CMDS = 12;
		static const int N_ACTION_CMDS = 3;

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
			
			START_SERVER,
			CONNECT_TO_SERVER,
			//TOGGLE_MENU,
			
			TMP_LEFT,
			TMP_RIGHT,
			TMP_UP,
			TMP_DOWN,
			TMP_ZOOM_IN,
			TMP_ZOOM_OUT		
		};

		//Action commands
		enum
		{			
			SWITCH_WEAPON = 0,
			START_SHOOTING,
			STOP_SHOOTING,

			// other commands
			TOGGLE_MENU
		};


	};
};

#endif
