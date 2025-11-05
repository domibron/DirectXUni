#pragma once

#include "Debug.h"
#include <Windows.h>

class Camera;

class Window
{
private:
	HWND handle = NULL;
	HINSTANCE instance = NULL;
	int width = 32, height = 32;

	static Camera* cam;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	bool Exists() { return handle != NULL; }
	HWND GetHandle() { return handle; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	Window(int width, int height, HINSTANCE hInstance, int nCmdShow);

	static void SetCamera(Camera& camera);
};

