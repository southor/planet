#ifndef __ConfigHandler_h__
#define __ConfigHandler_h__

#include <string>
#include <map>

namespace Prototype
{
	class ConfigHandler
	{
	public:
		bool loadFile(std::string filename);
		
		double getDoubleValue(std::string option, double defaultValue) const
		{
			double value;
			return getValue(option, value) ? value : defaultValue;
		}
		
		float getFloatValue(std::string option, float defaultValue) const
		{
			double value;
			return getValue(option, value) ? static_cast<float>(value) : defaultValue;
		}
			
		int getIntValue(std::string option, int defaultValue) const
		{
			double value;
			return getValue(option, value) ? static_cast<int>(value + 0.5) : defaultValue;
		}

		
		double getDoubleValue(std::string option) const;

		inline float getFloatValue(std::string option) const
		{
			return static_cast<float>(getDoubleValue(option));
		}

		inline int getIntValue(std::string option) const
		{
			return static_cast<int>(getDoubleValue(option));
		}

		
	private:

		/**
		 * If option has a value, the value will be written to result.
		 * @return true if option has a value.
		 */
		bool getValue(std::string option, double &result) const;

		std::map<std::string, double> configMap;
	
	};
};

#endif
