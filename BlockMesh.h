#pragma once

#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

class Renderer;

class BlockMesh
{
private:
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	ID3D11Buffer* vBuffer = NULL; // Vertex Buffer
	ID3D11Buffer* topiBuffer = NULL; // Index Buffer
	ID3D11Buffer* bottomiBuffer = NULL; // Index Buffer
	ID3D11Buffer* frontiBuffer = NULL; // Index Buffer
	ID3D11Buffer* backiBuffer = NULL; // Index Buffer
	ID3D11Buffer* leftiBuffer = NULL; // Index Buffer
	ID3D11Buffer* rightiBuffer = NULL; // Index Buffer

	unsigned int indexCount = 0;
public:
	BlockMesh(Renderer& renderer);
	void Render();
};

