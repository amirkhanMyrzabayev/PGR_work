/**

    @file      InputManager.h
    @brief     Class for handling input.
    @details   ~
    @author    Amirkhan Myrzabayev
    @date      10.05.2026
    @copyright © Amirkhan Myrzabayev, 2026. All right reserved.

**/
#pragma once


/// @brief Keeps track of which keyboard keys are currently pressed down.
class InputManager
{
public:
	InputManager();
	~InputManager();

	/// @brief Marks a standard keyboard key as pressed.
	/// @param key The ASCII character of the key that was pressed.
	void pressKey(unsigned char key);

	/// @brief Marks a standard keyboard key as released.
	/// @param key The ASCII character of the key that was released.
	void releaseKey(unsigned char key);

	/// @brief Marks a special keyboard key (like arrows or F-keys) as pressed.
	/// @param key The specific integer code for the special key.
	void pressSpecialKey(int key);

	/// @brief Marks a special keyboard key as released.
	/// @param key The specific integer code for the special key.
	void releaseSpecialKey(int key);

	bool keys[256] = { false };        ///< Array storing the true/false pressed state of normal ASCII keys.
	bool specialKeys[256] = { false }; ///< Array storing the true/false pressed state of special non-ASCII keys.
};