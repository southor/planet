#include "keyhandler.h"

namespace Prototype
{

	KeyHandler::KeyHandler()
	{
		cmds.resize(NUMBER_OF_COMMANDS);
	
		cmds[CMD_LEFT] = KeyCmd(CMD_LEFT, SDLK_LEFT);
		cmds[CMD_RIGHT] = KeyCmd(CMD_RIGHT, SDLK_RIGHT);
		cmds[CMD_UP] = KeyCmd(CMD_UP, SDLK_UP);
		cmds[CMD_DOWN] = KeyCmd(CMD_DOWN, SDLK_DOWN);
		cmds[CMD_SHOOT] = KeyCmd(CMD_SHOOT, SDLK_SPACE);

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
