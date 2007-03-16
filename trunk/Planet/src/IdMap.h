#ifndef __idmap_h__
#define __idmap_h__

#include <map>

#include "assert.h"

namespace Planet
{
	template <typename Id, typename T>
	class IdMap
	{
	private:
		std::map<Id, T> map;
	public:

		typedef typename std::pair<Id, T> Pair;

		typedef typename std::map<Id, T>::iterator Iterator;

		IdMap()
		{}

		inline size_t getSize() const		{ return map.size(); }

		inline Iterator begin()				{ return map.begin(); }
		inline Iterator end()				{ return map.end(); }

		inline Iterator find(Id id)			{ return map.find(id); }
		inline bool exists(Id id) const		{ return map.find(id) != map.end(); }

		void add(Id id, T item)				{ bool result = (map.insert(Pair(id, item))).second;
											  assert(result); }
		bool remove(Id id)					{ bool removed = (map.erase(id) > 0);
											 // assert(removed);
											  return removed; }

		T& operator[](Id id)
		{
			assert(exists(id));
			return map[id];
		}

		bool isConsistent()					{ return true; }
	};
};

#endif
