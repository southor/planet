#ifndef __ConfigHandler_h__
#define __ConfigHandler_h__

#include <string>
#include <map>

namespace Prototype
{
	class ConfigHandler
	{
	public:
		void loadFile(std::string filename);

		double getDoubleValue(std::string option, double defaultValue)
		{
			double value;
			return getValue(option, value) ? value : defaultValue;
		}
		
		inline float getFloatValue(std::string option, float defaultValue)
		{
			double value;
			return getValue(option, value) ? static_cast<float>(value) : defaultValue;
		}
			
		inline int getIntValue(std::string option, int defaultValue)
		{
			double value;
			return getValue(option, value) ? static_cast<int>(value + 0.5) : defaultValue;
		}
		
	private:

		/**
		 * If option has a value, the value will be written to result.
		 * @return true if option has a value.
		 */
		bool getValue(std::string option, double &result);

		std::map<std::string, double> configMap;
	
	};
};

#endif
