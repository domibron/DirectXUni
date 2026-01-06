#include "BlockMesh.h"

#include <d3d11.h>

#include "Renderer.h"
#include "ModelLoader.h"
#include "Debug.h"

void BlockMesh::Render()
{
	// Select which primative we are using
	devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Select which vertex buffer to use
	UINT stride = sizeof(VertexPosUVNorm);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);

	// all faces need 6 for a square grouping. so we are working with offests of 6.
	// first 6 (0) is the face away from camera. so positive z.
	// second (6) is the top face.
	// third (12) is the face towards cam or negative z.
	// fourth (18) is the bottom face.
	// fifth (24) is the right face.
	// sixth (30) is the left face

	if (backFace) {
		devcon->DrawIndexed(6, 0, 0);
	}

	if (topFace) {
		devcon->DrawIndexed(6, 6, 0);
	}

	if (frontFace) {
		devcon->DrawIndexed(6, 12, 0);
	}

	if (bottomFace) {
		devcon->DrawIndexed(6, 18, 0);
	}

	if (rightFace) {
		devcon->DrawIndexed(6, 24, 0);
	}

	if (leftFace) {
		devcon->DrawIndexed(6, 30, 0);
	}
}

void BlockMesh::SetVisibleFaces(bool back, bool top, bool front, bool bottom, bool right, bool left)
{
	backFace = back;
	topFace = top;
	frontFace = front;
	bottomFace = bottom;
	rightFace = right;
	leftFace = left;
}

BlockMesh::BlockMesh(Renderer& renderer)
	: Mesh(renderer, "Assets/block.obj", false)
{

}
