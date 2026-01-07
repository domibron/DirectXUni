#define MAX_POINT_LIGHTS 8

#pragma once

#include "Transform.h"
#include "Camera.h"
#include <vector>
#include <SpriteFont.h>

#include "Lighting.h"


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


	Camera* camera;
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


	ID3D11Buffer* cBuffer_PerObject = nullptr; // Constant Buffer
	ID3D11Buffer* cBuffer_PerFrame = nullptr;
	//ID3D11Buffer* cBuffer_Lighting = nullptr; // Constant Buffer - for lighting

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
	void InitGraphics();
	long InitDepthBuffer();
	void DrawSkybox();

	void RenderText(const char* text, int x, int y);
};

