#include "keycmd.h"

KeyCmd::KeyCmd(Cmd cmd, int key) 
{
	this->cmd = cmd;
	this->key = key;
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
