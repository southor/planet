#ifndef __clienttimehandler_h__
#define __clienttimehandler_h__

#include "TimeHandler.h"
#include "basic.h"


namespace Planet
{
	class ClientTimeHandler : public TimeHandler
	{
	public:

		ClientTimeHandler() : TimeHandler(), stepTick(0), stepTickf(0.0f), newTick(false)//, deltaTime(1), stepTime(0), 
		{
			setTick0Time(0);
		}		
		inline Tickf getStepTick()				{ return stepTickf; }
		int getTickTime();

		// returns true only once for every new Tick
		bool isNewTick();											  

		void nextStep();


		// ------------- Tick0Time ---------------
		inline int getTick0Time()				{ return tick0TimeBufferSum / TICK0TIME_BUFFER_SIZE; }
		void setTick0Time(int tick0Time);
		void enterTick0Time(int tick0Time);



		
		
	private:
		Tickf stepTickf;
		int stepTick;
		bool newTick;

		
		static const int TICK0TIME_BUFFER_TIME = 1000;
		static const int TICK0TIME_BUFFER_SIZE = TICK0TIME_BUFFER_TIME / TICK_DELTA_TIME;

		int tick0TimeBufferSum;
		int currTimeBufferPos;
		int tick0TimeBuffer[TICK0TIME_BUFFER_SIZE];
		
	};

	static const size_t CLIENT_INTERPOOLATION_N_HISTORY_TICKS = 10;
	static const size_t CLIENT_PREDICTION_N_HISTORY_TICKS = static_cast<size_t>(MAX_CLIENT_TO_SERVER_LAG / TimeHandler::TICK_DELTA_TIME);

};
#endif
