#include <Windows.h>
#include <d3d11.h>
#include "Debug.h"
#include <iostream>
#include "Window.h"
#include "Renderer.h"
#include "Mesh.h"
#include "GameObject.h"
#include <Mouse.h>

void OpenConsole();

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) 
{
	DirectX::Mouse m;

	OpenConsole();

	Window window{ 800, 600, hInstance, nCmdShow };
	Renderer renderer{ window };

	window.SetCamera(renderer.camera);

	Mesh mesh_cube{ renderer, "Assets/cube.obj" };
	Mesh mesh_sphere{ renderer, "Assets/Sphere.obj" };

	/*
	You can extend your GameObject class further by creating a virtual or abstract (more often referred to as pure virtual in C++)
	Update function which can be overridden inside child classes. In other words, you could make a “SpinningGameObject” child
	class which overrides the Update function to rotate itself. You then just call the Update function on all GameObjects every
	frame.
	*/

	renderer.camera.transform.position = DirectX::XMVectorSetZ(renderer.camera.transform.position, -1);
	GameObject go1{ "Cube", &mesh_cube };
	GameObject go2{ "Sphere", &mesh_sphere };

	renderer.RegisterGameObject(&go1);
	renderer.RegisterGameObject(&go2);

	go1.transform.position = DirectX::XMVectorSet(-2, 0, 0, 1);
	go2.transform.position = DirectX::XMVectorSet(2, 0, 0, 1);

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
			//static float fakeTime = 0;
			//fakeTime += 0.0001f;
			//Transform& t = renderer.transform; // chache reference.
			//t.position = XMVectorSetZ(t.position, 0.75f);
			//t.position = XMVectorSetX(t.position, sin(fakeTime));
			//t.Rotate({ 0.0001f, 0.0001f, 0 });
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