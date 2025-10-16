#include "Renderer.h"
#include <d3d11.h>

// Must be before #include <DirectXMath.h> - allows direct access of DirectX Math Matrices/Vectors at cost of performace
//#define _XM_NO_INTRINSICS_
//#define XM_NO_
#include <DirectXMath.h>
using namespace DirectX;


#include <d3dcompiler.h>
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

	// Select which primative we are using
	devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->Draw(3, 0);

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
	// Load and compile the vertex and pixel shaders
	HRESULT result;
	ID3DBlob* VS, *PS, *pErrorBlob;

	result = D3DCompileFromFile(L"VertexShader.hlsl", 0, 0, "main", "vs_4_0", 0, 0, &VS, &pErrorBlob);

	if (FAILED(result)) {
		LOG(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
		pErrorBlob->Release();
		return result;
	}

	result = D3DCompileFromFile(L"PixelShader.hlsl", 0, 0, "main", "ps_4_0", 0, 0, &PS, &pErrorBlob);

	if (FAILED(result)) {
		LOG(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
		pErrorBlob->Release();
		return result;
	}

	// Encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// TODO: add error handling to the two functions above.

	// Set shader objects as active shaders in the pipeline
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	//Create the input layout discription
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	result = dev->CreateInputLayout(ied, ARRAYSIZE(ied), VS->GetBufferPointer(), VS->GetBufferSize(), &pIL);
	VS->Release();
	PS->Release();

	if (FAILED(result)){
		LOG("Failed to create input layout");
		return result;
	}

	// should be in render loop
	devcon->IASetInputLayout(pIL);

	return S_OK;
}

void Renderer::InitGraphics()
{
	Vertex vertices[] = {
		{XMFLOAT3{-0.5f, -0.5f, 0.0f }, XMFLOAT4{Colors::Red}},
		{XMFLOAT3{ 0.0f,  0.5f, 0.0f }, XMFLOAT4{Colors::Lime}},
		{XMFLOAT3{ 0.5f, -0.5f, 0.0f }, XMFLOAT4{Colors::Blue}},
	};

	// Create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DYNAMIC; // Dynamic allows CPU-write and GPU-read
	bd.ByteWidth = sizeof(Vertex) * 3; // Size of buffer - sizeof vertex * num of vertices
	// bd.ByteWidth = sizeof(vertices); // Alternatively can also be this for simplicty but only on local scope
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Use as vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Create the buffer
	dev->CreateBuffer(&bd, NULL, &vBuffer); // Create the buffer

	if (vBuffer == 0) {
		return;
	}

	// Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // Map the buffer

	memcpy(ms.pData, vertices, sizeof(vertices)); // Copy the data into the buffer

	devcon->Unmap(vBuffer, NULL);

}
