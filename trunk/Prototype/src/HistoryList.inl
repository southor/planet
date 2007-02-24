#ifndef __historylist_inl__
#define __historylist_inl__

#include "HistoryList.h"

namespace Prototype
{
	template <typename T>
	HistoryList<T>::HistoryList(size_t size) : size(size), nextTick(1), data(0)
	{
		assert(size >= 1);
		data = new T[size];
	}

	template <typename T>
	HistoryList<T>::~HistoryList()
	{
		delete[] data;
	}

	template <typename T>
	void HistoryList<T>::interExtraPolate(int tick1, const T& data1, int tick2, const T& data2, Tickf resultTick, T& resultData)
	{
		assert(tick1 != tick2); // would cause a division by zero
		
		Tickf totalDiff = static_cast<Tickf>(tick1) - static_cast<Tickf>(tick2);
		Tickf resultDiff = resultTick - static_cast<Tickf>(tick1);
		float tValue = static_cast<float>(resultDiff) / static_cast<float>(totalDiff);

		resultData = data1 + (data2 - data1) * tValue;
	}

	template <typename T>
	const T& HistoryList<T>::getData(int tick)
	{
		int firstTickTmp = firstTick();
		
		if (tick >= nextTick)
		{
			// extrapolate

			int tick1 = tmax(firstTickTmp, nextTick - 2);
			int tick2 = tmax(firstTickTmp, nextTick - 1);

			//float distFromTick1 = tick - tick1;
			//T::extrapolate(tickToDataRef(tick1), tickToDataRef(tick2), distFromTick1, data);

			if (tick1 == tick2) 
			{
				// avoiding division by zero
				return tickToDataRef(tick1); 
			}
			else
			{
				//T& data = tickToDataRef(tick);
				T data;
				interExtraPolate(tick1, tickToDataRef(tick1), tick2, tickToDataRef(tick2), static_cast<Tickf>(tick), data);
				setData(tick, data);
				return tickToDataRef(tick);
			}
		}
		else if (tick < firstTickTmp)
		{
			// use first stored value
			return tickToDataRef(firstTickTmp);
		}
		else
		{
			// just get data
			return tickToDataRef(tick);
		}
	}

	template <typename T>
	void HistoryList<T>::getData(Tickf tick, T &data)
	{
		int tick1 = static_cast<int>(tick);
		int tick2 = tick1 + 1;

		interExtraPolate(tick1, getData(tick1), tick2, getData(tick2), tick, data);
	}

	template <typename T>
	void HistoryList<T>::setData(int tick, T &data)
	{		
		if (tick <= nextTick)
		{
			if (tick == nextTick) ++nextTick;
			else if (tick < firstTick()) return; // log when this happends for debugging?
			
			tickToDataRef(tick) = data;
		}
		else // tick > nextTick
		{
			// store the current last data in case we overwrite it
			int tmpDataTick = nextTick-1; // current last tick
			T tmpData(tickToDataRef(tmpDataTick));

			// write new data and update nextTick
			tickToDataRef(tick) = data;
			nextTick = tick + 1;

			// fill in all missing datas by interpolating
			int startFillTick = tmax(firstTick(), tmpDataTick) + 1;
			for(int fillTick = startFillTick; fillTick < tick; ++fillTick)
			{
				interExtraPolate(tmpDataTick, tmpData, tick, data, static_cast<Tickf>(fillTick), tickToDataRef(fillTick));
			}
		}

		//if (tick >= nextTick)
		//{
		//	while (tick > nextTick)
		//	{			
		//		//interpolate between 2 values				
		//		
		//		int lastTick = nextTick-1;
		//		
		//		//Tickf totalDist = static_cast<Tickf>(tick) - static_cast<Tickf>(lastTick);
		//		//float rightWeight = 1.0f / static_cast<float>(totalDist);
		//		//float leftWeight = 1.0f - rightWeight;

		//		//T::interpolate(tickToDataRef(lastTick), leftWeight, data, rightWeight, tickToDataRef(nextTick));
		//		
		//		
		//		interExtraPolate(lastTick, tickToDataRef(lastTick), tick,
		//							data, static_cast<Tickf>(nextTick), tickToDataRef(nextTick));
		//		
		//		++nextTick;
		//	}
		//	++nextTick;
		//	
		//	assert(nextTick = tick+1);
		//}

		//tickToDataRef(tick) = data;
	}
};

#endif
