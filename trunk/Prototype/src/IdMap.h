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
	public:

		struct Pair
		{
			// Same names as maps in STL
			Id first;
			T second;

			Pair()			{}
			Pair(Id first, const T &second) : first(first), second(second)
			{}
		};

	protected:

		struct Entry
		{
			bool used;
			//Pair pair;
			mutable Pair pair;
			//T element;

			Entry() : used(false)								{}
			Entry(const Pair &pair) : used(true), pair(pair)	{}
		};

		std::vector<Entry> map;

		// The number of used id's in map.
		size_t size;

		std::vector<Id> freeIds;

		bool isValid(Id id) const			{ return id >= 0 && id < map.size(); }

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
				assert(it.isConsistent());
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


			inline Iterator& operator++()
			{
				assert(standardUniCheck());
				if (!atEnd()) ++i;
				for(;!atEnd() && !(*iterOver)[i].used; ++i);
				return *this;
			}

			inline const Pair& operator*()
			{
				assert(standardUniCheck());
				assert(i >= 0); // is valid index
				return (*iterOver)[i].pair;
			}

			inline const Pair* operator->()
			{
				assert(standardUniCheck());
				assert(i >= 0); // is valid index
				return &((*iterOver)[i].pair);
			}

			//std::forward_iterator_tag iterator_category()	{ return std::forward_iterator_tag; }

			// debug
			bool isConsistent() const		{ return i <= iterOver->size(); }

		};
		
		IdMap() : size(0)
		{}

		inline size_t getSize() const		{ return size; }

		inline Iterator begin()				{ return Iterator(&map, 0); }
		inline Iterator end()				{ return Iterator(&map); }

		//bool itemExists(Id id)			{ bool result = false;
		//									  if (isValid(id)) result = map[id].used;
		//									  return result; }

		Id findFreeId()
		{
			while(freeIds.size() > 0)
			{
				Id id = freeIds.back();
				freeIds.pop_back();
				if (isValid(id))
				{
					if (map[id].used == false) return id;
				}
			}
			Entry unusedEntry;
			map.push_back(unusedEntry);
			
			return map.size()-1;
		}

		void add(Id id, T item)
		{
			Entry entryAdd(Pair(id, item));
			
			if (isValid(id))
			{
				assert(map[id].used == false);
				map[id] = entryAdd;
			}
			else
			{
				assert(id >= map.size());
				
				Entry unusedEntry;
				while(id > map.size())
				{
					freeIds.push_back(map.size());
					map.push_back(unusedEntry);					
				}
				assert(id == map.size());
				map.push_back(entryAdd);
			}
			++size;
		}

		/**
		 * @param id The id to be freed.
		 * @return Returns false if id is already removed, otherwise true.
		 */
		bool remove(Id id)
		{
			if (isValid(id))
			{
				Entry &entry = map[id];
				if (entry.used)
				{					
					if (id == map.size()-1) // is last id?
					{
						map.pop_back();
						
						// cleanup other unused id's if possible
						while(map.size() > 0)
						{
							if (map.back().used) break;
							map.pop_back();
						}
					}
					else
					{
						entry.used = false;
						freeIds.push_back(id);
					}					
					--size;

					return true;
				}
				return false;
			}
			else
			{
				assert(false);
				return false;
			}
		}

		inline T& operator[](Id id)
		{			
			assert(size > 0);
			assert(isValid(id));
			assert(map[id].used);			
			return map[id].pair.second;			
		}

	};
};

#endif