#include "Camera.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

Camera::Camera()
{
	//functional
	m_movespeed = 10.0f;
	m_camRotRate = 1.0f;

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

DirectX::SimpleMath::Matrix Camera::Update(float dt, InputCommands _InputCommands)
{
	if (m_arcBallMovement == false)
	{
		if (_InputCommands.rotRight)
		{
			m_camOrientation.y -= dt * (m_camRotRate * _InputCommands.camRotateX);
		}
		if (_InputCommands.rotLeft)
		{
			m_camOrientation.y += dt * (m_camRotRate * _InputCommands.camRotateX);
		}
		if (_InputCommands.rotUp)
		{
			if (m_camOrientation.z < 1.5508)
			{
				m_camOrientation.z += dt * (m_camRotRate *_InputCommands.camRotateY);

				if (m_camOrientation.z > 1.5508)
				{
					m_camOrientation.z = 1.5508;
				}
			}
		}
		if (_InputCommands.rotDown)
		{
			if (m_camOrientation.z > -1.5508)
			{
				m_camOrientation.z -= dt * (m_camRotRate * _InputCommands.camRotateY);

				if (m_camOrientation.z < -1.5508)
				{
					m_camOrientation.z = -1.5508;
				}
			}
		}

		m_camLookDirection.x = sin(m_camOrientation.y) * cos(m_camOrientation.z);
		m_camLookDirection.y = sin(m_camOrientation.z) * cos(m_camOrientation.x);
		m_camLookDirection.z = cos(m_camOrientation.y) * cos(m_camOrientation.z);

		m_camLookDirection.Normalize();
	}
	else
	{
		Vector3 m_camOrientation = (m_arcBallOrigin - m_camPosition);

		m_camLookDirection.x = m_camOrientation.x;
		m_camLookDirection.y = m_camOrientation.y;
		m_camLookDirection.z = m_camOrientation.z;
		m_camLookDirection.Normalize();	
	}

	//Set the forward vector to be the look direction
	m_camFwd = m_camLookDirection;
	m_camFwd.Normalize();

	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);
	m_camRight.Normalize();

	//Get the up vector using the right vector.
	m_camLookDirection.Cross(-m_camRight, m_camUp);
	m_camUp.Normalize();

	//process input and update stuff
	if (m_arcBallMovement == false)
	{
		if (_InputCommands.forward)
		{
			m_camPosition += dt * (m_camFwd * (m_movespeed * _InputCommands.camMove));
		}
		if (_InputCommands.back)
		{
			m_camPosition -= dt * (m_camFwd * (m_movespeed * _InputCommands.camMove));
		}
		if (_InputCommands.right)
		{
			m_camPosition += dt * (m_camRight * (m_movespeed * _InputCommands.camMove));
		}
		if (_InputCommands.left)
		{
			m_camPosition -= dt * (m_camRight * (m_movespeed * _InputCommands.camMove));
		}
		if (_InputCommands.up)
		{
			m_camPosition += dt * (m_camUp * (m_movespeed * _InputCommands.camMove));
		}
		if (_InputCommands.down)
		{
			m_camPosition -= dt * (m_camUp * (m_movespeed * _InputCommands.camMove));
		}
	}
	else // ARC BALL CONTROLS.
	{
		if (_InputCommands.forward || _InputCommands.scforward)
		{
			m_camPosition += dt * (m_camFwd * (m_movespeed * _InputCommands.camMove));
		}
		if (_InputCommands.back || _InputCommands.scback)
		{
			m_camPosition -= dt * (m_camFwd * (m_movespeed * _InputCommands.camMove));
		}
		if (_InputCommands.right)
		{
			m_camPosition += (m_camRight * (m_movespeed * _InputCommands.camRotateX * 0.1f));

			m_camLookDirection.x = (m_arcBallOrigin - m_camPosition).x;
			m_camLookDirection.y = (m_arcBallOrigin - m_camPosition).y;
			m_camLookDirection.z = (m_arcBallOrigin - m_camPosition).z;
			m_camLookDirection.Normalize();
		}
		if (_InputCommands.left)
		{
			m_camPosition -= (m_camRight * (m_movespeed * _InputCommands.camRotateX * 0.1f));

			m_camLookDirection.x = (m_arcBallOrigin - m_camPosition).x;
			m_camLookDirection.y = (m_arcBallOrigin - m_camPosition).y;
			m_camLookDirection.z = (m_arcBallOrigin - m_camPosition).z;
			m_camLookDirection.Normalize();
		}
		if (_InputCommands.up)
		{
			if (m_camLookDirection.y >= -0.9f) 	m_camPosition +=  (m_camUp * (m_movespeed * _InputCommands.camRotateY * 0.1f));		

			m_camLookDirection.x = (m_arcBallOrigin - m_camPosition).x;
			m_camLookDirection.y = (m_arcBallOrigin - m_camPosition).y;
			m_camLookDirection.z = (m_arcBallOrigin - m_camPosition).z;
			m_camLookDirection.Normalize();

			if (m_camLookDirection.y < -0.9f)
			{
				m_camLookDirection = m_prevlookDirection;
				m_camPosition = m_prevPosition;
			}
		}
		if (_InputCommands.down)
		{
			if (m_camLookDirection.y <= 0.9f) m_camPosition -= (m_camUp * (m_movespeed * _InputCommands.camRotateY * 0.1f));	

			m_camLookDirection.x = (m_arcBallOrigin - m_camPosition).x;
			m_camLookDirection.y = (m_arcBallOrigin - m_camPosition).y;
			m_camLookDirection.z = (m_arcBallOrigin - m_camPosition).z;
			m_camLookDirection.Normalize();

			if (m_camLookDirection.y > 0.9f)
			{
				m_camLookDirection = m_prevlookDirection;
				m_camPosition = m_prevPosition;
			}
		}
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;

	//apply camera vectors
	DirectX::SimpleMath::Matrix	m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);

	//Save the previous camera position.
	m_prevPosition = m_camPosition;
	m_prevlookDirection = m_camLookDirection;

	return m_view;
}
