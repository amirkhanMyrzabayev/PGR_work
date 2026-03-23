#pragma once
class InputManager
{
public:
	InputManager();
	~InputManager();

	void pressKey(unsigned char key);
	void releaseKey(unsigned char key);
	void pressSpecialKey(int key);
	void releaseSpecialKey(int key);

	bool keys[256] = { false };
	bool specialKeys[256] = { false };
};

