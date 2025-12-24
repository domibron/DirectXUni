#define MAX_POINT_LIGHTS 8

#pragma once

#include "Transform.h" // You have directX using in this class FYI
#include "Camera.h"
#include <vector>
#include <SpriteFont.h>


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

struct PointLight {
	DirectX::XMVECTOR position{ 0,0,0 }; // 16 bytes
	DirectX::XMVECTOR color{ 1,1,1 }; // 16 bytes

	float strength = 10; // 4 bytes
	bool enabled = false; // 4 bytes
	float padding[2]; // 8 bytes
	// 48 total bytes
};

struct DirectionalLight {
	DirectX::XMVECTOR directionFrom{ 0,0,0 };
	DirectX::XMVECTOR color{ 1,1,1 };
};

class Renderer
{

public:
	Renderer(Window& inWindow);
	void Clean();
	void RenderFrame();
	void DrawSkybox();

	ID3D11Device* GetDevice() { return dev; }
	ID3D11DeviceContext* GetDeviceCon() { return devcon; }


	Camera camera;
	std::vector<GameObject*> gameObjects;
	GameObject* skyboxObject = nullptr;

	void RegisterGameObject(GameObject* e);
	void RemoveGameObject(GameObject* e);

	DirectX::XMVECTOR ambientLightColor{ 0.1f,0.1f,0.1f };
	DirectionalLight directionalLight{ {0.9f,0.8f,0.75f}, {0.9f,0.8f,0.75f} };
	PointLight pointLights[MAX_POINT_LIGHTS];

	
private:

	

	Window& window;
	IDXGISwapChain* swapchain = nullptr;
	ID3D11Device* dev = nullptr;
	ID3D11DeviceContext* devcon = nullptr;
	ID3D11RenderTargetView* backbuffer = nullptr;

	ID3D11VertexShader* pVS = nullptr; // Vertex Shader
	ID3D11PixelShader* pPS = nullptr; // Pixel Shader
	ID3D11InputLayout* pIL = nullptr; // Input layout
	
	// sky box stuff
	ID3D11VertexShader* pVSSkybox = nullptr;
	ID3D11PixelShader* pPSSkybox = nullptr;
	ID3D11InputLayout* pILSkybox = nullptr;

	ID3D11Buffer* vBuffer = nullptr; // Vertex buffer
	ID3D11Buffer* iBuffer = nullptr; // Index buffer
	ID3D11Buffer* cBuffer_PerObject = nullptr; // Constant Buffer
	ID3D11Buffer* cBuffer_Lighting = nullptr; // Constant Buffer - for lighting

	ID3D11DepthStencilView* depthBuffer = NULL;

	ID3D11RasterizerState* rasterizerCullBack = nullptr;
	ID3D11RasterizerState* rasterizerCallFront = nullptr;
	ID3D11RasterizerState* rasterizerCallNone = nullptr;

	ID3D11BlendState* blendOpaque = nullptr;
	ID3D11BlendState* blendTransparent = nullptr;

	ID3D11DepthStencilState* depthWriteOff = nullptr;

	DirectX::SpriteFont* font = nullptr;
	DirectX::SpriteBatch* spriteBatch = nullptr;

	long InitD3D();
	long InitPipeline();
	void InitGraphics();
	long InitDepthBuffer();

	void RenderText(const char* text, int x, int y);
};

