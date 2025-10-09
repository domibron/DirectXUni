#pragma once



struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

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

	ID3D11VertexShader* pVS = nullptr; // Vertex Shader
	ID3D11PixelShader* pPS = nullptr; // Pixel Shader
	ID3D11InputLayout* pIL = nullptr; // Input layout

	long InitD3D();
	long InitPipeline();
};

