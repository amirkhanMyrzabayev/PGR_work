#include "InputManager.h"


InputManager::InputManager() {
}

InputManager::~InputManager() {
}

void InputManager::pressKey(unsigned char key) {
    keys[key] = true;
}

void InputManager::releaseKey(unsigned char key) {
    keys[key] = false;
}

void InputManager::pressSpecialKey(int key) {
    specialKeys[key] = true;
}

void InputManager::releaseSpecialKey(int key) {
    specialKeys[key] = false;
}
