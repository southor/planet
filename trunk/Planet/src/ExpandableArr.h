#ifndef _ExpandableArr_h_
#define _ExpandableArr_h_

#include "basic.h"

namespace Planet
{

	class ExpandableArr
	{
		
	protected:
		
		float growFactor;

		size_t capacity;

		size_t size;

		/**
		 * @return The new capacity
		 */
		size_t calcNewCapacity(size_t newSize) const
		{
			return static_cast<size_t>(static_cast<float>(newSize)*growFactor) + 1;
		}

		/**
		 * Assumes only one new element was added.
		 * Makes use of the current capacity.
		 * @return The new capacity.
		 */
		inline size_t calcNewCapacity() const
		{
			return calcNewCapacity(capacity + 1);
		}
		
		/**
		 * Grow contents, should grow all arrays
		 */
		virtual void growContents(size_t newCapacity) = 0;


	public:

		static const float DEFAULT_GROW_FACTOR;

		ExpandableArr(size_t initCapacity, float growFactor = DEFAULT_GROW_FACTOR)
			: growFactor(growFactor), capacity(initCapacity), size(0)
		{
		}

		virtual ~ExpandableArr()
		{
		}

		inline size_t getSize() const 							{ return size; }

		inline float getGrowFactor() const						{ return growFactor; }

		inline void setGrowFactor(float growFactor = 2.0f)		{ assert(growFactor >= 0.0f);
																  this->growFactor = growFactor; }

		//debug
		bool isConsistent() const
		{
			if (growFactor < 0.0f) return false;
			if (size > capacity) return false;

			return true;
		}

	};

};

#endif