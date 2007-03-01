#ifndef __clienttimehandler_h__
#define __clienttimehandler_h__

#include "TimeHandler.h"
#include "basic.h"

namespace Prototype
{
	class ClientTimeHandler : public TimeHandler
	{
	public:

		static const int DELTA_TIME_MAX = 1000/30;

		static const float DELTA_TIME_MAX_F;
		
		//static const int TICKS_PER_SECOND = 50;

		ClientTimeHandler() : TimeHandler(), deltaTime(1),
			stepTime(0), stepTick(0), stepTickf(0.0f), tick0Time(0), newTick(true)
		{}

		//inline int getTime()				{ return SDL_GetTicks() - startTime; }

		inline int getStepTime()			{ return stepTime; }		
		inline Tickf getStepTick()			{ return stepTickf; }
		inline int getTickTime()			{ return tick0Time + stepTick * TICK_DELTA_TIME; }

		// returns true only once for every new Tick
		bool isNewTick();											  

		//int getDeltaTime()				{ return deltaTime; }

		float getDeltaTimef()				{ return static_cast<float>(deltaTime); }
		
		//int getTick();
		
		void reset();

		void nextStep();

		void setTick0Time(int tick0Time)	{ this->tick0Time = tick0Time; }
		
	private:
		//int startTime;
		int stepTime;
		Tickf stepTickf;
		int stepTick;
		int deltaTime;
		int tick0Time;
		bool newTick;
		
	};

	static const size_t CLIENT_INTERPOOLATION_N_HISTORY_TICKS = 10;
	static const size_t CLIENT_PREDICTION_N_HISTORY_TICKS = static_cast<size_t>(MAX_SERVER_TO_CLIENT_LAG / TimeHandler::TICK_DELTA_TIME);

};
#endif
