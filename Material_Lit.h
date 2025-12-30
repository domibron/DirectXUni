#pragma once

#include "Material.h"

#include "Lighting.h"


class Material_Lit :
    public Material
{

protected:
    Texture* skyboxTexture = nullptr;
    ID3D11Buffer* cbufferPixelShader = nullptr;

    struct DirectionalLightingCBData {
        DirectX::XMVECTOR transposedDirection;
        DirectX::XMVECTOR color;
    };

    struct PointLightCBData {
        DirectX::XMVECTOR position; // 16
        DirectX::XMVECTOR color; // 16
        float strength = 1; // 4
        unsigned int enabled = 0; // 4
        DirectX::XMFLOAT2 padding; // 8
    };

    struct CBufferLighting : CBufferBase {
        DirectX::XMVECTOR ambientLight{ 1,1,1,1 };
        DirectionalLightingCBData directionalLight;
        PointLightCBData pointLights[MAX_POINT_LIGHTS];
    };

    struct CBufferPS : CBufferBase {
        float reflectiveness; // 4
        DirectX::XMFLOAT3 padding; // 12
    };

public:
    float reflectiveness = 0.1f;

    Material_Lit(std::string name, Renderer& renderer, std::string vShaderFilename, std::string pShaderFilename, Texture* texture);

    virtual void Bind() override;

    virtual void UpdateMaterial(GameObject* gameObject) override;

    void SetReflectionTexture(Texture* reflectionTexture) { skyboxTexture = reflectionTexture; }
};

