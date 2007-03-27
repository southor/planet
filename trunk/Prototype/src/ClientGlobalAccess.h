#ifndef __clientglobalaccess_h__
#define __clientglobalaccess_h__

#include "ClientTimeHandler.h"
#include "IdGenerator.h"

namespace Prototype
{
	class ClientGlobalObj
	{
	public:
		ClientTimeHandler timeHandler;
		ClientIdGenerator idGenerator;		
	};
	
	class ClientGlobalAccess
	{
	private:

		ClientGlobalObj *clientGlobalObj;

	public:
		
		ClientGlobalAccess(ClientGlobalObj *clientGlobalObj)
			: clientGlobalObj(clientGlobalObj)
		{}

		inline ClientTimeHandler* getTimeHandler()
		{
			assert(clientGlobalObj);
			return &(clientGlobalObj->timeHandler);
		}

		inline const ClientTimeHandler* getTimeHandler() const
		{
			assert(clientGlobalObj);
			return &(clientGlobalObj->timeHandler);
		}

		inline ClientIdGenerator* getIdGenerator()
		{
			assert(clientGlobalObj);
			return &(clientGlobalObj->idGenerator);
		}

		inline Tickf getStepTick()
		{			
			return getTimeHandler()->getStepTick();
		}

	};
};

#endif
