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
		


		double getDoubleValue(std::string option);
		
		inline float getFloatValue(std::string option)
		{
			return static_cast<float>(getDoubleValue(option));
		}
			
		inline int getIntValue(std::string option)
		{
			return static_cast<int>(getDoubleValue(option) + 0.5);
		}
		
	private:	
		std::map<std::string, double> configMap;
	
	};
};

#endif
