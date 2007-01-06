#ifndef __clientidmap_h__
#define __clientidmap_h__

#include "IdMap.h"

namespace Prototype
{
	// template argument Id must be a number type.
	template <typename Id, typename T>
	class ClientIdMap : public IdMap<Id, T>
	{
	public:
		
		ClientIdMap() : IdMap<Id, T>()
		{}

		void add(Id id, T item)
		{
			size_t mapsize = map.size();
			if (id >= mapsize) map.resize(id+1);
			assert(!map[id].used); //TODO throw			
			map[id] = Entry(Pair(id, item));
		}

		/**
		 * @param id The id to be freed.
		 * @return Returns false if id is already removed, otherwise true.
		 */
		bool remove(Id id)
		{
			Entry &entry = map[id];
			if (entry.used)
			{
				entry.used = false;
				--size;
				return true;
			}
			return false;
		}
		
	};
};

#endif