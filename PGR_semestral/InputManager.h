#pragma once
class InputManager
{
public:
	InputManager();
	~InputManager();

	void pressKey(unsigned char key);
	void releaseKey(unsigned char key);

	bool keys[256] = { false };
};

