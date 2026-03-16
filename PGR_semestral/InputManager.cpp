#include "InputManager.h"

InputManager::InputManager() {
    // Constructor: initialize your keys array to false here
}

InputManager::~InputManager() {
}

void InputManager::pressKey(unsigned char key) {
    keys[key] = true;
}

void InputManager::releaseKey(unsigned char key) {
    keys[key] = false;
}
