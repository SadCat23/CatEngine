#pragma once

#include "WindowContainer.h";

WindowContainer::WindowContainer()
{
	static bool raw_input_init = false;
	if (raw_input_init == false)
	{
		RAWINPUTDEVICE rawDevice;

		rawDevice.usUsagePage = 0x01;
		rawDevice.usUsage = 0x02;
		rawDevice.dwFlags = 0;
		rawDevice.hwndTarget = NULL;


		if (RegisterRawInputDevices(&rawDevice, 1, sizeof(rawDevice)) == FALSE)
		{
			ErrorLogger::Log(GetLastError(), "Fatal error rawInput");
			exit(-1);
		}
		raw_input_init = true;
	}
}

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
#pragma region Keyboard
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char> (wParam);
		if (keyboardInput.IsKeysAutoRepeat())
		{
			keyboardInput.OnKeyPressed(keycode);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				keyboardInput.OnKeyPressed(keycode);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char> (wParam);
		keyboardInput.OnKeyReleased(keycode);
		return 0;
	}

	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char> (wParam);
		if (keyboardInput.IsCharsAutoRepeat())
		{
			keyboardInput.OnChar(ch);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				keyboardInput.OnChar(ch);
			}
		}
		return 0;

	}
#pragma endregion Keyboard

#pragma region Mouse
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouseInput.OnMouseMove(x, y);
		return 0; 
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouseInput.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouseInput.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouseInput.OnMiddlePressed(x, y);
		return 0;
	}


	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouseInput.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouseInput.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouseInput.OnMiddleReleased(x, y);
		return 0;
	}

	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			mouseInput.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			mouseInput.OnWheelDown(x, y);
		}
		break;
		return 0;

	}

#pragma endregion Mouse

#pragma region RawInput

	case WM_INPUT:
	{

		UINT dataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));		
		
		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					mouseInput.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}
		
		
		return DefWindowProc(hwnd, uMsg, wParam, lParam);




	}



#pragma endregion RawInput
	default:
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	}
	}
	
