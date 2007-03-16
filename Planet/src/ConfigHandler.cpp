#include "ConfigHandler.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace Planet
{
	void ConfigHandler::loadFile(std::string fileName)
	{
		std::ifstream file(fileName.c_str());

		std::string line;
		std::string token;

		float value;

		while (std::getline(file, line)) 
		{
			std::istringstream lineStream(line);			
			lineStream >> token;
			lineStream >> value;

			configMap[token] = value;
		}
	}
	
	float ConfigHandler::getValue(std::string option)
	{
		if (configMap.count(option) > 0)
		{
			return configMap[option];
		}
		else
		{
			printf("Can't load option: %s\n", option.c_str());
			return 0.0f;
		}
	}
};
