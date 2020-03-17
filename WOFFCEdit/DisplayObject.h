#pragma once
#include "pch.h"


class DisplayObject
{
public:
	DisplayObject();
	~DisplayObject();

	std::shared_ptr<DirectX::Model>						m_model;							//main Mesh
	ID3D11ShaderResourceView *							m_texture_diffuse;					//diffuse texture


	int m_ID;
	DirectX::SimpleMath::Vector3			m_position;
	DirectX::SimpleMath::Vector3			m_orientation;
	DirectX::SimpleMath::Vector3			m_scale;
	bool									m_render;
	bool									m_wireframe;

	int		m_light_type;
	float	m_light_diffuse_r,	m_light_diffuse_g,	m_light_diffuse_b;
	float	m_light_specular_r, m_light_specular_g, m_light_specular_b;
	float	m_light_spot_cutoff;
	float	m_light_constant;
	float	m_light_linear;
	float	m_light_quadratic;

	float move_scalar = 30;

	void SetPosition(DirectX::SimpleMath::Vector3 in_vector); // offsets current position by input vector
	void SetRotation(DirectX::SimpleMath::Vector3 in_vector); // offsets current position by input vector
	void SetScale(DirectX::SimpleMath::Vector3 in_vector); // offsets current position by input vector

	void MoveObject(DirectX::SimpleMath::Vector3 in_vector, float dt); // offsets current position by input vector
	void RotateObject(DirectX::SimpleMath::Vector3 in_vector, float dt); // offsets current rotation by input vector
	void ScaleObject(DirectX::SimpleMath::Vector3 in_vector, float dt); // offsets current scale by input vector
};

