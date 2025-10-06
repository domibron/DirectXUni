#include "Window.h"

const wchar_t* windowName = L"DirectX Hello World!"; // Wide char array

LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		// This message is sent when the user closes the window
		// Depending on your handling of application windows, you may not need this.
		// In this case, if a window is destroyed (e.g. when escape is pressed below), we
		// also tell the application to quit entirely by posting a quit message.
	case WM_DESTROY:
	{
		// Send a quit message to the app
		PostQuitMessage(0);
		return 0;
	}


	// Key being pressed down events
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd); // Note! Destroying the window is not the same as closing the app
			// Destroying the window will post a WM_DESTROY which will lead to
			// PostQuitMessage(0) being called. (as above)
			break;

		case 'W':

			// W key was pressed
			break;

		}


	default:
		// Let windows handle everything else with default handling
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

Window::Window(int width, int height, HINSTANCE hInstance, int nCmdShow)
	: instance(hInstance), width(width), height(height)
{
	
	// First we need to register our window classs.
	// This is how windows stores properties for a window that we want to create.

	WNDCLASSEX wc = {}; // "= {}" sets all values to 0. Can also be achieved with "ZeroMemory(&wc, sizeof(WNDCLASSX))" macro
	// fill in the struct with the needed information

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc; // Our window procedure function, make sure it handles window creation or calls DefWindowProc(..)
	wc.hInstance = instance; // Give our app's handle
	wc.lpszClassName = L"WindowClass1"; // Windows will store our window class under this name.
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // Background color of the win32 app (not needed for D3D apps)

	// Register class above struct. If it fails, if block will execute
	if (!RegisterClassEx(&wc)) {
		//LOG("Failed to register window class.");
	}

	// Adjust the window dimensions so that the top winodow bar is not taking pixels away from our app
	RECT rc = { 0, 0, width	, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	// GetClientRect(g_hWnd, &rc); // alternative to using SCREEN WIDTH and HEIGHT
	// UINT width = rc.right - rc.left;
	// UINT height = rc.bottom - rc.top;

	// Create the window and use the result as the handle
	handle = CreateWindowEx(NULL,
		L"WindowClass1", // Name of our window class
		windowName, // Title of the window
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, // Window style with no resizing and maxmimising
		// WS_OVERLAPPEDWINDOW, // Alternative window style that allows resizing
		100, // x-position of the window
		100, // y-position of the window
		rc.right - rc.left, // Width of the window
		rc.bottom - rc.top, // Hight of the window
		NULL, // No parent window, NULL
		NULL, // No menus, NULL
		instance, // Aplication handle
		NULL); // Used with multiple windows, NULL

	if (handle != NULL) {
		//Display the window to the screen
		ShowWindow(handle, nCmdShow);
	}
	else {
		LOG("Failed to create window.");
	}
}
