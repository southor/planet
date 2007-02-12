#ifndef __message_h__
#define __message_h__

namespace Prototype
{
	struct Message
	{
		int type;
		int time;
		int tick;
		int size;
		void *data;
		Message() : type(0), time(0), tick(0), size(0), data(0)			{}
		Message(int type, int size, void *data, int time = 0, int tick = 0)
			: type(type), size(size), data(data), time(time), tick(tick)	{}
	};

};

#endif
