#include "keyhandler.h"

namespace Prototype
{

	KeyHandler::KeyHandler()
	{
	}

	void KeyHandler::setClient1Keys()
	{
		cmds.resize(NUMBER_OF_COMMANDS);
	
		cmds[CMD_LEFT] = KeyCmd(CMD_LEFT, SDLK_a);
		cmds[CMD_RIGHT] = KeyCmd(CMD_RIGHT, SDLK_d);
		cmds[CMD_UP] = KeyCmd(CMD_UP, SDLK_w);
		cmds[CMD_DOWN] = KeyCmd(CMD_DOWN, SDLK_s);
		cmds[CMD_ROTATE_LEFT] = KeyCmd(CMD_ROTATE_LEFT, SDLK_c);
		cmds[CMD_ROTATE_RIGHT] = KeyCmd(CMD_ROTATE_RIGHT, SDLK_v);
		cmds[CMD_SHOOT] = KeyCmd(CMD_SHOOT, SDLK_SPACE);
	}

	void KeyHandler::setClient2Keys()
	{
		cmds.resize(NUMBER_OF_COMMANDS);
	
		cmds[CMD_LEFT] = KeyCmd(CMD_LEFT, SDLK_k);
		cmds[CMD_RIGHT] = KeyCmd(CMD_RIGHT, SDLK_SEMICOLON);
		cmds[CMD_UP] = KeyCmd(CMD_UP, SDLK_o);
		cmds[CMD_DOWN] = KeyCmd(CMD_DOWN, SDLK_l);
		cmds[CMD_ROTATE_LEFT] = KeyCmd(CMD_ROTATE_LEFT, SDLK_LEFT);
		cmds[CMD_ROTATE_RIGHT] = KeyCmd(CMD_ROTATE_RIGHT, SDLK_RIGHT);
		cmds[CMD_SHOOT] = KeyCmd(CMD_SHOOT, SDLK_RSHIFT);
	}

	bool KeyHandler::getPressed(Cmd cmd)
	{
		bool pressed = isPressed(cmd);
		
		if (pressed)
		{
			cmds[cmd].setKeyState(KEY_STATE_DOWN);
		}
		
		return pressed;
	}

	bool KeyHandler::getReleased(Cmd cmd)
	{
		bool released = isReleased(cmd);
		
		if (released)
		{
			cmds[cmd].setKeyState(KEY_STATE_UP);
		}
		
		return released;
	}

	bool KeyHandler::isPressed(Cmd cmd)
	{
		return cmds[cmd].getKeyState() == KEY_STATE_PRESSED;
	}

	bool KeyHandler::isReleased(Cmd cmd)
	{
		return cmds[cmd].getKeyState() == KEY_STATE_RELEASED;
	}

	bool KeyHandler::isUp(Cmd cmd)
	{
		return cmds[cmd].getKeyState() == KEY_STATE_UP;
	}

	bool KeyHandler::isDown(Cmd cmd)
	{
		return cmds[cmd].getKeyState() == KEY_STATE_DOWN;
	}

	void KeyHandler::setKeyPressed(int key)
	{
		for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
		{
			if (cmds[i].getKey() == key)
			{
				cmds[i].setKeyState(KEY_STATE_PRESSED);
			}
		}
	}

	void KeyHandler::setKeyReleased(int key)
	{
		for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
		{
			if (cmds[i].getKey() == key)
			{
				cmds[i].setKeyState(KEY_STATE_RELEASED);
			}
		}
	}

	bool KeyHandler::changePressedToDownState()
	{
		bool someKeyPressed = false;
		
		for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
		{
			if (cmds[i].getKeyState() == KEY_STATE_PRESSED)
			{
				someKeyPressed = true;
				cmds[i].setKeyState(KEY_STATE_DOWN);
			}
		}
		
		return someKeyPressed;
	}
	
	bool KeyHandler::changeReleasedToUpState()
	{
		bool someKeyReleased = false;
		
		for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
		{
			if (cmds[i].getKeyState() == KEY_STATE_RELEASED)
			{
				someKeyReleased = true;
				cmds[i].setKeyState(KEY_STATE_UP);
			}
		}
		
		return someKeyReleased;
	}
	

};
