#ifndef _ExternData_inl_
#define _ExternData_inl_

#include "ExternData.h"
#include "basic.h"

namespace Planet
{
	template <typename T1, typename T2>
	Extern2Data<T1, T2>::Extern2Data(size_t initCapacity, float growFactor) : ExpandableArr(tmax(initCapacity, static_cast<size_t>(1)), growFactor)
	{
		data1Arr = allocData1Arr(capacity);
		data2Arr = allocData2Arr(capacity);
		accesserRefArr = allocAccesserRefArr(capacity);
	}

	template <typename T1, typename T2>
	Extern2Data<T1, T2>::~Extern2Data()
	{
		clear();
		
		delete[] data1Arr;
		delete[] data2Arr;
		delete[] accesserRefArr;
	}

	template <typename T1, typename T2>
	void Extern2Data<T1, T2>::add(Accesser *accesser)
	{
		assert(accesser->isConsistent());
		assert(!accesser->isMember()); // must remove from old first!
		
		size_t arrPos = size;

		if (size >= capacity)
		{
			capacity = this->calcNewCapacity();
			growContents(capacity);
		}

		accesserRefArr[arrPos] = accesser;
		accesser->arrPos = arrPos;
		accesser->extern2Data = this;

		++size;

		// in case accesser whants to something immidietly after add.
		accesser->added();
	}

	template <typename T1, typename T2>
	void Extern2Data<T1, T2>::remove(Accesser *accesser)
	{
		// in case accesser whants to something before remove
		accesser->removed();
		
		assert(accesser->isConsistent());
		assert(accesser->extern2Data == this);
		
		size_t arrPos = accesser->arrPos;

		if (arrPos < size-1)
		{
			// arrayPos is not the last position in the array
			// so last primitive needs to be moved

			// get last Accesser
			Accesser *lastAccesser = accesserRefArr[size-1];

			// update the accesser about the movement
			lastAccesser->arrPos = arrPos;
			
			// perform the move of everything for this index
			moveContents(size-1, arrPos);
		}

		--size;
	}

	template <typename T1, typename T2>
	void Extern2Data<T1, T2>::clear()
	{
		for(size_t i=size-1; i>=0; --i)
		{
			remove(accesserRefArr[i]);
		}
	}



	template <typename T1, typename T2>
	void Extern2Data<T1, T2>::moveContents(size_t fromArrPos, size_t toArrPos)
	{
		data1Arr[toArrPos] = data1Arr[fromArrPos];
		data2Arr[toArrPos] = data2Arr[fromArrPos];
		accesserRefArr[toArrPos] = accesserRefArr[fromArrPos];
	}

	template <typename T1, typename T2>
	void  Extern2Data<T1, T2>::growContents(size_t newCapacity)
	{
		T1 *newData1Arr = allocData1Arr(newCapacity);
		T2 *newData2Arr = allocData2Arr(newCapacity);
		Accesser **newAccesserRefArr = allocAccesserRefArr(newCapacity);

		tcopy(data1Arr, newData1Arr, size);
		tcopy(data2Arr, newData2Arr, size);
		tcopy(accesserRefArr, newAccesserRefArr, size);

		delete[] data1Arr;
		delete[] data2Arr;
		delete[] accesserRefArr;

		data1Arr = newData1Arr;
		data2Arr = newData2Arr;
		accesserRefArr = newAccesserRefArr;
	}

};

#endif