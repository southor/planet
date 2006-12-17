#ifndef __timehandler_h__
#define __timehandler_h__

namespace Prototype
{
	class TimeHandler
	{
	public:
		TimeHandler() : startTime(0) { }

		int getTime()
		{
			return SDL_GetTicks() - startTime;
		}
		
		void setStartTime(int startTime) 
		{ 
			this->startTime = startTime;
		}
		
	private:
		int startTime;
	};
};
#endif