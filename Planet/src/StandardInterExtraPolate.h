#ifndef __StandardInterExtraPolate_h__
#define __StandardInterExtraPolate_h__

namespace Planet
{
	template <typename T>
	void standardInterExtraPolate(int tick1, const T& data1, int tick2, const T& data2, Tickf resultTick, T& resultData)
	{
		// avoiding division by zero
		if (tick1 == tick2)
		{
			resultData = data2;
		}
		else
		{
			Tickf totalDiff = static_cast<Tickf>(tick2) - static_cast<Tickf>(tick1);
			Tickf resultDiff = resultTick - static_cast<Tickf>(tick1);
			float tValue = static_cast<float>(resultDiff) / static_cast<float>(totalDiff);

			resultData = data1 + (data2 - data1) * tValue;
		}
	}
};

#endif
