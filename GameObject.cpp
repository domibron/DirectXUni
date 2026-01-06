#include "GameObject.h"


#include "Mesh.h"

GameObject::GameObject(std::string objectName, Mesh* objectMesh, Material* material)
	: name(objectName), mesh(objectMesh), material(material)
{
}

void GameObject::RenderObject()
{
	mesh->Render();
}
