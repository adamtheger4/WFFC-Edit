#include "MouseTool.h"

MouseTool::MouseTool(Game* in_game, TerrainTool* in_terrainTool)
{
	m_d3dRenderer = in_game;
	m_terrainTool = in_terrainTool;
}

MouseTool::~MouseTool()
{
}

void MouseTool::Update(MSG * msg, CRect WindowRECT)
{
	if (m_windowOpen == false)
	{
		//Save mouse pos and direction in world space
		ScreenPosToWorldSpaceReturn mouse_pos = ScreenPosToWorldSpace();

		// create a ray using mouse pos and direction
		Ray ray;
		ray.position = mouse_pos.pos;
		ray.direction = mouse_pos.direction;

		//Set cursor position to screen centre whilst mouse controls active.
		if (m_toolInputCommands->mouseControls)
		{
			SetCursorPos(WindowRECT.CenterPoint().x, WindowRECT.CenterPoint().y);
		}

		if (m_grabbing != true)
		{
			//If mouse is overlapping an axis grab arrow then highlight the arrow.
			float dist = 0;
			if (m_axisBoxX->Intersects(ray.position, ray.direction, dist))
			{
				m_d3dRenderer->xAxisColor = DirectX::XMFLOAT4{ 0.5f, 0.0f, 0.0f, 1.0f };
				m_d3dRenderer->yAxisColor = DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 1.0f };
				m_d3dRenderer->zAxisColor = DirectX::XMFLOAT4{ 0.0f, 0.0f, 1.0f, 1.0f };
			}
			else if (m_axisBoxY->Intersects(ray.position, ray.direction, dist))
			{
				m_d3dRenderer->xAxisColor = DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f };
				m_d3dRenderer->yAxisColor = DirectX::XMFLOAT4{ 0.0f, 0.5f, 0.0f, 1.0f };
				m_d3dRenderer->zAxisColor = DirectX::XMFLOAT4{ 0.0f, 0.0f, 1.0f, 1.0f };
			}
			else if (m_axisBoxZ->Intersects(ray.position, ray.direction, dist))
			{
				m_d3dRenderer->xAxisColor = DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f };
				m_d3dRenderer->yAxisColor = DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 1.0f };
				m_d3dRenderer->zAxisColor = DirectX::XMFLOAT4{ 0.0f, 0.0f, 0.5f, 1.0f };
			}
			else
			{
				m_d3dRenderer->xAxisColor = DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 0.5f };
				m_d3dRenderer->yAxisColor = DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 0.5f };
				m_d3dRenderer->zAxisColor = DirectX::XMFLOAT4{ 0.0f, 0.0f, 1.0f, 0.5f };
			}
		}

		if (m_terrainTool->Active == true)
		{
			//Clicking Terrain
			if (x != m_prevX || y != m_prevY)	m_terrainTool->mouseTerrainManipReturn = m_d3dRenderer->RayToDisplayChunkCollision(ray);

			if (m_terrainTool->mouseTerrainManipReturn.did_hit)
			{
				std::vector<Quad> quads1 = m_d3dRenderer->BoxToQuads(m_terrainTool->mouseTerrainManipReturn.hit_location, DirectX::XMFLOAT3{ 0.1f, 0.1f, 0.1f });
				m_d3dRenderer->m_terrainToolCursor = quads1;

				if (m_terrainTool->updateTargetHeight)
				{
					m_terrainTool->targetHeight = m_terrainTool->mouseTerrainManipReturn.hit_location.y;
					m_terrainTool->updateTargetHeight = false;
				}

				if (m_terrainTool->GetSculptType() == TerrainSculptType::Flatten)
				{
					m_d3dRenderer->FlattenTerrain(m_terrainTool->mouseTerrainManipReturn.row, m_terrainTool->mouseTerrainManipReturn.column, DirectX::XMFLOAT3{ m_terrainTool->GetManipulationOffset().x, m_terrainTool->GetManipulationOffset().y, m_terrainTool->GetManipulationOffset().z }, m_terrainTool->targetHeight, m_terrainTool->smoothSculpt);
				}
				else if (m_terrainTool->GetSculptType() == TerrainSculptType::Dig)
				{
					m_d3dRenderer->SculptTerrain(m_terrainTool->mouseTerrainManipReturn.row, m_terrainTool->mouseTerrainManipReturn.column, DirectX::XMFLOAT3{ -m_terrainTool->GetManipulationOffset().x, -m_terrainTool->GetManipulationOffset().y, -m_terrainTool->GetManipulationOffset().z }, m_terrainTool->smoothSculpt, m_terrainTool->m_terrainSculptMode);
				}
				else
				{
					m_d3dRenderer->SculptTerrain(m_terrainTool->mouseTerrainManipReturn.row, m_terrainTool->mouseTerrainManipReturn.column, m_terrainTool->GetManipulationOffset(), m_terrainTool->smoothSculpt, m_terrainTool->m_terrainSculptMode);
				}
			}
		}
		else
		{
			std::vector<Quad> quads1 = m_d3dRenderer->BoxToQuads(m_terrainTool->mouseTerrainManipReturn.hit_location, DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f });
			m_d3dRenderer->m_terrainToolCursor = quads1;
		}

	}

	m_prevX = x;
	m_prevY = y;
}

void MouseTool::MouseGrabbing()
{
	//Mouse Grabbing objects
	if (m_grabbing)
	{
		float min_input = 1.0f;

		switch (m_grabbedAxis)
		{
		case GrabbedAxis::x:
			switch (m_manipType)
			{

			case ManipulationType::Position:

				if (m_d3dRenderer->GetActiveCameraLocation().z >= m_gameGraph[m_selectedObject].posZ) //Invert input based on which side the camera is on from the object->
				{
					if (x - m_grabbedCoords[0] > min_input)
						m_d3dRenderer->SetSelectedObjectPosition(m_selectedObject, m_d3dRenderer->GetDisplayObjPos(m_selectedObject) + Vector3{ m_positionSnap , 0.0f, 0.0f });
					else if (x - m_grabbedCoords[0] < -min_input)
						m_d3dRenderer->SetSelectedObjectPosition(m_selectedObject, m_d3dRenderer->GetDisplayObjPos(m_selectedObject) - Vector3{ m_positionSnap , 0.0f, 0.0f });
				}
				else
				{
					if (x - m_grabbedCoords[0] < -min_input)
						m_d3dRenderer->SetSelectedObjectPosition(m_selectedObject, m_d3dRenderer->GetDisplayObjPos(m_selectedObject) + Vector3{ m_positionSnap , 0.0f, 0.0f });
					else if (x - m_grabbedCoords[0] > min_input)
						m_d3dRenderer->SetSelectedObjectPosition(m_selectedObject, m_d3dRenderer->GetDisplayObjPos(m_selectedObject) - Vector3{ m_positionSnap , 0.0f, 0.0f });
				}

				break;

			case ManipulationType::Rotation:
				if (m_d3dRenderer->GetActiveCameraLocation().z >= m_gameGraph[m_selectedObject].posZ) //Invert input based on which side the camera is on from the object.
				{
					if (x - m_grabbedCoords[0] > min_input)
					{
						m_d3dRenderer->SetSelectedObjectRotation(m_selectedObject, m_d3dRenderer->GetDisplayObjRotation(m_selectedObject) + Vector3{ m_rotationSnap , 0.0f, 0.0f });
					}
					else if (x - m_grabbedCoords[0] < -min_input)
						m_d3dRenderer->SetSelectedObjectRotation(m_selectedObject, m_d3dRenderer->GetDisplayObjRotation(m_selectedObject) - Vector3{ m_rotationSnap , 0.0f, 0.0f });
				}
				else
				{
					if (x - m_grabbedCoords[0] < -min_input)
					{
						m_d3dRenderer->SetSelectedObjectRotation(m_selectedObject, m_d3dRenderer->GetDisplayObjRotation(m_selectedObject) + Vector3{ m_rotationSnap , 0.0f, 0.0f });

					}
					else if (x - m_grabbedCoords[0] > min_input)
						m_d3dRenderer->SetSelectedObjectRotation(m_selectedObject, m_d3dRenderer->GetDisplayObjRotation(m_selectedObject) - Vector3{ m_rotationSnap , 0.0f, 0.0f });
				}
				break;

			case ManipulationType::Scale:
				if (m_d3dRenderer->GetActiveCameraLocation().z >= m_gameGraph[m_selectedObject].posZ) //Invert input based on which side the camera is on from the object.
				{
					if (x - m_grabbedCoords[0] > min_input)
						m_d3dRenderer->SetSelectedObjectScale(m_selectedObject, m_d3dRenderer->GetDisplayObjScale(m_selectedObject) + Vector3{ m_scaleSnap , 0.0f, 0.0f });
					else if (x - m_grabbedCoords[0] < -min_input)
						m_d3dRenderer->SetSelectedObjectScale(m_selectedObject, m_d3dRenderer->GetDisplayObjScale(m_selectedObject) - Vector3{ m_scaleSnap , 0.0f, 0.0f });
				}
				else
				{
					if (x - m_grabbedCoords[0] < -min_input)
						m_d3dRenderer->SetSelectedObjectScale(m_selectedObject, m_d3dRenderer->GetDisplayObjScale(m_selectedObject) + Vector3{ m_scaleSnap , 0.0f, 0.0f });
					else if (x - m_grabbedCoords[0] > min_input)
						m_d3dRenderer->SetSelectedObjectScale(m_selectedObject, m_d3dRenderer->GetDisplayObjScale(m_selectedObject) - Vector3{ m_scaleSnap , 0.0f, 0.0f });
				}
				break;

			}
			break;

		case GrabbedAxis::y:

			switch (m_manipType)
			{
			case ManipulationType::Position:
				if (y - m_grabbedCoords[1] < -min_input)
					m_d3dRenderer->SetSelectedObjectPosition(m_selectedObject, m_d3dRenderer->GetDisplayObjPos(m_selectedObject) + Vector3{ 0.0f, m_positionSnap, 0.0f });
				else if (y - m_grabbedCoords[1] > min_input)
					m_d3dRenderer->SetSelectedObjectPosition(m_selectedObject, m_d3dRenderer->GetDisplayObjPos(m_selectedObject) - Vector3{ 0.0f, m_positionSnap, 0.0f });
				break;

			case ManipulationType::Rotation:
				if (y - m_grabbedCoords[1] > min_input)
					m_d3dRenderer->SetSelectedObjectRotation(m_selectedObject, m_d3dRenderer->GetDisplayObjRotation(m_selectedObject) + Vector3{ 0.0f, m_rotationSnap, 0.0f });
				else if (y - m_grabbedCoords[1] < -min_input)
					m_d3dRenderer->SetSelectedObjectRotation(m_selectedObject, m_d3dRenderer->GetDisplayObjRotation(m_selectedObject) - Vector3{ 0.0f, m_rotationSnap, 0.0f });
				break;

			case ManipulationType::Scale:
				if (y - m_grabbedCoords[1] < -min_input)
					m_d3dRenderer->SetSelectedObjectScale(m_selectedObject, m_d3dRenderer->GetDisplayObjScale(m_selectedObject) + Vector3{ 0.0f, m_scaleSnap, 0.0f });
				else if (y - m_grabbedCoords[1] > min_input)
					m_d3dRenderer->SetSelectedObjectScale(m_selectedObject, m_d3dRenderer->GetDisplayObjScale(m_selectedObject) - Vector3{ 0.0f, m_scaleSnap, 0.0f });
				break;
			}
			break;

		case GrabbedAxis::z:

			switch (m_manipType)
			{
			case ManipulationType::Position:
				if (m_d3dRenderer->GetActiveCameraLocation().x <= m_gameGraph[m_selectedObject].posX) //Invert input based on which side the camera is on from the object.
				{
					if (x - m_grabbedCoords[0] > min_input)
						m_d3dRenderer->SetSelectedObjectPosition(m_selectedObject, m_d3dRenderer->GetDisplayObjPos(m_selectedObject) + Vector3{ 0.0f, 0.0f, m_positionSnap });
					else if (x - m_grabbedCoords[0] < -min_input)
						m_d3dRenderer->SetSelectedObjectPosition(m_selectedObject, m_d3dRenderer->GetDisplayObjPos(m_selectedObject) - Vector3{ 0.0f, 0.0f, m_positionSnap });
				}
				else
				{
					if (x - m_grabbedCoords[0] < -min_input)
						m_d3dRenderer->SetSelectedObjectPosition(m_selectedObject, m_d3dRenderer->GetDisplayObjPos(m_selectedObject) + Vector3{ 0.0f, 0.0f, m_positionSnap });
					else if (x - m_grabbedCoords[0] > min_input)
						m_d3dRenderer->SetSelectedObjectPosition(m_selectedObject, m_d3dRenderer->GetDisplayObjPos(m_selectedObject) - Vector3{ 0.0f, 0.0f, m_positionSnap });
				}
				break;

			case ManipulationType::Rotation:
				if (m_d3dRenderer->GetActiveCameraLocation().x >= m_gameGraph[m_selectedObject].posX) //Invert input based on which side the camera is on from the object.
				{
					if (x - m_grabbedCoords[0] > min_input)
						m_d3dRenderer->SetSelectedObjectRotation(m_selectedObject, m_d3dRenderer->GetDisplayObjRotation(m_selectedObject) + Vector3{ 0.0f, 0.0f, m_rotationSnap });
					else if (x - m_grabbedCoords[0] < -min_input)
						m_d3dRenderer->SetSelectedObjectRotation(m_selectedObject, m_d3dRenderer->GetDisplayObjRotation(m_selectedObject) - Vector3{ 0.0f, 0.0f, m_rotationSnap });
				}
				else
				{
					if (x - m_grabbedCoords[0] < -min_input)
						m_d3dRenderer->SetSelectedObjectRotation(m_selectedObject, m_d3dRenderer->GetDisplayObjRotation(m_selectedObject) + Vector3{ 0.0f, 0.0f, m_rotationSnap });
					else if (x - m_grabbedCoords[0] > min_input)
						m_d3dRenderer->SetSelectedObjectRotation(m_selectedObject, m_d3dRenderer->GetDisplayObjRotation(m_selectedObject) - Vector3{ 0.0f, 0.0f, m_rotationSnap });
				}
				break;

			case ManipulationType::Scale:
				if (m_d3dRenderer->GetActiveCameraLocation().x >= m_gameGraph[m_selectedObject].posX) //Invert input based on which side the camera is on from the object.
				{
					if (x - m_grabbedCoords[0] > min_input)
						m_d3dRenderer->SetSelectedObjectScale(m_selectedObject, m_d3dRenderer->GetDisplayObjScale(m_selectedObject) + Vector3{ 0.0f, 0.0f, m_scaleSnap });
					else if (x - m_grabbedCoords[0] < -min_input)
						m_d3dRenderer->SetSelectedObjectScale(m_selectedObject, m_d3dRenderer->GetDisplayObjScale(m_selectedObject) - Vector3{ 0.0f, 0.0f, m_scaleSnap });
				}
				else
				{
					if (x - m_grabbedCoords[0] < -min_input)
						m_d3dRenderer->SetSelectedObjectScale(m_selectedObject, m_d3dRenderer->GetDisplayObjScale(m_selectedObject) + Vector3{ 0.0f, 0.0f, m_scaleSnap });
					else if (x - m_grabbedCoords[0] > min_input)
						m_d3dRenderer->SetSelectedObjectScale(m_selectedObject, m_d3dRenderer->GetDisplayObjScale(m_selectedObject) - Vector3{ 0.0f, 0.0f, m_scaleSnap });
				}
				break;

			}
			break;
		}

		m_grabbedCoords[0] = x;
		m_grabbedCoords[1] = y;

		//update axis arrows and collsion box position.
		DirectX::SimpleMath::Vector3 pos = m_d3dRenderer->GetDisplayObjPos(m_selectedObject);
		m_axisBoxX->Center = DirectX::SimpleMath::Vector3{ pos.x + 1.1f, pos.y, pos.z };
		m_axisBoxY->Center = DirectX::SimpleMath::Vector3{ pos.x, pos.y + 1.1f, pos.z };
		m_axisBoxZ->Center = DirectX::SimpleMath::Vector3{ pos.x, pos.y, pos.z + 1.1f };
		m_axisBoxX->Extents = DirectX::XMFLOAT3{ 1.0f, 0.1f,  0.1f };
		m_axisBoxY->Extents = DirectX::XMFLOAT3{ 0.1f, 1.0f,  0.1f };
		m_axisBoxZ->Extents = DirectX::XMFLOAT3{ 0.1f, 0.1f,  1.0f }; 

		m_d3dRenderer->x_arrow = m_axisBoxX->Center;
		m_d3dRenderer->y_arrow = m_axisBoxY->Center;
		m_d3dRenderer->z_arrow = m_axisBoxZ->Center;

		std::vector<Quad> quads1 = m_d3dRenderer->BoxToQuads(m_axisBoxX->Center, m_axisBoxX->Extents);
		std::vector<Quad> quads2 = m_d3dRenderer->BoxToQuads(m_axisBoxY->Center, m_axisBoxY->Extents);
		std::vector<Quad> quads3 = m_d3dRenderer->BoxToQuads(m_axisBoxZ->Center, m_axisBoxZ->Extents);

		m_d3dRenderer->m_axisBoxList.clear();
		for (int j = 0; j < quads1.size(); j++)
		{
			m_d3dRenderer->m_axisBoxList.push_back(quads1[j]);
		}
		for (int j = 0; j < quads2.size(); j++)
		{
			m_d3dRenderer->m_axisBoxList.push_back(quads2[j]);
		}
		for (int j = 0; j < quads3.size(); j++)
		{
			m_d3dRenderer->m_axisBoxList.push_back(quads3[j]);
		}
	}
}

bool MouseTool::MouseCollision()
{
	//Save mouse pos and direction in world space
	ScreenPosToWorldSpaceReturn mouse_pos = ScreenPosToWorldSpace();

	// create a ray using mouse pos and direction
	Ray ray;
	ray.position = mouse_pos.pos;
	ray.direction = mouse_pos.direction;

	//has the mouse clicked an obj
	bool collision = MouseClickedObj(ray);

	return collision;
}

bool MouseTool::MouseClickedObj(DirectX::SimpleMath::Ray ray)
{
	float dist = 0;

	//If clicking an axis box. (Used for position, rotation and scaling)
	if (m_axisBoxX->Intersects(ray.position, ray.direction, dist))
	{
		m_grabbedAxis = GrabbedAxis::x;

		m_grabbing = true;
		m_grabbedCoords[0] = x;
		m_grabbedCoords[1] = y;

		return true;
	}
	else if (m_axisBoxY->Intersects(ray.position, ray.direction, dist))
	{
		m_grabbedAxis = GrabbedAxis::y;

		m_grabbing = true;
		m_grabbedCoords[0] = x;
		m_grabbedCoords[1] = y;

		return true;
	}
	else if (m_axisBoxZ->Intersects(ray.position, ray.direction, dist))
	{
		m_grabbedAxis = GrabbedAxis::z;

		m_grabbing = true;
		m_grabbedCoords[0] = x;
		m_grabbedCoords[1] = y;

		return true;
	}

	//If clicking on an object.
	for (int i = 0; i < m_gameGraph.size(); i++)
	{
		if (ray.Intersects(m_gameGraph[i].bounding_sphere, dist))
		{
			//Set the selected object to be the object collision occurs with
			m_selectedObject = i;
			m_d3dRenderer->SetSelectedObj(i);

			//Axis arrows bounding box.
			m_axisBoxX->Center = DirectX::XMFLOAT3{ m_gameGraph[i].posX + 1.1f, m_gameGraph[i].posY, m_gameGraph[i].posZ };
			m_axisBoxY->Center = DirectX::XMFLOAT3{ m_gameGraph[i].posX, m_gameGraph[i].posY + 1.1f, m_gameGraph[i].posZ };
			m_axisBoxZ->Center = DirectX::XMFLOAT3{ m_gameGraph[i].posX, m_gameGraph[i].posY, m_gameGraph[i].posZ + 1.1f };

			m_axisBoxX->Extents = DirectX::XMFLOAT3{ 1.0f, 0.1f,  0.1f };
			m_axisBoxY->Extents = DirectX::XMFLOAT3{ 0.1f, 1.0f,  0.1f };
			m_axisBoxZ->Extents = DirectX::XMFLOAT3{ 0.1f, 0.1f,  1.0f };

			m_d3dRenderer->renderAxisArrows = true;
			m_d3dRenderer->x_arrow = m_axisBoxX->Center;
			m_d3dRenderer->y_arrow = m_axisBoxY->Center;
			m_d3dRenderer->z_arrow = m_axisBoxZ->Center;

			std::vector<Quad> quads1 = m_d3dRenderer->BoxToQuads(m_axisBoxX->Center, m_axisBoxX->Extents);
			std::vector<Quad> quads2 = m_d3dRenderer->BoxToQuads(m_axisBoxY->Center, m_axisBoxY->Extents);
			std::vector<Quad> quads3 = m_d3dRenderer->BoxToQuads(m_axisBoxZ->Center, m_axisBoxZ->Extents);

			m_d3dRenderer->m_axisBoxList.clear();
			for (int j = 0; j < quads1.size(); j++)
			{
				m_d3dRenderer->m_axisBoxList.push_back(quads1[j]);
			}
			for (int j = 0; j < quads2.size(); j++)
			{
				m_d3dRenderer->m_axisBoxList.push_back(quads2[j]);
			}
			for (int j = 0; j < quads3.size(); j++)
			{
				m_d3dRenderer->m_axisBoxList.push_back(quads3[j]);
			}

			m_d3dRenderer->showObjText = true;
			return true;
		}
	}

	m_d3dRenderer->showObjText = false;
	m_d3dRenderer->renderAxisArrows = false;
	m_axisBoxX->Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };
	m_axisBoxY->Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };
	m_axisBoxZ->Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };

	return false;
}

void MouseTool::MouseLDown(MSG* msg)
{
	if (m_windowOpen == false)
	{
		if (m_terrainTool->GetEnable())
		{
			m_terrainTool->Active = true;
		}
		else MouseCollision();

	}
}

void MouseTool::MouseLUp(MSG* msg)
{
	if (m_grabbing)
	{
		//Update the scene list to match the current display list info.
		//m_d3dRenderer->UpdateSceneList(m_gameGraph);
		m_grabbing = false;
	}

	if (m_terrainTool->Active) m_terrainTool->Active = false;

	m_terrainTool->updateTargetHeight = true;
}

void MouseTool::MouseRDown(MSG* msg)
{
	if (m_windowOpen == false)
	{
		m_toolInputCommands->mouseControls = true;
		m_once = false;
	}
}

void MouseTool::MouseRUp(MSG* msg)
{
	m_toolInputCommands->mouseControls = false;
}

ScreenPosToWorldSpaceReturn MouseTool::ScreenPosToWorldSpace()
{
	ScreenPosToWorldSpaceReturn return_struct;

	// create 2 positions in screenspace using the cursor position
	Vector3 nearSource = Vector3(x, y, 0.0f);
	Vector3 farSource = Vector3(x, y, 1.0f);

	// find the two screen space positions in world space
	Vector3 nearPoint = DirectX::XMVector3Unproject(nearSource, 0, 0, m_screenWidth, m_screenHeight, 0.0f, 1.0f,
		m_d3dRenderer->GetProjMatrix(), m_d3dRenderer->GetViewMatrix(), DirectX::XMMatrixIdentity());

	Vector3 farPoint = DirectX::XMVector3Unproject(farSource, 0, 0, m_screenWidth, m_screenHeight, 0.0f, 1.0f,
		m_d3dRenderer->GetProjMatrix(), m_d3dRenderer->GetViewMatrix(), DirectX::XMMatrixIdentity());

	Vector3 direction = farPoint - nearPoint;
	direction.Normalize();

	return_struct.pos = nearPoint;
	return_struct.direction = direction;

	return return_struct;
}