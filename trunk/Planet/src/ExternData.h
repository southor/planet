#ifndef _ExternData_h_
#define _ExternData_h_

#include "ExpandableArr.h"

namespace Planet
{
	
	template <typename T1, typename T2>
	class Extern2Data;

	template <typename T1, typename T2>
	class Extern2Data : public ExpandableArr
	{
	protected:
		
		size_t nDatas;
		
		T1 *data1Arr;
		T2 *data2Arr;


		T1 *allocData1Arr(size_t newCapacity)			{ return new T1[newCapacity]; }
		T2 *allocData2Arr(size_t newCapacity)			{ return new T2[newCapacity]; }

		inline T1 &data1(size_t arrPos) const			{ assert(arrPos < nDatas);
														  return data1Arr[arrPos]; }

		inline T2 &data2(size_t arrPos) const			{ assert(arrPos < nDatas);
														  return data2Arr[arrPos]; }

		void moveContents(size_t fromArrPos, size_t toArrPos);

	public:	
		
		class Accesser
		{
		private:
			Extern2Data<T1, T2> *extern2Data;
			size_t arrPos;
		
		protected:
			
			// called when the object was added to a extern2data
			virtual void added()					{}

			// called when the object is about to be removed from an extern2data
			virtual void removed()					{}

		public:

			Accesser() : extern2Data(NULL), arrPos(0)	{}

			virtual ~Accesser()						{ if (isMember()) extern2Data->remove(this); }

			inline bool isMember() const			{ return extern2Data != NULL; }

			Extern2Data<T1, T2> *getExtern2Data()	{ return extern2Data; }


			inline T1 &data1()						{ assert(isMember());
													  return extern2Data->data1(arrPos); }

			inline T2 &data2()						{ assert(isMember());
													  return extern2Data->data2(arrPos); }




			//debug
			bool isConsistent() const
			{
				if (isMember()) return arrPos < extern2Data->getSize();
				else return true;				
			}

			friend Extern2Data<T1, T2>;

		};

		Extern2Data(size_t initCapacity = 0, float growFactor = DEFAULT_GROW_FACTOR);

		~Extern2Data();



		
		inline const T1 *getData1Arr() const		{ return data1Arr; }

		inline const T2 *getData2Arr() const		{ return data2Arr; }

		void add(Accesser *accesser);

		void remove(Accesser *accesser);

		// removes all accessers
		void clear();

		//debug
		bool isConsistent() const
		{
			if (!ExpandableArr::isConsistent()) return false;
			
			if (capacity < 1) return false;
			if (!data1Arr || !data2Arr || !accesserRefArr) return false;

			return true;
		}

	protected:

		Accesser **accesserRefArr;

		Accesser **allocAccesserRefArr(size_t newCapacity) const	{ return new Accesser*[newCapacity]; }	

		virtual void growContents(size_t newCapacity);


	};

};

#endif