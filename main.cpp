#include <Windows.h>

HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;
const wchar_t* windowName = L"DirectX Hello World!";

HRESULT InitWindow(HINSTANCE instanceHandle, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) 
{
	if (FAILED(InitWindow(hInstance, nCmdShow))) {
		MessageBox(NULL, L"Failed to create window", L"Critical Error!", MB_ICONERROR | MB_OK);
	}

	// Used to hold windows even messages
	MSG msg;

	// Enter the main loop
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// Translate certain messages into correct format, namely key presses
			TranslateMessage(&msg);

			// Send the message to the WindowProc function
			DispatchMessage(&msg);

			// Break out of the loop if a quit message is detected
			if (msg.message == WM_QUIT)
				break;
		}
		else {
			// Game code here.
		}
	}

	return 0;

}


HRESULT InitWindow(HINSTANCE instanceHandle, int nCmdShow) 
{
	g_hInst = instanceHandle; // Store our app handle (app memory location)

	// First we need to register our window classs.
	// This is how windows stores properties for a window that we want to create.

	WNDCLASSEX wc = {}; // "= {}" sets all values to 0. Can also be achieved with "ZeroMemory(&wc, sizeof(WNDCLASSX))" macro
	// fill in the struct with the needed information

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc; // Our window procedure function, make sure it handles window creation or calls DefWindowProc(..)
	wc.hInstance = instanceHandle; // Give our app's handle
	wc.lpszClassName = L"WindowClass1"; // Windows will store our window class under this name.
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // Background color of the win32 app (not needed for D3D apps)

	// Register class above struct. If it fails, if block will execute
	if (!RegisterClassEx(&wc)) {
		return E_FAIL; // Return fail code to be picked up by FAIELD(..) macro
	}

	// Adjust the window dimensions so that the top winodow bar is not taking pixels away from our app
	RECT wr = { 0, 0, 640, 480 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and use the result as the handle
	g_hWnd = CreateWindowEx(NULL,
		L"WindowClass1", // Name of our window class
		windowName, // Title of the window
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, // Window style with no resizing and maxmimising
		// WS_OVERLAPPEDWINDOW, // Alternative window style that allows resizing
		100, // x-position of the window
		100, // y-position of the window
		wr.right - wr.left, // Width of the window
		wr.bottom - wr.top, // Hight of the window
		NULL, // No parent window, NULL
		NULL, // No menus, NULL
		instanceHandle, // Aplication handle
		NULL); // Used with multiple windows, NULL

	if (g_hWnd == NULL) return E_FAIL;

	//Display the window to the screen
	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
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
			DestroyWindow(g_hWnd); // Note! Destroying the window is not the same as closing the app
			// Destroying the window will post a WM_DESTROY which will lead to
			// PostQuitMessage(0) being called. (as above)
			break;

		case 'W':

			// W key was pressed
			break;

		}
	

	default:
		// Let windows handle everything else with default handling
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}