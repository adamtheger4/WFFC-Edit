#include "GameObject.h"

GameObject::GameObject()
{
	bounding_sphere.Center = DirectX::XMFLOAT3{ 0, 0, 0 };
	bounding_sphere.Radius = 1.0f;
}

GameObject::~GameObject()
{
}