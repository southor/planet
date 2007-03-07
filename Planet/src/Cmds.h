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
		};

		//Action commands
		enum
		{
			SHOOT = 0,
			SWITCH_WEAPON
		};

		static const int N_STATE_CMDS = 6;
		static const int N_ACTION_CMDS = 2;

		//static const int N_CMDS = 8; // all commands
		//static const int N_SERVER_STATE_CMDS = 4; // first group of commands
		//static const int N_STATE_CMDS = 6; // first and second group of commands
		//static const int N_ACTION_CMDS = 2; // third group of commands

		//inline bool isCommand(int cmd)
		//{
		//	return (cmd >= 0) && (cmd < N_CMDS);
		//}

		//inline bool isStateCommand(int cmd)
		//{
		//	return (cmd >= 0) && (cmd < N_STATE_CMDS);
		//}

		//inline bool isActionCommand(int cmd)
		//{
		//	cmd -= N_STATE_CMDS;
		//	return (cmd >= 0) && (cmd < N_ACTION_CMDS);
		//}
	};
};


#endif
