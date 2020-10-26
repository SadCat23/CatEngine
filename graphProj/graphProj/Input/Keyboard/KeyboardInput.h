#pragma once
#include "KeyboardInputEvent.h"
#include <queue>

class KeyboardInput
{
public:
	KeyboardInput();

	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty();
	bool CharBufferIsEmpty();
	
	KeyboardInputEvent ReadKey();
	unsigned char ReadChar();
	
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);

	void EnableAutoRepeatKeys();
	void DisableAutoRepeatKeys();

	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();

	bool IsKeysAutoRepeat();
	bool IsCharsAutoRepeat();


	
private:
	bool autoRepeatKeys = false;
	bool autoRepeatchars = false;
	bool keyStates[256];
	std::queue<KeyboardInputEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;
	

};