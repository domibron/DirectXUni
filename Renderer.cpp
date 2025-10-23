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

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
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
	FLOAT bg[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	devcon->ClearRenderTargetView(backbuffer, bg);

	// Select which vertex buffer to use
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Select which primative we are using
	devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->DrawIndexed(36, 0, 0); // 36 is the number of indicies.s

	// Alternatively, include <DirectXColors.h> and do
	//devcon->ClearRenderTargetView(backbuffer, DirectX::Colors::DarkSlateGray);
	// You can press F12 on the Colors or DarkSlateGray to see a list of all colours
	// Adding a using namespace DirectX will make it less cumbersome to use this

	// Flip the back and front buffers around. Display on screen
	swapchain->Present(0, 0);
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

	// Set the back buffer as the current render target
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

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
	// for when camera can be moved.
	//Vertex vertices[] = {
	//	{XMFLOAT3{-0.5f, -0.5f, -0.5f }, XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}}, // Front BL
	//	{XMFLOAT3{-0.5f,  0.5f, -0.5f }, XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}}, // Front TL
	//	{XMFLOAT3{ 0.5f,  0.5f, -0.5f }, XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}}, // Front TR
	//	{XMFLOAT3{ 0.5f, -0.5f, -0.5f }, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}}, // Front BR

	//	{XMFLOAT3{-0.5f, -0.5f,  0.5f }, XMFLOAT4{0.0f, 1.0f, 1.0f, 1.0f}}, // Back  BL
	//	{XMFLOAT3{-0.5f,  0.5f,  0.5f }, XMFLOAT4{1.0f, 0.0f, 1.0f, 1.0f}}, // Back  TL
	//	{XMFLOAT3{ 0.5f,  0.5f,  0.5f }, XMFLOAT4{1.0f, 1.0f, 0.0f, 1.0f}}, // Back  TR
	//	{XMFLOAT3{ 0.5f, -0.5f,  0.5f }, XMFLOAT4{0.0f, 0.0f, 0.0f, 1.0f}}, // Back  BR
	//};

	Vertex vertices[] = {
		{XMFLOAT3{-0.5f, -0.5f,  0.5f }, XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}}, // Front BL
		{XMFLOAT3{-0.5f,  0.5f,  0.5f }, XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}}, // Front TL
		{XMFLOAT3{ 0.5f,  0.5f,  0.5f }, XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}}, // Front TR
		{XMFLOAT3{ 0.5f, -0.5f,  0.5f }, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}}, // Front BR

		{XMFLOAT3{-0.5f, -0.5f,  1.5f }, XMFLOAT4{0.0f, 1.0f, 1.0f, 1.0f}}, // Back  BL
		{XMFLOAT3{-0.5f,  0.5f,  1.5f }, XMFLOAT4{1.0f, 0.0f, 1.0f, 1.0f}}, // Back  TL
		{XMFLOAT3{ 0.5f,  0.5f,  1.5f }, XMFLOAT4{1.0f, 1.0f, 0.0f, 1.0f}}, // Back  TR
		{XMFLOAT3{ 0.5f, -0.5f,  1.5f }, XMFLOAT4{0.0f, 0.0f, 0.0f, 1.0f}}, // Back  BR
	};

	

	

	// Create an index buffer
	unsigned int indices[] = { /*front*/ 0,1,2,2,3,0, /*back*/ 7,6,5,5,4,7, /*left*/ 4,5,1,1,0,4,
		/*right*/ 3,2,6,6,7,3, /*top*/ 1,5,6,6,2,1, /*bottom*/ 4,0,3,3,7,4 };

	// Fill in a buffer description
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(indices);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// Defube the resource data.
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = indices;

	// Create the buffer with the decvice
	if (FAILED(dev->CreateBuffer(&bufferDesc, &initData, &iBuffer))) {
		LOG("Failed to create index buffer");
		return;
	}


	// Create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DYNAMIC; // Dynamic allows CPU-write and GPU-read
	bd.ByteWidth = sizeof(vertices); // Size of buffer
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Use as vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Create the buffer
	if (FAILED(dev->CreateBuffer(&bd, NULL, &vBuffer))) // Create the buffer
	{
		LOG("Failed to create vertex buffer.");
		return;
	}

	if (vBuffer == 0) {
		return;
	}


	// Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // Map the buffer

	memcpy(ms.pData, vertices, sizeof(vertices)); // Copy the data into the buffer

	devcon->Unmap(vBuffer, NULL);

}
