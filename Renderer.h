#pragma once

#include "Transform.h" // You have directX using in this class FYI
#include "Camera.h"
#include <vector>


struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;

struct ID3D11DepthStencilView;

struct ID3D11RasterizerState;

struct ID3D11BlendState;

struct ID3D11DepthStencilState;

class Window;

class GameObject;

class Texture;

class Renderer
{
public:
	Renderer(Window& inWindow);
	void Clean();
	void RenderFrame();

	ID3D11Device* GetDevice() { return dev; }
	ID3D11DeviceContext* GetDeviceCon() { return devcon; }


	Camera camera;
	std::vector<GameObject*> gameObjects;

	void RegisterGameObject(GameObject* e);
	void RemoveGameObject(GameObject* e);

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

	ID3D11DepthStencilView* depthBuffer = NULL;

	ID3D11RasterizerState* rasterizerCullBack = nullptr;
	ID3D11RasterizerState* rasterizerCallNone = nullptr;

	ID3D11BlendState* blendOpaque = nullptr;
	ID3D11BlendState* blendTransparent = nullptr;

	ID3D11DepthStencilState* depthWriteOff = nullptr;

	long InitD3D();
	long InitPipeline();
	void InitGraphics();
	long InitDepthBuffer();
};

