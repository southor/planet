#ifndef __StateCmds_h__
#define __StateCmds_h__

namespace Planet
{

	class StateCmds
	{
	public:
		StateCmds()	: states(0) {}
		StateCmds(int states) : states(states) {}
	
		bool getCurrentState(int stateCmd);
		void setCurrentState(int stateCmd, bool state);

		//int getStates()	{ return states; }

		StateCmds operator |(StateCmds rh) const		{ return StateCmds(states | rh.states); }
		StateCmds operator &(StateCmds rh) const		{ return StateCmds(states & rh.states); }
		StateCmds operator +(StateCmds rh) const		{ return StateCmds(states + rh.states); }
	
	private:
		int states;
	};
};

#endif