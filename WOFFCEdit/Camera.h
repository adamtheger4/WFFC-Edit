#pragma once
#include "DisplayChunk.h"
#include "SceneObject.h"
#include "InputCommands.h"
#include <vector>

class Camera
{
public:
	Camera();
	~Camera();

	DirectX::SimpleMath::Matrix Update(InputCommands _InputCommands);

	//camera
	DirectX::SimpleMath::Vector3		m_camPosition;
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camFwd;
	DirectX::SimpleMath::Vector3		m_camRight;
	DirectX::SimpleMath::Vector3		m_camUp;
	float m_camRotRate;
	float m_movespeed;


};

