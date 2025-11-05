#pragma once



struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;

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

	ID3D11Buffer* vBuffer = nullptr; // Vertex buffer
	ID3D11Buffer* iBuffer = nullptr; // Index buffer
	ID3D11Buffer* cBuffer_PerObject = nullptr; // Constant Buffer

	long InitD3D();
	long InitPipeline();
	void InitGraphics();
};

