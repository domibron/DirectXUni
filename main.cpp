#include <Windows.h>
#include <d3d11.h>
#include "Debug.h"
#include <iostream>
#include "Window.h"
#include "Renderer.h"

void OpenConsole();

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) 
{
	OpenConsole();

	Window window{ 800, 600, hInstance, nCmdShow };
	Renderer renderer{ window };



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
			static float fakeTime = 0;
			fakeTime += 0.0001f;
			Transform& t = renderer.transform; // chache reference.
			t.position = XMVectorSetZ(t.position, 0.75f);
			t.position = XMVectorSetX(t.position, sin(fakeTime));
			t.Rotate({ 0.0001f, 0.0001f, 0 });
			//renderer.transform.position = XMVectorSetZ(renderer.transform.position, 0.5f);

			renderer.RenderFrame();
		}
	}

	renderer.Clean();

	return 0;

}


void OpenConsole()
{
	if (AllocConsole())
	{
		// Console successfully allocated

		//// Read from console
		//wchar_t c[7];
		//memset(c, 0, sizeof(c));
		//DWORD n;
		//ReadConsole(GetStdHandle(STD_INPUT_HANDLE), &c, 5, &n, 0);

		//// Write to console
		//wchar_t text[] = L"Side Console\n";
		//DWORD n;
		//WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), text, wcslen(text), NULL, 0);

		// Redirecting inout, output and errors to allocated console
		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		std::ios::sync_with_stdio(true);

		std::cout << "Hello Side Console!!" << std::endl;

		// TODO: Make sure to open and close file stream properly (otherwise fuckery will happen and windows will fucking DIE)
	}
}