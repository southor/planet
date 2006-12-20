#ifndef __idmap_h__
#define __idmap_h__

#include <vector>
#include "assert.h"

namespace Prototype
{
	// template argument Id must be a number type.
	template <typename Id, typename T>
	class IdMap
	{
	protected:
		
		struct Entry
		{
			bool used;
			T element;

			Entry() : used(false)							{}
			Entry(T element) : used(true), element(element) {}
		};
		
		std::vector<Entry> map;

		// The number of used id's in map.
		size_t size;

	public:

		class Iterator
		{
		private:
			std::vector<Entry> *iterOver;		
				
			size_t i;

			//inline typename std::vector<T>::size_type getSize() const
			//{ 
			//	return iterOver->size();
			//}

			inline bool atStart() const		{ return i == 0; }
			
			inline bool atEnd() const		{ return i == iterOver->size(); }

			static bool standardUniCheck(const Iterator &it)
			{
				assert(isConsistent());
				//assert(it.i >= 0); // check if valid index
				return true;
			}

			inline bool standardUniCheck() const 	{ return standardUniCheck(*this); }

			bool standardBiCheck(const Iterator &it) const
			{
				standardUniCheck(*this);
				standardUniCheck(it);			
				assert(it.iterOver == this->iterOver);		
				return true;
			}

		public:

			//inline T& operator*()
			//{
			//	assert(standardUniCheck());
			//	assert(i >= 0); // is valid index
			//	return (*iterOver)[i];
			//}

			//inline T* operator->()
			//{
			//	assert(standardUniCheck());
			//	assert(i >= 0); // is valid index
			//	return &((*iterOver)[i]);
			//}

			Iterator() 			{}
		
			Iterator(std::vector<Entry> *iterOver, size_t i) : i(i), iterOver(iterOver)		{}

			// End iterator
			Iterator(std::vector<Entry> *iterOver)
				: i(static_cast<int>(iterOver->size())), iterOver(iterOver)					{}

			// Copy constructor
			Iterator(const Iterator &it) : iterOver(it.iterOver), i(it.i)
			{ 
				assert(standardUniCheck());
			}
			



			Iterator &operator=(const Iterator &it)
			{
				assert(standardUniCheck(it));
				this->iterOver = it.iterOver;
				this->i = it.i;
				return *this;
			}

			inline bool operator<(const Iterator &it) const
			{
				assert(standardBiCheck(it));
				return i < it.i;
			}

			inline bool operator>(const Iterator &it) const
			{
				assert(standardBiCheck(it));
				return i > it.i;		
			}

			inline bool operator>=(const Iterator &it) const
			{
				assert(standardBiCheck(it));			
				return i >= it.i;		
			}

			inline bool operator<=(const Iterator &it) const
			{
				assert(standardBiCheck(it));
				return i <= it.i;		
			}

			inline bool operator==(const Iterator &it) const
			{
				return this->i == it.i && this->iterOver == it.iterOver;
			}

			inline bool operator!=(const Iterator &it) const
			{
				return this->i != it.i || this->iterOver != it.iterOver;
			}


			inline Iterator &operator++()
			{
				assert(standardUniCheck());
				if (!atEnd()) ++i;
				for(;!atEnd() && !(*iterOver)[i].used; ++i);
				return *this;
			}

			inline T& operator*()
			{
				assert(standardUniCheck());
				assert(i >= 0); // is valid index
				return (*iterOver)[i].element;
			}

			inline T* operator->()
			{
				assert(standardUniCheck());
				assert(i >= 0); // is valid index
				return &((*iterOver)[i].element);
			}

			// debug
			bool isConsistent() const		{ return i <= static_cast<int>(getSize()); }

		};
		
		IdMap() : size(0)
		{}

		inline size_t getSize() const		{ return size; }

		inline Iterator begin()				{ return Iterator(&map, 0); }
		inline Iterator end()				{ return Iterator(&map); }

		bool isValid(Id id) const			{ return id >= 0 && id < map.size(); }

		inline T& operator[](Id id)
		{			
			assert(map[id].used);			
			return map[id].element;
		}

	};
};

#endif