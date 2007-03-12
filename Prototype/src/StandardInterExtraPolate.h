#ifndef __StandardInterExtraPolate_h__
#define __StandardInterExtraPolate_h__

namespace Prototype
{
	template <typename T>
	void standardInterExtraPolate(int tick1, const T& data1, int tick2, const T& data2, Tickf resultTick, T& resultData)
	{
		assert(tick1 != tick2); // would cause a division by zero
		
		Tickf totalDiff = static_cast<Tickf>(tick2) - static_cast<Tickf>(tick1);
		Tickf resultDiff = resultTick - static_cast<Tickf>(tick1);
		float tValue = static_cast<float>(resultDiff) / static_cast<float>(totalDiff);

		resultData = data1 + (data2 - data1) * tValue;
	}
};

#endif
