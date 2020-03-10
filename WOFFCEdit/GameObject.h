#pragma once
#include "SceneObject.h"
#include "pch.h"

class GameObject : public SceneObject
{
public:
	GameObject();
	~GameObject();

	DirectX::BoundingSphere bounding_sphere;
};

