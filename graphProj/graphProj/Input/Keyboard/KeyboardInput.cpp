#include "KeyboardInput.h"

KeyboardInput::KeyboardInput()
{
	for (int i = 0; i < 256; i++)
	{
		this-> keyStates[i] = false;
	}
}

bool KeyboardInput::KeyIsPressed(const unsigned char keycode)
{
	return this->keyStates[keycode];
}

bool KeyboardInput::KeyBufferIsEmpty()
{
	return this->keyBuffer.empty();
}

bool KeyboardInput::CharBufferIsEmpty()
{
	return this->charBuffer.empty();
}

KeyboardInputEvent KeyboardInput::ReadKey()
{
	if (this->keyBuffer.empty())
	{
		return KeyboardInputEvent();
	}
	else
	{
		KeyboardInputEvent event = this->keyBuffer.front();
		this->keyBuffer.pop();
		return event;
	}

	
}

unsigned char KeyboardInput::ReadChar()
{
	if (this->charBuffer.empty())
	{
		return 0u;
	}
	else
	{
		unsigned char event = this->charBuffer.front();
		this->charBuffer.pop();
		return event;
	}

}

void KeyboardInput::OnKeyPressed(const unsigned char key)
{
	this->keyStates[key] = true;
	this->keyBuffer.push(KeyboardInputEvent(KeyboardInputEvent::EventType::Press, key));
}

void KeyboardInput::OnKeyReleased(const unsigned char key)
{
	this->keyStates[key] = false;
	this->keyBuffer.push(KeyboardInputEvent(KeyboardInputEvent::EventType::Release, key));

}

void KeyboardInput::OnChar(const unsigned char key)
{
	this->charBuffer.push(key);
}

void KeyboardInput::EnableAutoRepeatKeys()
{
	this->autoRepeatKeys = true;
}

void KeyboardInput::DisableAutoRepeatKeys()
{
	this->autoRepeatKeys = false;
}

void KeyboardInput::EnableAutoRepeatChars()
{
	this->autoRepeatchars = true;
}

void KeyboardInput::DisableAutoRepeatChars()
{
	this->autoRepeatchars = false;
}

bool KeyboardInput::IsKeysAutoRepeat()
{
	return this->autoRepeatKeys;
}

bool KeyboardInput::IsCharsAutoRepeat()
{
	return this->autoRepeatchars;
}
