#pragma once
#include "ErrorLoger.h"

class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer*pWindowcontainer,HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height, float posX, float posY);
	bool ProcessMessages();
	HWND GetHWND() const;
	~RenderWindow();

private:
	void RegisterWindowClass();
	HWND handle = NULL;
	HINSTANCE hInstance = NULL;
	std::string window_title = "";
	std::string window_class = "";
	std::wstring window_title_wide = L"";
	std::wstring window_class_wide = L"";
	int width = 0;
	int height = 0;
	float posX = 0;
	float posY = 0;

};
