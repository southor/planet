#ifndef __messages_h__
#define __messages_h__

//#include <vector>

#include "basic.h"

namespace Prototype
{
	//class Message
	//{
	//private:
	//	int type;
	//	void *data;

	//	Message& operator =(const Message &message);
	//public:
	//	Message() : type(0), data(NULL)			{}
	//	~Message()								{ if (data != NULL) delete data; }

	//	// @param data This Message will take over the ownership of the data.
	//	void set(int type, void *data)			{ this->type = type;
	//											  if (this->data != NULL) delete data;
	//											  this->data = data; }

	//	int getType() const						{ return type; }
	//	void* getData()	const					{ return data; }		
	//};


	enum MessageTypes
	{
		UPDATE_PLAYER,
		
		USER_CMD	
	};

	struct UpdatePlayerObj
	{
		int playerObjId;
		Pos pos;
		float angle;

		UpdatePlayerObj()		{}
		UpdatePlayerObj(int playerObjId, const Pos &pos, float angle)
			: playerObjId(playerObjId), pos(pos), angle(angle)
		{}
	};

	struct UserCmd
	{
		bool cmd_left;
		bool cmd_right;
		bool cmd_up;
		bool cmd_down;
		bool cmd_shoot;
	};

	struct Message
	{
		int type;
		int time;
		void *data;
		Message() : type(0), time(0), data(0)					{}
		Message(int type, void *data) : type(type), data(data)	{}
	};

	class MessageReciever
	{
	public:
		MessageReciever()				{}
		virtual ~MessageReciever()		{}
		
		// Get number of messages recieved.
		virtual int getNMessages() = 0;		
		
		virtual Message popMessage() = 0;
	};

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

	struct Link
	{
		MessageSender *messageSender;
		MessageReciever *messageReciever;

		Link(MessageSender *messageSender, MessageReciever *messageReciever) 
			: messageSender(messageSender), messageReciever(messageReciever) { }
	};



};

#endif