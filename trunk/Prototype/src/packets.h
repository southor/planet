#ifndef __packets_h__
#define __packets_h__

//#include <vector>

//#include "common.h"

namespace Prototype
{
	//class Packet
	//{
	//private:
	//	int type;
	//	void *data;

	//	Packet& operator =(const Packet &packet);
	//public:
	//	Packet() : type(0), data(NULL)			{}
	//	~Packet()								{ if (data != NULL) delete data; }

	//	// @param data This Packet will take over the ownership of the data.
	//	void set(int type, void *data)			{ this->type = type;
	//											  if (this->data != NULL) delete data;
	//											  this->data = data; }

	//	int getType() const						{ return type; }
	//	void* getData()	const					{ return data; }		
	//};

	struct Packet
	{
		int type;
		void *data;
		Packet() : type(0), data(NULL)			{}
	};

	class PacketReciever
	{
	public:
		PacketReciever();
		virtual ~PacketReciever();
		
		// Get number of packets recieved.
		virtual int getNpackets() = 0;		
		
		virtual Packet popPacket() = 0;
	};

	class PacketSender
	{
	public:
		PacketSender();
		virtual ~PacketSender();
		
		virtual pushPacket(const Packet &packet) = 0;

		// Get number of packets queued to be transmitted.
		virtual int getNpackets() = 0;

		// Transmits queued packets.
		virtual void transmit() = 0;
	};
};

#endif