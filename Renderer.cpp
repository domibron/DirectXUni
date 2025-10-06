#include "Renderer.h"
#include <d3d11.h>

#include "Debug.h"
#include "Window.h"

Renderer::Renderer(Window& inWindow)
	: window(inWindow)
{
	if (InitD3D() != S_OK) {
		LOG("Failed to initialise D3D renderer.");
		return;
	}
}

void Renderer::Clean()
{
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
	scd.BufferDesc.Width = window.GetHeight(); // set the back buffer width
	scd.BufferDesc.Height = window.GetWidth(); // set the back buffer height
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
