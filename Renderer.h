#pragma once

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Window;

class Renderer
{
public:
	Renderer(Window& inWindow);
	void Clean();

private:
	Window& window;
	IDXGISwapChain* swapchain = nullptr;
	ID3D11Device* dev = nullptr;
	ID3D11DeviceContext* devcon = nullptr;
};

