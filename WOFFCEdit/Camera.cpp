#include "Camera.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;



Camera::Camera()
{
	//functional
	m_movespeed = 0.30;
	m_camRotRate = 0.01;

	//camera
	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0;
	m_camOrientation.y = 0;
	m_camOrientation.z = 0;

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 1.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	m_camUp.x = 0.0f;
	m_camUp.y = 0.0f;
	m_camUp.z = 1.0f;

	m_camFwd.x = 0.0f;
	m_camFwd.y = 1.0f;
	m_camFwd.z = 0.0f;

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;
}


Camera::~Camera()
{
}

DirectX::SimpleMath::Matrix Camera::Update(InputCommands _InputCommands)
{
	if (_InputCommands.rotRight)
	{
		m_camOrientation.y -= m_camRotRate * _InputCommands.camRotate ;
	}
	if (_InputCommands.rotLeft)
	{
		m_camOrientation.y += m_camRotRate * _InputCommands.camRotate;
	}
	if (_InputCommands.rotUp)
	{
		m_camOrientation.z += m_camRotRate * _InputCommands.camRotate;
	}
	if (_InputCommands.rotDown)
	{
		m_camOrientation.z -= m_camRotRate * _InputCommands.camRotate;
	}
	if (_InputCommands.rollRight)
	{
		m_camOrientation.x -= m_camRotRate;
	}
	if (_InputCommands.rollLeft)
	{
		m_camOrientation.x -= m_camRotRate;
	}

	if (m_camOrientation.z < -1.5608)
	{
		m_camOrientation.z = -1.5508;
	}
	if (m_camOrientation.z > 1.5608)
	{
		m_camOrientation.z = 1.5508;
	}

	m_camLookDirection.x *= 3.1415 / 180;
	m_camLookDirection.y *= 3.1415 / 180;
	m_camLookDirection.z *= 3.1415 / 180;

	m_camLookDirection.x = sin(m_camOrientation.y) * cos(m_camOrientation.z);
	m_camLookDirection.y = sin(m_camOrientation.z);// *  cos(m_camOrientation.y);
	m_camLookDirection.z = cos(m_camOrientation.y) * cos(m_camOrientation.z);
	m_camLookDirection.Normalize();


	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);
	m_camRight.Normalize();

	//create up vector from look Direction
	m_camUp.x = (float)(sin(m_camOrientation.y) * sin(m_camOrientation.z) * -1);
	m_camUp.y = (float)(cos(m_camOrientation.z));
	m_camUp.z = (float)(cos(m_camOrientation.y) * sin(m_camOrientation.z) * -1);
	m_camUp.Normalize();

	//process input and update stuff
	if (_InputCommands.forward)
	{
		m_camPosition += m_camLookDirection * m_movespeed;
	}
	if (_InputCommands.back)
	{
		m_camPosition -= m_camLookDirection * m_movespeed;
	}
	if (_InputCommands.right)
	{
		m_camPosition += m_camRight * m_movespeed;
	}
	if (_InputCommands.left)
	{
		m_camPosition -= m_camRight * m_movespeed;
	}
	if (_InputCommands.up)
	{
		m_camPosition += m_camUp * m_movespeed;
	}
	if (_InputCommands.down)
	{
		m_camPosition -= m_camUp * m_movespeed;
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;

	//apply camera vectors
	DirectX::SimpleMath::Matrix	m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);

	return m_view;
}
