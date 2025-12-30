#include "Material_Lit.h"

#include <d3d11.h>

#include "GameObject.h"
#include "Texture.h"
#include "Renderer.h"

Material_Lit::Material_Lit(std::string name, Renderer& renderer, std::string vShaderFilename, std::string pShaderFilename, Texture* texture)
	: Material(name, renderer, vShaderFilename, pShaderFilename, texture)
{
	CreateCBuffer(sizeof(CBufferLighting), cbuffer); // Slot 0 VS CB
	CreateCBuffer(sizeof(CBufferPS), cbufferPixelShader); // Slot 0 PS CB
}

void Material_Lit::Bind()
{
	// Call parent bind function to bind base material shader and inputs.
	Material::Bind();

	// Not needed here as it's set by the base material bind function above
	//devcon->VSSetConstantBuffers(0, 1, &cbuffer);

	// Bind pixel shader CBuffer
	if (cbufferPixelShader != nullptr) {
		devcon->PSSetConstantBuffers(0, 1, &cbufferPixelShader);
	}

	// Skybox reflection texture
	if (skyboxTexture != nullptr) {
		ID3D11ShaderResourceView* t = skyboxTexture->GetTexture();
		devcon->PSSetShaderResources(1, 1, &t);
	}
}

void Material_Lit::UpdateMaterial(GameObject* gameObject)
{
	using namespace DirectX;
	Material::UpdateMaterial(gameObject);

	CBufferLighting cbData;
	//Ambient light
	cbData.ambientLight = renderer.ambientLightColor;
	// Directional light
	DirectionalLight& dirLight = renderer.directionalLight;
	cbData.directionalLight.color = dirLight.color;
	XMMATRIX transpose = XMMatrixTranspose(gameObject->transform.GetWorldMatrix()); // Transpose rotations
	cbData.directionalLight.transposedDirection = XMVector3Transform(XMVector3Normalize(dirLight.directionFrom), transpose);

	// point lights
	PointLight* pointLights = renderer.pointLights;
	for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i) {
		cbData.pointLights[i].enabled = pointLights[i].enabled;

		if (!pointLights[i].enabled)
			continue; // Skip disabled lights

		XMMATRIX inverse = XMMatrixInverse(nullptr, gameObject->transform.GetWorldMatrix());

		cbData.pointLights[i].position = XMVector3Transform(pointLights[i].position, inverse);
		cbData.pointLights[i].color = pointLights[i].color;
		cbData.pointLights[i].strength = pointLights[i].strength;
	}

	UpdateCBuffer(cbData, cbuffer);

	// Pixel Shader
	CBufferPS cbpsData;
	cbpsData.reflectiveness = reflectiveness;
	UpdateCBuffer(cbpsData, cbufferPixelShader);
}
