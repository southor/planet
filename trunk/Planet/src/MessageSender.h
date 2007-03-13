#ifndef __MessageSender_h__
#define __MessageSender_h__

#include "Message.h"

namespace Planet
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
		
		// Debug purpose. Shows which message reciever this is.
		int v;
	};
};

#endif
