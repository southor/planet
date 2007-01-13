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
		//std::vector<Id> freeIds;

	public:
	
			
		ServerIdMap() : IdMap<Id, T>()
		{}

		Id add(T item)
		{
			//Id id;
			//if (freeIds.size() > 0)
			//{
			//	id = freeIds.back();
			//	freeIds.pop_back();
			//	ServerIdMap::map[id] = typename ServerIdMap::Entry(typename ServerIdMap::Pair(id, item));
			//}
			//else
			//{
			//	id = static_cast<Id>(ServerIdMap::map.size());
			//	ServerIdMap::map.push_back(typename ServerIdMap::Entry(typename ServerIdMap::Pair(id, item)));
			//}
			//++ServerIdMap::size;
			//return id;

			Id id = findFreeId();
			IdMap<Id, T>::add(id, item);
			return id;
		}

		/**
		 * @param id The id to be freed.
		 * @return Returns false if id is already removed, otherwise true.
		 */
/*		bool remove(Id id)
		{
			if (isValied(id))
			{
				typename ServerIdMap::Entry &entry = ServerIdMap::map[id];
				if (entry.used)
				{
					entry.used = false;
					freeIds.push_back(id);
					--ServerIdMap::size;
					return true;
				}
				return false;
			}
			else
			{
				assert(false);
				return false;
			}
		}*/	
	};
};

#endif