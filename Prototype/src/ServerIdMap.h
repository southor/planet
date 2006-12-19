#ifndef __serveridmap_h__
#define __serveridmap_h__

#include "IdMap.h"

namespace Prototype
{
	// template argument Id must be a number type.
	template <typename Id, typename T>
	class ServerIdMap : public IdMap<Id, T>
	{
	private:
		
		//std::vector<Entry> map;
		std::vector<Id> freeIds;

	public:
		
		ServerIdMap() : IdMap()
		{}

		Id add(T item)
		{
			Id id;
			if (freeIds.size() > 0)
			{
				id = freeIds.back();
				freeIds.pop_back();
				map[id] = Entry(item);
			}
			else
			{
				id = static_cast<Id>(map.size());
				map.push_back(Entry(item));
			}
			++size;
			return id;
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
				freeIds.push_back(id);
				--size;
				return true;
			}
			return false;
		}	
	};
};

#endif