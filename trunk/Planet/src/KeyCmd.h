#ifndef __KeyCmd_h__
#define __KeyCmd_h__

#define NUMBER_OF_COMMANDS 8

namespace Planet
{

	enum Cmd
	{
		CMD_LEFT,
		CMD_RIGHT,
		CMD_UP,
		CMD_DOWN,
		CMD_ROTATE_LEFT,
		CMD_ROTATE_RIGHT,
		CMD_SHOOT,
		CMD_SWITCH_WEAPON
	};

	enum KeyState
	{
		KEY_STATE_PRESSED,
		KEY_STATE_DOWN,
		KEY_STATE_RELEASED,
		KEY_STATE_UP
	};

	/**
	 * Holder of a key command
	 */
	class KeyCmd
	{
	public:
		KeyCmd();

		KeyCmd(Cmd cmd, int key);

		/**
		 * @return The command of this command
		 */
		Cmd getCmd();
		
		/**
		 * @return The category of this command
		 */
		//int getCategory();

		/**
		 * @return The key of this command
		 */
		int getKey();

		/**
		 * Sets the key of this command
		 * @param key New key
		 */
		void setKey(int key);

		/**
		 * Sets the key state of this command
		 * @param keystate New key state
		 */
		void setKeyState(KeyState keystate);

		/**
		 * @return The key state of this command
		 */
		KeyState getKeyState();

	private:
		Cmd cmd;
		int key;
		//int category;
		KeyState keystate;
	};
};
#endif

