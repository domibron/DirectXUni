#include "Mesh.h"

#include <d3d11.h>

#include "Renderer.h"
#include "ModelLoader.h"
#include "Debug.h"

Mesh::Mesh(Renderer& renderer, std::string objPath)
	: dev(renderer.GetDevice()), devcon(renderer.GetDeviceCon())
{
	ModelLoader ml{ objPath };

	// Create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DYNAMIC; // Dynamic allows CPU-write and GPU-read
	bd.ByteWidth = (unsigned int)ml.GetVertexBufferSize(); // Size of buffer
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

	memcpy(ms.pData, ml.GetVertexData(), ml.GetVertexBufferSize()); // Copy the data into the buffer
	devcon->Unmap(vBuffer, NULL); // Unmap the buffer

	// Fill in a buffer description
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = ml.GetIndexBufferSize();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// Defube the resource data.
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = ml.GetIndexData();

	indexCount = (int)ml.GetIndexCount();

	// Create the buffer with the decvice
	if (FAILED(dev->CreateBuffer(&bufferDesc, &initData, &iBuffer))) {
		LOG("Failed to create index buffer");
		return;
	}
}

void Mesh::Render()
{
	// Select which primative we are using
	devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Select which vertex buffer to use
	UINT stride = sizeof(VertexPosUVNorm);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);

	devcon->DrawIndexed(indexCount, 0, 0);
}
