#include "BlockMesh.h"

#include <d3d11.h>

#include "Renderer.h"
#include "Debug.h"

struct Vertex {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
};

BlockMesh::BlockMesh(Renderer& renderer)
	: dev(renderer.GetDevice()), devcon(renderer.GetDeviceCon())
{
	using namespace DirectX;

	Vertex topVerticies[] = {
		{XMFLOAT3{0.0f, 1.0f, 1.0f},XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{0.0f, 0.5f, 1.0f} }, // top down, this is top left.
		{XMFLOAT3{1.0f, 1.0f, 1.0f},XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{0.0f, 0.5f, 1.0f} }, // top down, this is top right.
		{XMFLOAT3{0.0f, 1.0f, 0.0f},XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{0.0f, 0.5f, 1.0f} }, // top down, this is bottom left.
		{XMFLOAT3{1.0f, 1.0f, 0.0f},XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{0.0f, 0.5f, 1.0f} }, // top down, this is bottom right.
	};

}
