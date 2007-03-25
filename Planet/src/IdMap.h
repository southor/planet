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

		void add(Id id, T item)				{ Pair pair(id, item);
											  bool result = (map.insert(pair)).second;
											  assert(result); }
		bool remove(Id id)					{ bool removed = (map.erase(id) > 0);
											  return removed; }

		T& operator[](Id id)
		{
			std::map<Id, T>::iterator it = find(id);
			if (it == map.end())
			{
				assert(false);
				throw "error! in IdMap::operator[] ";
			}
			else
			{
				return it->second;
			}
		}

		bool isConsistent()					{ return true; }
	};
};

#endif
