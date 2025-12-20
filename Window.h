#pragma once

#include "Debug.h"
#include <Keyboard.h> // You may need to reinstall the nuget package after git or moving to new comp since its stored in the c drive under programs x64 for some god damn reason.
#include <Mouse.h>

#include <Windows.h>

class Window
{
private:
	HWND handle = NULL;
	HINSTANCE instance = NULL;
	int width = 32, height = 32;

	DirectX::Keyboard keyboard;
	DirectX::Mouse mouse;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	bool Exists() { return handle != NULL; }
	HWND GetHandle() { return handle; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	Window(int width, int height, HINSTANCE hInstance, int nCmdShow);
};

