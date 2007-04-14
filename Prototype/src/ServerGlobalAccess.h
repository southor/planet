#ifndef __serverglobalaccess_h__
#define __serverglobalaccess_h__

#include "ServerTimeHandler.h"
#include "IdGenerator.h"

namespace Prototype
{
	class ServerGlobalObj
	{
	public:
		ServerTimeHandler timeHandler;
		ServerIdGenerator idGenerator;
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

		inline ServerIdGenerator* getIdGenerator()
		{
			assert(serverGlobalObj);
			return &(serverGlobalObj->idGenerator);
		}

	};
};

#endif
