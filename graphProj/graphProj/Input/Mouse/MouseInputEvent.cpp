#include "MouseInputEvent.h"

MouseInputEvent::MouseInputEvent(): type(EventType::Invalid),x(0),y(0)
{
}

MouseInputEvent::MouseInputEvent(EventType type, int  x, int y) : type(type), x(x), y(y)
{
}

bool MouseInputEvent::IsValid() const
{
	return this->type != EventType::Invalid;
}

 MouseInputEvent::EventType MouseInputEvent::GetType() const
{
	 return this->type;
	
}
 MousePoint MouseInputEvent::GetPos() const
 {
	 return  { this->x,this->y };

 } 
 int MouseInputEvent::GetPosX() const
 {
	 return  this->x;

 }

 int MouseInputEvent::GetPosY() const
 {
	 return  this->y;

 }
