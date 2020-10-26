#pragma once
class KeyboardInputEvent
{
public :

	enum EventType
	{
		Press,
		Release,
		Invalid
	};

	KeyboardInputEvent();
	KeyboardInputEvent(const EventType type, const unsigned char key);
	bool IsPress() const;
	bool IsRealese() const;
	bool IsValid() const;
	unsigned char GetKeyCode() const;
private:
	EventType type;
	unsigned char key;
};