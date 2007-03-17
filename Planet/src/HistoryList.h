#ifndef __historylist_h__
#define __historylist_h__

#include "assert.h"
#include "basic.h"
#include "StandardInterExtraPolate.h"

namespace Planet
{	

	// template argument Id must be a number type.
	template <typename Data, typename Host = void*>
	class HistoryList
	{
	public:

		// uses 2 datas from different ticks to calculate a resulting data for any tick.
		typedef void (*InterExtraPolateFun)(int tick1, const Data& data1, int tick2, const Data& data2, Tickf resultTick, Data& resultData);

		typedef void (*HostInterExtraPolateFun)(int tick, const Data& data, Tickf resultTick, Data& resultData, Host hostObj);

	private:

		//void ignoreHostInterExtraPolate(int tick1, const Data& data1, int tick2, const Data& data2, Tickf resultTick, Data& resultData, Host *hostObj);
		

		Data *data; // data array
		size_t size;
		
		int nextTick;
		

		int firstTick();
		//inline int lastTick()					{ return nextTick-1; }

		inline Data& tickToDataRef(int tick)	{ return data[tick % size]; }

		
		// Helper function, uses 2 datas from different ticks to calculate a resulting data for any tick.
		//static void standardInterExtraPolate(int tick1, const T& data1, int tick2, const T& data2, Tickf resultTick, T& resultData);

		InterExtraPolateFun interExtraPolate;
		HostInterExtraPolateFun hostInterExtraPolate;
		Host hostObj;

		//void localInterExtraPolate(int tick1, const Data& data1, int tick2, const Data& data2, Tickf resultTick, Data& resultData);

	public:

		//HistoryList(size_t size, InterExtraPolateFun interExtrapolate, const Data &defaultData = Data());
		
		HistoryList(size_t size, InterExtraPolateFun interExtrapolate = standardInterExtraPolate);

		HistoryList(size_t size, HostInterExtraPolateFun hostInterExtraPolate, Host hostObj);

		~HistoryList();

		
		//const T& getData(int tick);
		void getData(int tick, Data &data);
		//inline void getData(int tick, T &data)			{ data = getDataRef(tick); }
		void getData(Tickf tick, Data &data);
		

		void setDefaultData(const Data &defaultData)		{ tickToDataRef(0) = defaultData; }
		void setData(int tick, const Data &data);

		int getLastTick()									{ return nextTick - 1; }

		bool isConsistent();

	};
};

#endif
