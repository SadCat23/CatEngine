#include "KeyboardInputEvent.h"

KeyboardInputEvent::KeyboardInputEvent() :type(EventType::Invalid), key(0u)
{

}

KeyboardInputEvent::KeyboardInputEvent(const EventType type,const unsigned char key) : type(type), key(key)
{

}

bool KeyboardInputEvent::IsPress() const
{
	return this->type == EventType::Press;
}

bool KeyboardInputEvent::IsValid() const
{
	return this->type != EventType::Invalid;
}

bool KeyboardInputEvent::IsRealese() const
{
	return this->type == EventType::Release;
}
unsigned char KeyboardInputEvent::GetKeyCode() const
{
	return this->key;
}