#ifndef __historylist_h__
#define __historylist_h__

#include "assert.h"
#include "basic.h"
#include "StandardInterExtraPolate.h"

namespace Prototype
{	

	// template argument Id must be a number type.
	template <typename Data, typename Host = void*>
	class HistoryList
	{
	public:

		// uses 2 datas from different ticks to calculate a resulting data for any tick.
		typedef void (*InterExtraPolateFun)(int tick1, const Data& data1, int tick2, const Data& data2, Tickf resultTick, Data& resultData);

		typedef void (*HostInterExtraPolateFun)(int tick, const Data& data, Tickf resultTick, Data& resultData, Host hostObj);

		Data *data; // data array
		size_t size;
		int nextTick;

		int firstTick();

	private:

		inline Data& tickToDataRef(int tick)
		{
			return data[tick % size];
		}

		InterExtraPolateFun interExtraPolate;
		HostInterExtraPolateFun hostInterExtraPolate;
		Host hostObj;

	public:

		
		HistoryList(size_t size, InterExtraPolateFun interExtrapolate = standardInterExtraPolate);

		HistoryList(size_t size, HostInterExtraPolateFun hostInterExtraPolate, Host hostObj);

		~HistoryList();


		void getData(int tick, Data &data);
		void getData(Tickf tick, Data &data);
		

		void setDefaultData(const Data &defaultData)
		{
			assert(defaultData.isConsistent());
			tickToDataRef(0) = defaultData;
		}

		void setData(int tick, const Data &data);

		int getLastTick()									{ return nextTick - 1; }

		bool isConsistent();
		bool isConsistentRec();

	};
};

#endif
