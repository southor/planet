#ifndef __VirtualConnection_h__
#define __VirtualConnection_h__

#include <deque>

#include "common.h"
#include "MessageSender.h"
#include "MessageReciever.h"

namespace Planet
{
	class VirtualConnection
	{
	public:
		class VirtualMessageSender : public MessageSender
		{
		public:
			VirtualMessageSender(MessageReciever *messageReciever)
				: MessageSender(), messageReciever(messageReciever)
			{}
			
			~VirtualMessageSender()					{}
		
			void pushMessage(const Message &message);

			// Get number of messages queued to be transmitted.
			int getNMessages();
			
			// Transmits queued messages.
			void transmit();
					
		private:
			MessageReciever *messageReciever;
			std::deque<Message> sendDeque;
		};

	public:
		VirtualConnection();

		MessageSender* getMessageSender();
		MessageReciever* getMessageReciever();

	private:
		VirtualMessageSender virtualMessageSender;
		MessageReciever messageReciever;
	};
};

#endif
