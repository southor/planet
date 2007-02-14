#ifndef __serverglobalaccess_h__
#define __serverglobalaccess_h__

#include "ServerTimeHandler.h"

namespace Prototype
{
	class ServerGlobalObj
	{
	public:
		ServerTimeHandler timeHandler;
	};
	
	
	class ServerGlobalAccess
	{
	private:

		ServerGlobalObj *serverGlobalObj;

	public:
		
		ServerGlobalAccess(ServerGlobalObj *serverGlobalObj)
			: serverGlobalObj(serverGlobalObj)
		{}

		inline ServerTimeHandler* getTimeHandler()
		{
			assert(serverGlobalObj);
			return &(serverGlobalObj->timeHandler);
		}

	};
};

#endif
