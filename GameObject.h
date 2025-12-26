#pragma once
#include <string>

#include "Debug.h"

#include "Transform.h"

class Mesh;
//class Texture;
class Material;

class GameObject
{
private:
	std::string name = "GameObject";

public:
	Transform transform;

	Mesh* mesh;

	//Texture* texture;
	Material* material;

	std::string GetName() { return name; }

	GameObject(std::string objectName, Mesh* objectMesh, Material* material);
};

