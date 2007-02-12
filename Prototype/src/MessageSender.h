#ifndef __messagesender_h__
#define __messagesender_h__

//#include <vector>

#include "Message.h"


namespace Prototype
{
	class MessageSender
	{
	public:
		MessageSender()					{}
		virtual ~MessageSender()		{}
		
		virtual void pushMessage(const Message &message) = 0;

		// Get number of messages queued to be transmitted.
		virtual int getNMessages() = 0;

		// Transmits queued messages.
		virtual void transmit() = 0;
	};
};

#endif
