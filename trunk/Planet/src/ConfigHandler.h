#ifndef __ConfigHandler_h__
#define __ConfigHandler_h__

#include <string>
#include <map>

namespace Planet
{
	class ConfigHandler
	{
	public:
		void loadFile(std::string filename);
		
		float getValue(std::string option);
		
	private:	
		std::map<std::string, float> configMap;
	
	};
};

#endif
