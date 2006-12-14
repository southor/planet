#ifndef __virtualconnection_h__
#define __virtualconnection_h__

#include "messages.h"

namespace Prototype
{
	class VirtualConnection
	{
	protected:
		
		class VirtualMessageSender : public MessageSender
		{};
		
		class VirtualMessageReciever : public MessageReciever
		{};

	public:
		VirtualConnection();

		MessageSender* getMessageSender();
		MessageReciever* getMessageReciever();
	};
};

#endif