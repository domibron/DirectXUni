#pragma once

// Must be before #include <DirectXMath.h> - allows direct access of DirectX Math Matrices/Vectors at cost of performace
//#define _XM_NO_INTRINSICS_
//#define XM_NO_
#include <DirectXMath.h>
using namespace DirectX;

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

class Window;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

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

