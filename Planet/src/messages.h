#ifndef __Messages_h__
#define __Messages_h__

#include "Color.h"
#include "basic.h"

namespace Planet
{
	enum MessageType
	{
		// server
		UPDATE_PLAYER_OBJ = 0,
		ADD_PLAYER_OBJ,		
		ADD_OBSTACLE,
		WELCOME_CLIENT,
		ADD_PLAYER,
		ADD_PROJECTILE,
		UPDATE_PROJECTILE,
		REMOVE_PROJECTILE,
		KILL,
		START_GAME,
		SYNC_PONG,
		SET_TICK_0_TIME,

		// client
		INIT_CLIENT,
		SYNC_PING,
		USER_CMD,
		SHOOT_CMD,
	};

	// --------------------------------------------------------------------------------
	// --------------------------------- server messages ------------------------------
	// --------------------------------------------------------------------------------

	struct WelcomeClient
	{
		static const size_t messageType = WELCOME_CLIENT;

		PlayerId playerId;

		WelcomeClient(PlayerId playerId) 
			: playerId(playerId) {}
	};

	struct AddPlayer
	{
		static const size_t messageType = ADD_PLAYER;

		PlayerId playerId;
		Color color;
		Pos startPos;
		
		AddPlayer(PlayerId playerId, Color color, Pos startPos) 
			: playerId(playerId), color(color), startPos(startPos) {}
	};
	
	struct StartGame
	{
		static const size_t messageType = START_GAME;
	};

	struct SyncPong
	{
		static const size_t messageType = SYNC_PONG;

		int time;
		int pingSendTime;

		SyncPong(int time, int pingSendTime) : time(time), pingSendTime(pingSendTime) {}
	};

	struct SetTick0Time
	{
		static const size_t messageType = SET_TICK_0_TIME;

		int tick0Time;

		SetTick0Time(int tick0Time) : tick0Time(tick0Time)
		{}

		inline operator int() const		{ return tick0Time; }
	};

	//// --------------------------------------------------------------------------------
	//// --------------------------------- client messages ------------------------------
	//// --------------------------------------------------------------------------------

	struct InitClient
	{
		static const size_t messageType = INIT_CLIENT;

		Color color;
		
		InitClient(Color color) : color(color) {}
	};
	
	struct SyncPing
	{
		static const size_t messageType = SYNC_PING;

		PlayerId playerId;
		int pingSendTime;

		SyncPing(PlayerId playerId, int pingSendTime) : playerId(playerId), pingSendTime(pingSendTime) {}
	};

};

#endif
