#include "Renderer.h"
#include <d3d11.h>

// Must be before #include <DirectXMath.h> - allows direct access of DirectX Math Matrices/Vectors at cost of performace
//#define _XM_NO_INTRINSICS_
//#define XM_NO_
#include <DirectXMath.h>
using namespace DirectX;


//#include <d3dcompiler.h> // this was replaced with shader loading.
#include "ShaderLoading.h"
#include "Debug.h"
#include "Window.h"

#include <DirectXColors.h>

#include "ModelLoader.h"

#include "Mesh.h"

#include "GameObject.h"

#include "Texture.h"

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct CBuffer_PerObject {
	XMMATRIX WVP; // 64 byte world matrix.
	// The 64 comes from each row being 16 bytes
	// and 4 rows in total. 4 * 16 = 64 bytes
	// Each '4' us a 4 byte float value
	// 4,4,4,4
	// 4,4,4,4
	// 4,4,4,4
	// 4,4,4,4
	// XMMATRIX is a strictly aligned type for SIMD hardware
	// Single Instruction, Multiple Data
};

Renderer::Renderer(Window& inWindow)
	: window(inWindow)
{
	if (InitD3D() != S_OK) {
		LOG("Failed to initialise D3D renderer.");
		return;
	}

	if (InitPipeline() != S_OK) {
		LOG("Failed to initialise shader pipeline.");
		return;
	}

	InitGraphics();
}

void Renderer::Clean()
{
	delete font;
	delete spriteBatch;

	if (depthBuffer) depthBuffer->Release();

	if (cBuffer_PerObject) cBuffer_PerObject->Release();
	if (iBuffer) iBuffer->Release();
	if (vBuffer) vBuffer->Release();

	if (pVS) pVS->Release();
	if (pPS) pPS->Release();
	if (pIL) pIL->Release();

	if (backbuffer) backbuffer->Release();
	if (swapchain) swapchain->Release();
	if (dev) dev->Release();
	if (devcon) devcon->Release();
}

void Renderer::RenderFrame()
{
	// C;ear back buffer with desired color
	//FLOAT bg[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	devcon->ClearRenderTargetView(backbuffer, DirectX::Colors::DarkSlateGray);
	devcon->ClearDepthStencilView(depthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	//auto t = texture->GetTexture();
	//devcon->PSSetShaderResources(0, 1, &t);
	//auto s = texture->GetSampler();
	//devcon->PSSetSamplers(0, 1, &s);
	

	CBuffer_PerObject cBufferData;
	cBufferData.WVP = XMMatrixIdentity();
	XMMATRIX view = camera.GetViewMatrix();
	XMMATRIX projection = camera.GetProjectionMatrix(window.GetWidth(), window.GetHeight());

	// this fixes the font fucking up the shaders and such.
	devcon->IASetInputLayout(pIL);
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	for (auto go : gameObjects) {
	
		XMMATRIX world = go->transform.GetWorldMatrix();
		cBufferData.WVP = world * view * projection;
		devcon->UpdateSubresource(cBuffer_PerObject, NULL, NULL, &cBufferData, NULL, NULL);
		devcon->VSSetConstantBuffers(0, 1, &cBuffer_PerObject);
		
		auto t = go->texture->GetTexture();
		devcon->PSSetShaderResources(0, 1, &t);
		auto s = go->texture->GetSampler();
		devcon->PSSetSamplers(0, 1, &s);

		devcon->RSSetState(go->mesh->isDoubleSideded ? rasterizerCallNone : rasterizerCullBack);

		// NOTE: pixel shader cuts the transparancy, this means that we cannot achive windows or translucent materails without a rework to current code, remove clip from pixel
		// and un comment the below lines, then add a transparancy check to render transparent objects over opaque objects.
		//devcon->OMSetBlendState(go->texture->isTransparent ? blendTransparent : blendOpaque, 0, 0xffffffff);

		//devcon->OMSetDepthStencilState(go->texture->isTransparent ? depthWriteOff : nullptr, 1);

		go->mesh->Render();
	}

	// Since the camera doesn’t move between rendering different objects on the same frame, we can calculate the view and
	// projection matrices outside of the for loop.
	
	RenderText("Hello World", 100, 100);

	// Flip the back and front buffers around. Display on screen
	swapchain->Present(0, 0);
}


void Renderer::RegisterGameObject(GameObject* e)
{
	gameObjects.push_back(e);
	LOG("Registered " + e->GetName() + ".");
}

void Renderer::RemoveGameObject(GameObject* e)
{
	auto foundEntity = std::find(gameObjects.begin(), gameObjects.end(), e);
	if (foundEntity != gameObjects.end()) {
		gameObjects.erase(foundEntity);
	}
	// Note: will affect index-based iterating // I presume this means this edits memory at runtime and is not thread safe.
}

long Renderer::InitD3D()
{
	// Create a struct to hold the information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd = {}; // sets everything to 0 to remove garb vals
	scd.BufferCount = 1; // One back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32-bit color
	scd.BufferDesc.Width = window.GetWidth(); // set the back buffer width
	scd.BufferDesc.Height = window.GetHeight(); // set the back buffer height
	scd.BufferDesc.RefreshRate.Numerator = 60; //60 FPS
	scd.BufferDesc.RefreshRate.Denominator = 1; // 60/1 = 60 FPS
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Intended swapchain use
	scd.OutputWindow = window.GetHandle(); // Window to use
	scd.SampleDesc.Count = 1; // Number of samples for AA
	scd.Windowed = TRUE; // Windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Allow full-scrren switching

	HRESULT hr;
	// Create a swap chain, device and device context from the scd
	hr = D3D11CreateDeviceAndSwapChain(NULL, // Use default graphics adapter
		D3D_DRIVER_TYPE_HARDWARE, // Use hardware acceleration, can also use software or WARP renderers
		NULL, // Used for software driver types
		D3D11_CREATE_DEVICE_DEBUG, // Flags can be OR'd together. We are enabling debug for better warnings and errors
		NULL, // Direct3D feature levels. NULL will use D3D11.0 or older
		NULL, // Size of array passed to above member - NULL since we passed NULL
		D3D11_SDK_VERSION, // Always set to D3D11_SDK_VERSION
		&scd, // Pointer to our swap chain description
		&swapchain, // Pointer to our swap chain COM object
		&dev, // Pointer to our device
		NULL, // Out param - will be set to chosen feature level
		&devcon); // Pointer to our immediate device context

	if (FAILED(hr)) {
		LOG("Failed to create a renderer");
		return hr;
	}

	// Get the address of teh back buffer
	ID3D11Texture2D* backBufferTexture = nullptr;

	// Get the back buffer from the swapchain
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);

	if (FAILED(hr)) {
		LOG("Failed to get backbuffer texture.");
		return hr; // Abort
	}

	hr = dev->CreateRenderTargetView(backBufferTexture, NULL, &backbuffer);
	// dangling pointer :3
	backBufferTexture->Release();// yeet back into the void, we dont need you anymore.
	if (FAILED(hr)) {
		LOG("Failed to create backbuffer view.");
		return hr;
	}

	hr = InitDepthBuffer();
	if (FAILED(hr)) {
		LOG("Failed to create depth buffer.");
		return hr;
	}

	// Set the back buffer as the current render target
	devcon->OMSetRenderTargets(1, &backbuffer, depthBuffer);

	// Define and set the viewport
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)window.GetWidth();
	viewport.Height = (float)window.GetHeight();
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	devcon->RSSetViewports(1, &viewport);

	return S_OK;
}

long Renderer::InitPipeline()
{
	ShaderLoading::LoadVertexShader("Compiled Shaders/VertexShader.cso", dev, &pVS, &pIL);
	ShaderLoading::LoadPixelShader("Compiled Shaders/PixelShader.cso", dev, &pPS);

	// TODO: add error handling to the two functions above.
	// Set shader objects as active shaders in the pipeline
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);



	// should be in render loop
	devcon->IASetInputLayout(pIL);

	return S_OK;
}

void Renderer::InitGraphics()
{

	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(CBuffer_PerObject);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	if (FAILED(dev->CreateBuffer(&cbd, NULL, &cBuffer_PerObject))) {
		LOG("Oops, failed to create CBuffer.");
		return;
	}

	// Setup raster states
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	//rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	// Create no culling rasterizer
	dev->CreateRasterizerState(&rsDesc, &rasterizerCallNone);
	// Create backface culling rasterizer
	rsDesc.CullMode = D3D11_CULL_BACK;
	dev->CreateRasterizerState(&rsDesc, &rasterizerCullBack);

	// Setup transparancy
	D3D11_BLEND_DESC bdDesc = { 0 };
	bdDesc.IndependentBlendEnable = FALSE;
	bdDesc.AlphaToCoverageEnable = FALSE;
	bdDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bdDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bdDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bdDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bdDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bdDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bdDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bdDesc.RenderTarget[0].BlendEnable = TRUE;
	dev->CreateBlendState(&bdDesc, &blendTransparent);
	bdDesc.RenderTarget[0].BlendEnable = FALSE;
	dev->CreateBlendState(&bdDesc, &blendOpaque);

	// Setup depth write off state
	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dev->CreateDepthStencilState(&dsDesc, &depthWriteOff);

	// Fonts
	font = new DirectX::SpriteFont(dev, L"Assets/arial.spritefont");
	spriteBatch = new DirectX::SpriteBatch(devcon);

}

long Renderer::InitDepthBuffer()
{
	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC scd = {};
	swapchain->GetDesc(&scd);

	// Z-Buffer texture description
	D3D11_TEXTURE2D_DESC tex2dDesc = { 0 };
	tex2dDesc.Width = window.GetWidth();
	tex2dDesc.Height = window.GetHeight();
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = scd.SampleDesc.Count;
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

	// Z-Buffer texture
	ID3D11Texture2D* zBufferTexture;
	hr = dev->CreateTexture2D(&tex2dDesc, NULL, &zBufferTexture);
	if (FAILED(hr)) {
		OutputDebugString(L"Failed to create Z-Buffer Texture!");
		return E_FAIL;
	}

	// Create the depth buffer view
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = dev->CreateDepthStencilView(zBufferTexture, &dsvDesc, &depthBuffer);
	if (FAILED(hr)) {
		OutputDebugString(L"Failed to create depth stencil view!");
		return E_FAIL;
	}
	zBufferTexture->Release();
	return S_OK;
}

void Renderer::RenderText(const char* text, int x, int y)
{
	// Get current depth stencil
	ID3D11DepthStencilState* depthState;
	devcon->OMGetDepthStencilState(&depthState, 0);

	// Draw text - Rendering the font will change our device context. This is why depthState is used.
	spriteBatch->Begin();
	font->DrawString(spriteBatch, text, DirectX::XMFLOAT2(x, y));
	spriteBatch->End();

	// restore previous depth stencil
	devcon->OMSetDepthStencilState(depthState, 0);
}
