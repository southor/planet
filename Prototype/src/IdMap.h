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
		uint size;

	public:

		class Iterator : public C0Iterator<Entry>
		{
		public:

			Iterator() : C0Iterator<Entry>()		{}
		
			Iterator(std::vector<Entry> *iterOver, int i) : C0Iterator<Entry>(iterOver, i)	{}

			// End iterator
			Iterator(std::vector<Entry> *iterOver) : C0Iterator<Entry>(iterOver)			{}

			// Copy constructor
			Iterator(const Iterator &it) : C0Iterator<Entry>(it)							{}

			
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

		};
		
		IdMap() : size(0)
		{}

		inline uint getSize() const		{ return size; }

		inline Iterator begin()			{ return Iterator(&map, 0); }
		inline Iterator end()			{ return Iterator(&map); }

		bool isValid(Id id) const		{ return id >= 0 && id < map.size(); }

		inline T& operator[](Id id)
		{			
			assert(map[id].used);			
			return map[id].element;
		}

	};
};

#endif