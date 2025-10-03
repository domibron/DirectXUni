#include <Windows.h>
#include <d3d11.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//GLOBAL VARS
HINSTANCE g_hInst = NULL; // handle to this instance (our app loaded in memory)
HWND g_hWnd = NULL; // Handle to our created window
const wchar_t* windowName = L"DirectX Hello World!"; // Wide char array

IDXGISwapChain* g_swapchain = NULL; // the pointer to the swap chain interface
ID3D11Device* g_dev = NULL; // the pointer to our Direct3D device interface
ID3D11DeviceContext* g_devcon = NULL; // the pointer to our Direct3D device context

// Function Protortpes
HRESULT InitWindow(HINSTANCE instanceHandle, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitD3D(HWND hWin);
void CleanD3D();

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) 
{
	if (FAILED(InitWindow(hInstance, nCmdShow))) {
		MessageBox(NULL, L"Failed to create window", L"Critical Error!", MB_ICONERROR | MB_OK);
	}

	if (FAILED(InitD3D(g_hWnd))) {
		MessageBox(NULL, L"Unable to create swapchain and device.", L"Critical Error", MB_ICONERROR | MB_OK);
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

	CleanD3D();

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
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	// GetClientRect(g_hWnd, &rc); // alternative to using SCREEN WIDTH and HEIGHT
	// UINT width = rc.right - rc.left;
	// UINT height = rc.bottom - rc.top;

	// Create the window and use the result as the handle
	g_hWnd = CreateWindowEx(NULL,
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

HRESULT InitD3D(HWND hWnd)
{
	// Create a struct to hold the information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd = {}; // sets everything to 0 to remove garb vals
	scd.BufferCount = 1; // One back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH; // set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT; // set the back buffer height
	scd.BufferDesc.RefreshRate.Numerator = 60; //60 FPS
	scd.BufferDesc.RefreshRate.Denominator = 1; // 60/1 = 60 FPS
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Intended swapchain use
	scd.OutputWindow = hWnd; // Window to use
	scd.SampleDesc.Count = 1; // Number of samples for AA
	scd.Windowed = TRUE; // Windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Allow full-scrren switching

	HRESULT hr;
	// Create a swap chain, device and device context from the scd
	hr = D3D11CreateDeviceAndSwapChain(NULL, // Use default graphics adapter
		D3D_DRIVER_TYPE_HARDWARE, // Use hardware acceleration, can also use software or WARP renderers
		NULL, // Used for software driver types
		D3D11_CREATE_DEVICE_DEBUG, // Flags can be OR'd together. We are enabling debug for better warnings and errors
		NULL, // Direct3D feature levels. NULL will use D3D11.0 or older
		NULL, // Size of array passed to above member - NULL since we passed NULL
		D3D11_SDK_VERSION, // Always set to D3D11_SDK_VERSION
		&scd, // Pointer to our swap chain description
		&g_swapchain, // Pointer to our swap chain COM object
		&g_dev, // Pointer to our device
		NULL, // Out param - will be set to chosen feature level
		&g_devcon); // Pointer to our immediate device context

	if (FAILED(hr)) return hr;

	return S_OK;
}

void CleanD3D() {
	if (g_swapchain) g_swapchain->Release();
	if (g_dev) g_dev->Release();
	if (g_devcon) g_devcon->Release();
}