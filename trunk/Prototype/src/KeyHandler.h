#ifndef __keyhandler_h__
#define __keyhandler_h__

#include "common.h"
#include "KeyCmd.h"

#include <vector>

namespace Prototype
{

	class KeyHandler
	{
	public:
		/**
		 * Default constructor
		 */
		KeyHandler();
		
		void KeyHandler::setClient1Keys();
		void KeyHandler::setClient2Keys();

		/**
		 * Checks if command is pressed and if so changes the state to KEY_STATE_DOWN
		 * @param cmd Command to check
		 * @return true if command is pressed
		 */
		bool getPressed(Cmd cmd);

		/**
		 * Checks if command is released and if so changes the state to KEY_STATE_UP
		 * @param cmd Command to check
		 * @return true if command is released
		 */
		bool getReleased(Cmd cmd);
		
		/**
		 * Checks if keycommand is being pressed
		 * @param cmd Command to check
		 * @return true if keycommand is being pressed
		 */
		bool isPressed(Cmd cmd);
		
		/**
		 * Checks if keycommand is being released
		 * @param cmd Command to check
		 * @return true if keycommand is being released
		 */
		bool isReleased(Cmd cmd);

		/**
		 * Checks if keycommand is up
		 * @param cmd Command to check
		 * @return true if keycommand is up
		 */
		bool isUp(Cmd cmd);

		/**
		 * Checks if keycommand is down
		 * @param cmd Command to check
		 * @return true if keycommand is down
		 */
		bool isDown(Cmd cmd);

		/**
		 * Sets the keys state to pressed
		 * @param key Key which state we should change
		 */
		void setKeyPressed(int key);

		/**
		 * Sets the keys state to released
		 * @param key Key which state we should change
		 */
		void setKeyReleased(int key);

		/**
		 * Changes all key commands with pressed state to down state
		 * @return true if any key was pressed
		 */
		bool changePressedToDownState();

		/**
		 * Changes all key commands with released state to up state
		 * @return true if any key was released
		 */
		bool changeReleasedToUpState();
		
		/**
		 * @return Size of container holding all key commands, i.e. number of key commands
		 */
//		int getSize();

		/**
		 * @param cmd Command of KeyCommand
		 * @return The state of wanted KeyCommand
		 */
//		KeyState getKeyState(Cmd cmd);

		/**
		 * @param cmd Command of KeyCommand
		 * @return The state of wanted KeyCommand
		 */
//		KeyCmd getKeyCommandFromCmd(Cmd cmd);

		/**
		 * @param key Key of KeyCommand
		 * @return The wanted KeyCommand
		 */
//		KeyCmd getKeyCommandFromKey(int key);

	private:
		std::vector<KeyCmd> cmds;
	};
};
#endif