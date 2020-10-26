#pragma once
struct MousePoint
{
	int x;
	int y;
};

class MouseInputEvent
{
public:
		enum EventType
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			MPress,
			MRelease,
			WheelUP,
			WheelDown,
			Move,
			RAW_MOVE,
			Invalid
		};
private:
		EventType type;
		int x;
		int y;
public:
	MouseInputEvent();
	MouseInputEvent(const EventType type, const int x, const int y);
	bool IsValid() const;
	EventType GetType() const;
	MousePoint GetPos() const;
	int GetPosX() const;
	int GetPosY() const;

};