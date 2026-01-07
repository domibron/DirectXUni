#include <Windows.h>
#include <d3d11.h>
#include "Debug.h"
#include <iostream>
#include "Window.h"
#include "Renderer.h"
#include "Mesh.h"
#include "GameObject.h"
#include <Mouse.h>
#include "Texture.h"
#include "Material.h"
#include "Material_Lit.h"
#include "TimeKeeping.h"
#include "ChunkData.h"
#include "BlockMesh.h"
#include "BlockObject.h"
#include "PhysicsHanderler.h"
#include "PlayerEntity.h"

using namespace DirectX; // ? fixes the XMVector * float.

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

	TimeKeeping timeKeeping;

	Mesh mesh_cube{ renderer, "Assets/cube.obj" };
	Mesh mesh_block{ renderer, "Assets/block.obj" };
	Mesh mesh_sphere{ renderer, "Assets/Sphere.obj" };
	Mesh mesh_grass{ renderer, "Assets/grass.obj", true };
	Texture tex_box{ renderer, "Assets/Box.bmp" };
	Texture tex_grass{ renderer, "Assets/grass.png", true };
	Texture tex_skybox{ renderer, "Assets/skybox01.dds", false, Texture::TextureType::Cubemap };
	Material mat_unlit{ "Unlit", renderer, "Compiled Shaders/VertexShader.cso", "Compiled Shaders/PixelShader.cso", &tex_box };
	Material mat_skybox{ "Skybox", renderer, "Compiled Shaders/SkyboxVShader.cso", "Compiled Shaders/SkyboxPShader.cso", &tex_skybox };
	Material_Lit mat_lit{ "Lit", renderer, "Compiled Shaders/ReflectiveVShader.cso", "Compiled Shaders/ReflectivePShader.cso", &tex_box };
	mat_lit.SetReflectionTexture(&tex_skybox);
	mat_lit.reflectiveness = 0.001f;
	Material_Lit mat_litGrass{ "LitGrass", renderer, "Compiled Shaders/ReflectiveVShader.cso", "Compiled Shaders/ReflectivePShader.cso", &tex_grass };
	mat_lit.SetReflectionTexture(&tex_skybox);
	mat_litGrass.reflectiveness = 0.01f;

	BlockMesh bm_block{ renderer };


	PhysicsHanderler pHanderler{&timeKeeping};
	
	PlayerEntity player;
	renderer.camera = player.GetCamera();

	pHanderler.RegisterRigidBody(&player);

	/*
	You can extend your GameObject class further by creating a virtual or abstract (more often referred to as pure virtual in C++)
	Update function which can be overridden inside child classes. In other words, you could make a “SpinningGameObject” child
	class which overrides the Update function to rotate itself. You then just call the Update function on all GameObjects every
	frame.
	*/
	if (renderer.camera != nullptr)
	renderer.camera->camTransform->position = DirectX::XMVectorSetZ(renderer.camera->camTransform->position, -1);

	GameObject go_skybox{ "Skybox", &mesh_cube, &mat_skybox };
	renderer.skyboxObject = &go_skybox;

	GameObject go1{ "Cube", &mesh_cube, &mat_lit };
	GameObject go2{ "Sphere", &mesh_sphere, &mat_lit };
	GameObject go_grass{ "Grass", &mesh_grass, &mat_litGrass };


	renderer.RegisterGameObject(&go1);
	renderer.RegisterGameObject(&go2);
	renderer.RegisterGameObject(&go_grass);

	ChunkData chunk{ &renderer, XMVectorSet(0,0,0,1), &bm_block, &mat_lit };
	chunk.LoadChunk();

	go1.transform.position = DirectX::XMVectorSet(-2, 0, 0, 1);
	go2.transform.position = DirectX::XMVectorSet(2, 0, 0, 1);
	//go3.transform.position = DirectX::XMVectorSet(-4, 0, 0, 1);
	go_grass.transform.position = DirectX::XMVectorSet(-5, -0.5f, 0, 1);

	renderer.pointLights[0] = { XMVECTOR{-1, 1, -3}, {0.85f, 0, 0.85f}, 10, true };
	renderer.pointLights[1] = { XMVECTOR{1, -1, -4}, {0, 0.85f, 0.85f}, 20, true };

	// Used to hold windows event messages
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

			auto kbState = DirectX::Keyboard::Get().GetState();

			if (renderer.camera != nullptr) {

				if (kbState.S) {
					renderer.camera->camTransform->Translate(renderer.camera->camTransform->GetForward() * -10.0f * timeKeeping.GetDeltaTime());
				}

				if (kbState.W) {
					renderer.camera->camTransform->Translate(renderer.camera->camTransform->GetForward() * 10.0f * timeKeeping.GetDeltaTime());
				}

				if (kbState.A) {
					renderer.camera->camTransform->Translate(renderer.camera->camTransform->GetRight() * -10.0f * timeKeeping.GetDeltaTime());
				}

				if (kbState.D) {
					renderer.camera->camTransform->Translate(renderer.camera->camTransform->GetRight() * 10.0f * timeKeeping.GetDeltaTime());
				}

				auto msState = DirectX::Mouse::Get().GetState();
				renderer.camera->camTransform->Rotate({ -(float)msState.y * 0.001f, (float)msState.x * 0.001f, 0 });
				if (msState.leftButton)
					renderer.camera->camTransform->position = { 0, 0, -5 };
			}

			pHanderler.TickPhysics();

			renderer.RenderFrame();

			
			// we tick after becuse we need to 
			timeKeeping.Tick();
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
