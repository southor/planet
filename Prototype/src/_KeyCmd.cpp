#include "keycmd.h"

namespace Prototype
{
	KeyCmd::KeyCmd()
	{
		this->keystate = KEY_STATE_UP;
	}

	KeyCmd::KeyCmd(Cmd cmd, int key) 
	{
		this->cmd = cmd;
		this->key = key;
		this->keystate = KEY_STATE_UP;
	}

	Cmd KeyCmd::getCmd()
	{
		return cmd;
	}

	int KeyCmd::getKey()
	{
		return key;
	}

	void KeyCmd::setKey(int key)
	{
		this->key = key;
	}

	void KeyCmd::setKeyState(KeyState keystate)
	{
		this->keystate = keystate;
	}

	KeyState KeyCmd::getKeyState()
	{
		return keystate;
	}
};