#pragma once

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

class Window;

class Renderer
{
public:
	Renderer(Window& inWindow);
	void Clean();
	void RenderFrame();

private:
	Window& window;
	IDXGISwapChain* swapchain = nullptr;
	ID3D11Device* dev = nullptr;
	ID3D11DeviceContext* devcon = nullptr;
	ID3D11RenderTargetView* backbuffer = nullptr;

	long InitD3D();
};

