#ifndef __StateCmds_h__
#define __StateCmds_h__

namespace Prototype
{

	class StateCmds
	{
	public:
		StateCmds()	: states(0) {}
		StateCmds(int states) : states(states) {}
	
		bool getCurrentState(int stateCmd);
		void setCurrentState(int stateCmd, bool state);

		int getStates()	{ return states; }
	
	private:
		int states;
	};
};

#endif