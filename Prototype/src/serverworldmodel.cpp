#include "serverworldmodel.h"
#include <algorithm>

namespace Prototype
{
	ServerWorldModel::~ServerWorldModel()			
	{
		deleteAllObjs();
	}
};