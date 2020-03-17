#include "DisplayObject.h"

DisplayObject::DisplayObject()
{
	m_model = NULL;
	m_texture_diffuse = NULL;
	m_orientation.x = 0.0f;
	m_orientation.y = 0.0f;
	m_orientation.z = 0.0f;
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;
	m_scale.x = 0.0f;
	m_scale.y = 0.0f;
	m_scale.z = 0.0f;
	m_render = true;
	m_wireframe = false;

	m_light_type =0;
	m_light_diffuse_r = 0.0f;	m_light_diffuse_g = 0.0f;	m_light_diffuse_b = 0.0f;
	m_light_specular_r = 0.0f;	m_light_specular_g = 0.0f;	m_light_specular_b = 0.0f;
	m_light_spot_cutoff = 0.0f;
	m_light_constant = 0.0f;
	m_light_linear = 0.0f;
	m_light_quadratic = 0.0f;
}


DisplayObject::~DisplayObject()
{
//	delete m_texture_diffuse;
}

void DisplayObject::SetPosition(DirectX::SimpleMath::Vector3 in_vector)
{
	m_position =  in_vector;
}

void DisplayObject::SetRotation(DirectX::SimpleMath::Vector3 in_vector)
{
	m_orientation = in_vector;
}

void DisplayObject::SetScale(DirectX::SimpleMath::Vector3 in_vector)
{
	m_scale = in_vector;
}

void DisplayObject::MoveObject(DirectX::SimpleMath::Vector3 in_vector, float dt)
{
	m_position.x += dt * (move_scalar * in_vector.x);
	m_position.y += dt * (move_scalar * in_vector.y);
	m_position.z += dt * (move_scalar * in_vector.z);
}

void DisplayObject::RotateObject(DirectX::SimpleMath::Vector3 in_vector, float dt)
{
	m_orientation.x += dt * (move_scalar * in_vector.x);
	m_orientation.y += dt * (move_scalar * in_vector.y);
	m_orientation.z += dt * (move_scalar * in_vector.z);
}

void DisplayObject::ScaleObject(DirectX::SimpleMath::Vector3 in_vector, float dt)
{
	m_scale.x += dt * (move_scalar * in_vector.x);
	m_scale.y += dt * (move_scalar * in_vector.y);
	m_scale.z += dt * (move_scalar * in_vector.z);
}
