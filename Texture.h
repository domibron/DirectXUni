#pragma once

#include <string>

class Renderer;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

class Texture
{
private:
	ID3D11ShaderResourceView* texture = nullptr; // Texture
	ID3D11SamplerState* sampler = nullptr; // Sampler.

public:
	ID3D11ShaderResourceView* GetTexture() { return texture; }
	ID3D11SamplerState* GetSampler() { return sampler; }
	Texture(Renderer& renderer, std::string path, bool transparent = false);
	~Texture();

	bool isTransparent = false;
};

