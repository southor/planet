#include "ConfigHandler.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "assert.h"

namespace Prototype
{
	void ConfigHandler::loadFile(std::string fileName)
	{
		std::ifstream file(fileName.c_str());

		std::string line;
		std::string token;

		double value;

		while (std::getline(file, line)) 
		{
			std::istringstream lineStream(line);			
			lineStream >> token;
			lineStream >> value;

			configMap[token] = value;
		}
	}
	
	//double ConfigHandler::getDoubleValue(std::string option)
	//{
	//	if (configMap.count(option) > 0)
	//	{
	//		return configMap[option];
	//	}
	//	else
	//	{
	//		assert(false);
	//		printf("Can't load option: %s\n", option.c_str());
	//		return 0.0f;
	//	}
	//}

	bool ConfigHandler::getValue(std::string option, double &result) const
	{
		if (configMap.count(option) > 0)
		{
			//result = configMap[option];
			result = configMap.find(option)->second;
			return true;
		}
		else
		{
			//assert(false);
			//printf("Can't load option: %s\n", option.c_str());
			//std::cout << "config option " << option << " missing in file, using default value." << std::endl; 
			return false;
		}
	}

	double ConfigHandler::getDoubleValue(std::string option) const
	{
		double value = 0.0;
		if(!getValue(option, value)) assert(false); // TODO: in release mode: throw exception ?
		return value;
	}
};
