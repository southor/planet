#ifndef __historylist_h__
#define __historylist_h__

#include "assert.h"
#include "basic.h"

namespace Prototype
{
	// template argument Id must be a number type.
	template <typename T>
	class HistoryList
	{
	private:
		T *data; // data array
		size_t size;
		
		int nextTick;

		inline int firstTick()				{ return tmax(0, nextTick - static_cast<int>(size)); }
		//inline int lastTick()				{ return nextTick-1; }

		inline T& tickToDataRef(int tick)	{ return data[tick % size]; }

		// Helper function, uses 2 datas from different ticks to calculate a resulting data for any tick.
		static void interExtraPolate(int tick1, const T& data1, int tick2, const T& data2, Tickf resultTick, T& resultData);

	public:
		
		HistoryList(size_t size);

		~HistoryList();

		
		const T& getData(int tick);
		//inline void getData(int tick, T &data)		{ data = getDataRef(tick); }
		void getData(Tickf tick, T &data);

		void setData(int tick, const T &data);

	};
};

#endif
