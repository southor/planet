#ifndef __historylist_inl__
#define __historylist_inl__

#include "HistoryList.h"
#include <iostream>

namespace Prototype
{

	//template <typename Data, typename Host>
	//HistoryList<Data, Host>::HistoryList(size_t size)
	//	: size(size), nextTick(1), data(0), interExtraPolate(0), hostInterExtraPolate(0), hostObj(0)
	//{
	//	assert(size >= 1);
	//	data = new Data[size];
	//	Data defaultData;
	//	setDefaultData(defaultData);
	//	
	//	//assert(isConsistent());
	//}

	template <typename Data, typename Host>
	HistoryList<Data, Host>::HistoryList(size_t size, InterExtraPolateFun interExtraPolate)
		: size(size), nextTick(1), data(0), interExtraPolate(interExtraPolate), hostInterExtraPolate(0), hostObj(0)
	{
		
		//if (interExtraPolate) this->interExtraPolate = interExtraPolate;
		//else this->interExtraPolate = standardInterExtraPolate<Data>;

		assert(size >= 1);
		data = new Data[size];
		Data defaultData;
		setDefaultData(defaultData);		

		//assert(isConsistent());
	}

	template <typename Data, typename Host>
	HistoryList<Data, Host>::HistoryList(size_t size, HostInterExtraPolateFun hostInterExtrapolate, Host hostObj)
		: size(size), nextTick(1), data(0), interExtraPolate(0), hostInterExtraPolate(hostInterExtrapolate), hostObj(hostObj)
	{
		assert(size >= 1);		
		data = new Data[size];
		Data defaultData;
		setDefaultData(defaultData);
	}

	//template <typename Data, typename S>
	//HistoryList<Data, S>::HistoryList(size_t size, const T &defaultData, InterExtraPolateFun interExtraPolate)
	//	: size(size), nextTick(1), data(0), interExtraPolate(interExtraPolate)
	//{
	//	assert(size >= 1);		
	//	data = new Data[size];
	//	setDefaultData(defaultData);
	//}

	template <typename Data, typename Host>
	HistoryList<Data, Host>::~HistoryList()
	{
		delete[] data;
	}

	template <typename Data, typename Host>
	int HistoryList<Data, Host>::firstTick()
	{
		int tmp = nextTick - static_cast<int>(size);
		if (tmp > 0) return tmp % static_cast<int>(size);
		else return 0;		
	}

	//template <typename Data, typename Host>
	//void HistoryList<Data, Host>::localInterExtraPolate(int tick1, const Data& data1, int tick2, const Data& data2, Tickf resultTick, Data& resultData)
	//{		
	//	if (interExtraPolate) // specific, no host
	//	{
	//		interExtraPolate(tick1, data1, tick2, data2, resultTick, resultData);
	//	}
	//	else if (hostInterExtraPolate) // specific, host
	//	{
	//		assert(hostObj);
	//		hostInterExtraPolate(tick1, data1, tick2, data2, resultTick, resultData, hostObj);
	//	}
	//	else // standard, no host
	//	{
	//		standardInterExtraPolate(tick1, data1, tick2, data2, resultTick, resultData);
	//	}
	//}

	template <typename Data, typename Host>
	//const T& HistoryList<T>::getData(int tick)
	void HistoryList<Data, Host>::getData(int tick, Data &data)
	{
		int firstTickTmp = firstTick();
		
		if (tick >= nextTick)
		{
			// extrapolate

			int tick1 = tmax(firstTickTmp, nextTick - 2);
			int tick2 = tmax(firstTickTmp, nextTick - 1);

			//float distFromTick1 = tick - tick1;
			//T::extrapolate(tickToDataRef(tick1), tickToDataRef(tick2), distFromTick1, data);

			//if (tick1 == tick2) 
			//{
			//	// avoiding division by zero
			//	data = tickToDataRef(tick1);
			//	//return tickToDataRef(tick1); 
			//}
			//else
			//{
			//	


			if (interExtraPolate) // no host, ordinary interExtraPolation
			{
				interExtraPolate(tick1, tickToDataRef(tick1), tick2, tickToDataRef(tick2), static_cast<Tickf>(tick), data);
			}
			else // host logic, only tick1 used
			{
				assert(hostInterExtraPolate);
				assert(hostObj);
				hostInterExtraPolate(tick1, tickToDataRef(tick1), static_cast<Tickf>(tick), data, hostObj);
			}
			//localInterExtraPolate(tick1, tickToDataRef(tick1), tick2, tickToDataRef(tick2), static_cast<Tickf>(tick), data);
				
				

				
			//}
		}
		else if (tick < firstTickTmp)
		{
			if (interExtraPolate) // no host
			{
			
				// use first stored value
				data = tickToDataRef(firstTickTmp);
			
				if (tick != 0) std::cout << "first stored history data was used, diff: " << firstTickTmp - tick << std::endl;
			}
			else
			{
				assert(hostInterExtraPolate);
				assert(hostObj);
				hostInterExtraPolate(firstTickTmp, tickToDataRef(firstTickTmp), static_cast<Tickf>(tick), data, hostObj);
			}
		}
		else
		{
			// just get data
			data = tickToDataRef(tick);
			//return tickToDataRef(tick);
		}
	}

	template <typename Data, typename Host>
	void HistoryList<Data, Host>::getData(Tickf tick, Data &data)
	{
		// This code Will be unessary ineffective at extrapolation
		// but will work fine with interpolation.
		
		// data tick1
		int tick1 = static_cast<int>(tick);
		Data tick1Data;
		getData(tick1, tick1Data);

		if (interExtraPolate) // no host, ordinary interExtraPolation
		{
			// data tick2
			int tick2 = tick1 + 1;
			Data tick2Data;
			getData(tick2, tick2Data);

			interExtraPolate(tick1, tick1Data, tick2, tick2Data, tick, data);
		}
		else // host logic, only data1 needed
		{
			assert(hostInterExtraPolate);
			assert(hostObj);
			hostInterExtraPolate(tick1, tick1Data, tick, data, hostObj);
		}
		//localInterExtraPolate(tick1, tick1Data, tick2, tick2Data, tick, data);
		
	}

	template <typename Data, typename Host>
	void HistoryList<Data, Host>::setData(int tick, const Data &data)
	{		
		assert(isConsistent());
		
		if (tick <= nextTick)
		{
			if (tick == nextTick) ++nextTick;
			else if (tick < firstTick()) return; // log when this happends for debugging?
			
			tickToDataRef(tick) = data;
		}
		else // tick > nextTick
		{
			// current last tick
			int tmpDataTick = nextTick-1; 

			// update nextTick
			nextTick = tick + 1;

			// calculate the start of the filling process
			int startFillTick = tmax(firstTick(), tmpDataTick) + 1;
			
			// Fill in the missing datas
			if (interExtraPolate) // no host, normal interpolation
			{
				// store the current last data in case we overwrite it
				Data tmpData(tickToDataRef(tmpDataTick));
				
				// fill in all missing datas by interpolating
				for(int fillTick = startFillTick; fillTick < tick; ++fillTick)
				{
					interExtraPolate(tmpDataTick, tmpData, tick, data, static_cast<Tickf>(fillTick), tickToDataRef(fillTick));
				}
			}
			else // with host
			{
				assert(hostInterExtraPolate);
				assert(hostObj);

				// fill in the missing datas using host logic
				for(int fillTick = startFillTick; fillTick < tick; ++fillTick)
				{
					hostInterExtraPolate(tick, data, static_cast<Tickf>(fillTick), tickToDataRef(fillTick), hostObj);
				}
			}

			// write new data
			tickToDataRef(tick) = data;



			//for(int fillTick = startFillTick; fillTick < tick; ++fillTick)
			//{
			//	assert(isConsistent());
			//	localInterExtraPolate(tmpDataTick, tmpData, tick, data, static_cast<Tickf>(fillTick), tickToDataRef(fillTick));
			//}
		}
	}

	template <typename Data, typename Host>
	bool HistoryList<Data, Host>::isConsistent()
	{
		//Data testData;
		//localInterExtraPolate(nextTick-2, tickToDataRef(nextTick-2), nextTick-1, tickToDataRef(nextTick-1), nextTick, testData);
		return true;
	}
};

#endif
