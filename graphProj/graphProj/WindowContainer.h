#pragma once
#include "RenderWindow.h"
#include "Input/Keyboard/KeyboardInput.h"
#include "Input/Mouse/MouseInput.h"
#include "Graphics.h"
class WindowContainer
{

public:
	WindowContainer();
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	RenderWindow render_window;
	KeyboardInput keyboardInput;
	MouseInput mouseInput;
	Graphics gfx;

private:

};