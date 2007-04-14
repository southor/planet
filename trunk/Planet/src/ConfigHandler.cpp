#include "ConfigHandler.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "assert.h"

namespace Planet
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
		
		file.close();
	}
	
	bool ConfigHandler::getValue(std::string option, double &result)
	{
		if (configMap.count(option) > 0)
		{
			result = configMap[option];
			return true;
		}
		else
		{
			std::cout << "config option " << option << " missing in file, using default value." << std::endl; 
			return false;
		}
	}

	double ConfigHandler::getDoubleValue(std::string option)
	{
		double value = 0.0;
		if(!getValue(option, value)) assert(false); // TODO: in release mode: throw exception ?
		return value;
	}
};
