#ifndef __templatefunctions_h__
#define __templatefunctions_h__

namespace Planet
{
	
	template <class It, class Functor>
	void ForEach(It begin, It end, Functor &fun)
	{
		It &it=begin;
		for(; it!=end; ++it)
		{
			fun(*it);
		}
	}

	template <typename T>
	inline T tmin(T a, T b)			{ return a < b ? a : b; }

	template <typename T>
	inline T tmax(T a, T b)			{ return a > b ? a : b; }

	template <typename T>
	T tmin(T a, T b, T c, T d)
	{
		return tmin(tmin(a, b), tmin(c, d));		
	}

	template <typename T>
	void tcopy(T *from, T *to, size_t n)
	{
		for(size_t i=0; i<n; ++i)
		{
			to[i] = from[i];
		}
	}

	template <typename Pair>
	class DeleteSecond
	{
	public:
		void operator ()(const Pair &pair)		{ delete pair.second; }
	};
};

#endif
