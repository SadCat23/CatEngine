#include "MouseInput.h"

void MouseInput::OnLeftPressed(int x, int y)
{
	this->leftIsDown = true;
	MouseInputEvent event(MouseInputEvent::EventType::LPress, x, y);
	this->eventBuffer.push(event);

}

void MouseInput::OnLeftReleased(int x, int y)
{
	this->leftIsDown = false;
	MouseInputEvent event(MouseInputEvent::EventType::LRelease, x, y);
	this->eventBuffer.push(event);
}

void MouseInput::OnRightPressed(int x, int y)
{
	this->rightIsDown = true;
	MouseInputEvent event(MouseInputEvent::EventType::RPress, x, y);
	this->eventBuffer.push(event);
}

void MouseInput::OnRightReleased(int x, int y)
{
	this->rightIsDown = false;
	MouseInputEvent event(MouseInputEvent::EventType::RRelease, x, y);
	this->eventBuffer.push(event);
}

void MouseInput::OnMiddlePressed(int x, int y)
{
	this->middleIsDown = true;
	MouseInputEvent event(MouseInputEvent::EventType::MPress, x, y);
	this->eventBuffer.push(event);
}

void MouseInput::OnMiddleReleased(int x, int y)
{
	this->middleIsDown = false;
	MouseInputEvent event(MouseInputEvent::EventType::MRelease, x, y);
	this->eventBuffer.push(event);
}

void MouseInput::OnWheelUp(int x, int y)
{
	MouseInputEvent event(MouseInputEvent::EventType::WheelUP, x, y);
	this->eventBuffer.push(event);
}

void MouseInput::OnWheelDown(int x, int y)
{
	MouseInputEvent event(MouseInputEvent::EventType::WheelDown, x, y);
	this->eventBuffer.push(event);
}

void MouseInput::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
	MouseInputEvent event(MouseInputEvent::EventType::Move, x, y);
	this->eventBuffer.push(event);
}

void MouseInput::OnMouseMoveRaw(int x, int y)
{
	MouseInputEvent event(MouseInputEvent::EventType::RAW_MOVE, x, y);
	this->eventBuffer.push(event);
}

bool MouseInput::IsLeftDown()
{
	return this->leftIsDown;
}

bool MouseInput::IsRightDown()
{
	return this->rightIsDown;
}

bool MouseInput::IsMiddleDown()
{
	return this->middleIsDown;
}

int MouseInput::GetPosX()
{
	return this->x;
}

int MouseInput::GetPosY()
{
	return this->y;
}

MousePoint MouseInput::GetPos()
{
	return { this->x,this->y };
}

bool MouseInput::EventBufferIsEmpty()
{
	return this->eventBuffer.empty();
}

MouseInputEvent MouseInput::ReadEvent()
{
	if (this->eventBuffer.empty())
	{
		return MouseInputEvent();
	}
	else
	{
		MouseInputEvent event = this->eventBuffer.front();
		this->eventBuffer.pop();
		return event;
	}
}
