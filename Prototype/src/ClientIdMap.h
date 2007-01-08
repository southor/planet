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
			size_t mapsize = ClientIdMap::map.size();
			if (id >= mapsize) ClientIdMap::map.resize(id+1);
			++ClientIdMap::size;
			assert(!ClientIdMap::map[id].used); //TODO throw			
			ClientIdMap::map[id] = typename ClientIdMap::Entry(typename ClientIdMap::Pair(id, item));
		}

		/**
		 * @param id The id to be freed.
		 * @return Returns false if id is already removed, otherwise true.
		 */
		bool remove(Id id)
		{
			typename ClientIdMap::Entry &entry = ClientIdMap::map[id];
			if (entry.used)
			{
				entry.used = false;
				--ClientIdMap::size;
				return true;
			}
			return false;
		}
		
	};
};

#endif