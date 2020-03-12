#include "ToolMain.h"
#include "resource.h"
#include <vector>
#include <iostream>
#include <sstream>

using namespace DirectX::SimpleMath;

//
//ToolMain Class
ToolMain::ToolMain()
{

	m_currentChunk = 0;		//default value
	m_selectedObject = 0;	//initial selection ID
	//m_sceneGraph.clear();	//clear the vector for the scenegraph
	m_gameGraph.clear();
	m_databaseConnection = NULL;

	//zero input commands
	m_toolInputCommands.forward = false;
	m_toolInputCommands.back = false;
	m_toolInputCommands.left = false;
	m_toolInputCommands.right = false;
	m_toolInputCommands.up = false;
	m_toolInputCommands.down = false;
	m_toolInputCommands.rotRight = false;
	m_toolInputCommands.rotLeft = false;
	m_toolInputCommands.rotUp = false;
	m_toolInputCommands.rotDown = false;
	m_toolInputCommands.mouseControls = false;

	//collision used for axis alligned arrows when grabbing objects.
	m_axisBoxX.Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };
	m_axisBoxY.Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };
	m_axisBoxZ.Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };

}

ToolMain::~ToolMain()
{
	sqlite3_close(m_databaseConnection);		//close the database connection
}


int ToolMain::getCurrentSelectionID()
{
	return m_selectedObject;
}

void ToolMain::onActionInitialise(HWND handle, int width, int height)
{
	//window size, handle etc for directX
	m_width		= width;
	m_height	= height;
	
	m_d3dRenderer.Initialize(handle, m_width, m_height);
	m_toolHandle = handle;

	//database connection establish
	int rc;
	rc = sqlite3_open_v2("database/test.db",&m_databaseConnection, SQLITE_OPEN_READWRITE, NULL);

	if (rc) 
	{
		TRACE("Can't open database");
		//if the database cant open. Perhaps a more catastrophic error would be better here
	}
	else 
	{
		TRACE("Opened database successfully");
	}


	mouseGrabbing = false;
	m_axisBoxX.Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };
	m_axisBoxY.Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };
	m_axisBoxZ.Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };


	onActionLoad();
}

void ToolMain::onActionLoad()
{
	//load current chunk and objects into lists
	//if (!m_sceneGraph.empty())		//is the vector empty
	//{
	//	m_sceneGraph.clear();		//if not, empty it
	//}

	if (m_gameGraph.empty())
	{
		m_gameGraph.clear();
	}

	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	sqlite3_stmt *pResultsChunk;

	//OBJECTS IN THE WORLD
	//prepare SQL Text
	sqlCommand = "SELECT * from Objects";				//sql command which will return all records from the objects table.
	//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0 );

	//loop for each row in results until there are no more rows.  ie for every row in the results. We create and object
	while (sqlite3_step(pResults) == SQLITE_ROW)
	{	
		GameObject newSceneObject;
		newSceneObject.ID = sqlite3_column_int(pResults, 0);
		newSceneObject.chunk_ID = sqlite3_column_int(pResults, 1);
		newSceneObject.model_path		= reinterpret_cast<const char*>(sqlite3_column_text(pResults, 2));
		newSceneObject.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 3));
		newSceneObject.posX = sqlite3_column_double(pResults, 4);
		newSceneObject.posY = sqlite3_column_double(pResults, 5);
		newSceneObject.posZ = sqlite3_column_double(pResults, 6);
		newSceneObject.rotX = sqlite3_column_double(pResults, 7);
		newSceneObject.rotY = sqlite3_column_double(pResults, 8);
		newSceneObject.rotZ = sqlite3_column_double(pResults, 9);
		newSceneObject.scaX = sqlite3_column_double(pResults, 10);
		newSceneObject.scaY = sqlite3_column_double(pResults, 11);
		newSceneObject.scaZ = sqlite3_column_double(pResults, 12);
		newSceneObject.render = sqlite3_column_int(pResults, 13);
		newSceneObject.collision = sqlite3_column_int(pResults, 14);
		newSceneObject.collision_mesh = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 15));
		newSceneObject.collectable = sqlite3_column_int(pResults, 16);
		newSceneObject.destructable = sqlite3_column_int(pResults, 17);
		newSceneObject.health_amount = sqlite3_column_int(pResults, 18);
		newSceneObject.editor_render = sqlite3_column_int(pResults, 19);
		newSceneObject.editor_texture_vis = sqlite3_column_int(pResults, 20);
		newSceneObject.editor_normals_vis = sqlite3_column_int(pResults, 21);
		newSceneObject.editor_collision_vis = sqlite3_column_int(pResults, 22);
		newSceneObject.editor_pivot_vis = sqlite3_column_int(pResults, 23);
		newSceneObject.pivotX = sqlite3_column_double(pResults, 24);
		newSceneObject.pivotY = sqlite3_column_double(pResults, 25);
		newSceneObject.pivotZ = sqlite3_column_double(pResults, 26);
		newSceneObject.snapToGround = sqlite3_column_int(pResults, 27);
		newSceneObject.AINode = sqlite3_column_int(pResults, 28);
		newSceneObject.audio_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 29));
		newSceneObject.volume = sqlite3_column_double(pResults, 30);
		newSceneObject.pitch = sqlite3_column_double(pResults, 31);
		newSceneObject.pan = sqlite3_column_int(pResults, 32);
		newSceneObject.one_shot = sqlite3_column_int(pResults, 33);
		newSceneObject.play_on_init = sqlite3_column_int(pResults, 34);
		newSceneObject.play_in_editor = sqlite3_column_int(pResults, 35);
		newSceneObject.min_dist = sqlite3_column_double(pResults, 36);
		newSceneObject.max_dist = sqlite3_column_double(pResults, 37);
		newSceneObject.camera = sqlite3_column_int(pResults, 38);
		newSceneObject.path_node = sqlite3_column_int(pResults, 39);
		newSceneObject.path_node_start = sqlite3_column_int(pResults, 40);
		newSceneObject.path_node_end = sqlite3_column_int(pResults, 41);
		newSceneObject.parent_id = sqlite3_column_int(pResults, 42);
		newSceneObject.editor_wireframe = sqlite3_column_int(pResults, 43);
		newSceneObject.name = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 44));

		newSceneObject.light_type = sqlite3_column_int(pResults, 45);
		newSceneObject.light_diffuse_r = sqlite3_column_double(pResults, 46);
		newSceneObject.light_diffuse_g = sqlite3_column_double(pResults, 47);
		newSceneObject.light_diffuse_b = sqlite3_column_double(pResults, 48);
		newSceneObject.light_specular_r = sqlite3_column_double(pResults, 49);
		newSceneObject.light_specular_g = sqlite3_column_double(pResults, 50);
		newSceneObject.light_specular_b = sqlite3_column_double(pResults, 51);
		newSceneObject.light_spot_cutoff = sqlite3_column_double(pResults, 52);
		newSceneObject.light_constant = sqlite3_column_double(pResults, 53);
		newSceneObject.light_linear = sqlite3_column_double(pResults, 54);
		newSceneObject.light_quadratic = sqlite3_column_double(pResults, 55);
	
		const DirectX::BoundingSphere bounding_sphere(DirectX::XMFLOAT3 { newSceneObject.posX, newSceneObject.posY, newSceneObject.posZ }, 1.0f);
		newSceneObject.bounding_sphere = bounding_sphere;

		//send completed object to scenegraph
		m_gameGraph.push_back(newSceneObject);
	}

	//THE WORLD CHUNK
	//prepare SQL Text
	sqlCommand = "SELECT * from Chunks";				//sql command which will return all records from  chunks table. There is only one tho.
														//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResultsChunk, 0);


	sqlite3_step(pResultsChunk);
	m_chunk.ID = sqlite3_column_int(pResultsChunk, 0);
	m_chunk.name = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 1));
	m_chunk.chunk_x_size_metres = sqlite3_column_int(pResultsChunk, 2);
	m_chunk.chunk_y_size_metres = sqlite3_column_int(pResultsChunk, 3);
	m_chunk.chunk_base_resolution = sqlite3_column_int(pResultsChunk, 4);
	m_chunk.heightmap_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 5));
	m_chunk.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 6));
	m_chunk.tex_splat_alpha_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 7));
	m_chunk.tex_splat_1_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 8));
	m_chunk.tex_splat_2_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 9));
	m_chunk.tex_splat_3_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 10));
	m_chunk.tex_splat_4_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 11));
	m_chunk.render_wireframe = sqlite3_column_int(pResultsChunk, 12);
	m_chunk.render_normals = sqlite3_column_int(pResultsChunk, 13);
	m_chunk.tex_diffuse_tiling = sqlite3_column_int(pResultsChunk, 14);
	m_chunk.tex_splat_1_tiling = sqlite3_column_int(pResultsChunk, 15);
	m_chunk.tex_splat_2_tiling = sqlite3_column_int(pResultsChunk, 16);
	m_chunk.tex_splat_3_tiling = sqlite3_column_int(pResultsChunk, 17);
	m_chunk.tex_splat_4_tiling = sqlite3_column_int(pResultsChunk, 18);


	//Process REsults into renderable
	std::vector<SceneObject> sceneGraph = GameGraphToSceneGraph(m_gameGraph);
	m_d3dRenderer.BuildDisplayList(&sceneGraph);
	//build the renderable chunk 
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);

}

void ToolMain::onActionSave()
{
	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	

	//OBJECTS IN THE WORLD Delete them all
	//prepare SQL Text
	sqlCommand = "DELETE FROM Objects";	 //will delete the whole object table.   Slightly risky but hey.
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0);
	sqlite3_step(pResults);

	//Populate with our new objects
	std::wstring sqlCommand2;
	
	//Update the scene list to match the current display list info.
	m_d3dRenderer.UpdateSceneList(&m_gameGraph);

	int numObjects = m_gameGraph.size();	//Loop thru the scengraph.

	for (int i = 0; i < numObjects; i++)
	{
		std::stringstream command;
		command << "INSERT INTO Objects " 
			<<"VALUES(" << m_gameGraph.at(i).ID << ","
			<< m_gameGraph.at(i).chunk_ID  << ","
			<< "'" << m_gameGraph.at(i).model_path <<"'" << ","
			<< "'" << m_gameGraph.at(i).tex_diffuse_path << "'" << ","
			<< m_gameGraph.at(i).posX << ","
			<< m_gameGraph.at(i).posY << ","
			<< m_gameGraph.at(i).posZ << ","
			<< m_gameGraph.at(i).rotX << ","
			<< m_gameGraph.at(i).rotY << ","
			<< m_gameGraph.at(i).rotZ << ","
			<< m_gameGraph.at(i).scaX << ","
			<< m_gameGraph.at(i).scaY << ","
			<< m_gameGraph.at(i).scaZ << ","
			<< m_gameGraph.at(i).render << ","
			<< m_gameGraph.at(i).collision << ","
			<< "'" << m_gameGraph.at(i).collision_mesh << "'" << ","
			<< m_gameGraph.at(i).collectable << ","
			<< m_gameGraph.at(i).destructable << ","
			<< m_gameGraph.at(i).health_amount << ","
			<< m_gameGraph.at(i).editor_render << ","
			<< m_gameGraph.at(i).editor_texture_vis << ","
			<< m_gameGraph.at(i).editor_normals_vis << ","
			<< m_gameGraph.at(i).editor_collision_vis << ","
			<< m_gameGraph.at(i).editor_pivot_vis << ","
			<< m_gameGraph.at(i).pivotX << ","
			<< m_gameGraph.at(i).pivotY << ","
			<< m_gameGraph.at(i).pivotZ << ","
			<< m_gameGraph.at(i).snapToGround << ","
			<< m_gameGraph.at(i).AINode << ","
			<< "'" << m_gameGraph.at(i).audio_path << "'" << ","
			<< m_gameGraph.at(i).volume << ","
			<< m_gameGraph.at(i).pitch << ","
			<< m_gameGraph.at(i).pan << ","
			<< m_gameGraph.at(i).one_shot << ","
			<< m_gameGraph.at(i).play_on_init << ","
			<< m_gameGraph.at(i).play_in_editor << ","
			<< m_gameGraph.at(i).min_dist << ","
			<< m_gameGraph.at(i).max_dist << ","
			<< m_gameGraph.at(i).camera << ","
			<< m_gameGraph.at(i).path_node << ","
			<< m_gameGraph.at(i).path_node_start << ","
			<< m_gameGraph.at(i).path_node_end << ","
			<< m_gameGraph.at(i).parent_id << ","
			<< m_gameGraph.at(i).editor_wireframe << ","
			<< "'" << m_gameGraph.at(i).name << "'" << ","

			<< m_gameGraph.at(i).light_type << ","
			<< m_gameGraph.at(i).light_diffuse_r << ","
			<< m_gameGraph.at(i).light_diffuse_g << ","
			<< m_gameGraph.at(i).light_diffuse_b << ","
			<< m_gameGraph.at(i).light_specular_r << ","
			<< m_gameGraph.at(i).light_specular_g << ","
			<< m_gameGraph.at(i).light_specular_b << ","
			<< m_gameGraph.at(i).light_spot_cutoff << ","
			<< m_gameGraph.at(i).light_constant << ","
			<< m_gameGraph.at(i).light_linear << ","
			<< m_gameGraph.at(i).light_quadratic

			<< ")";
		std::string sqlCommand2 = command.str();
		rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand2.c_str(), -1, &pResults, 0);
		sqlite3_step(pResults);	
	}
	MessageBox(NULL, L"Objects Saved", L"Notification", MB_OK);
}

void ToolMain::onActionSaveTerrain()
{
	m_d3dRenderer.SaveDisplayChunk(&m_chunk);
}

void ToolMain::Tick(MSG *msg)
{
	//do we have a selection
	
	//do we have a mode
	//are we clicking / dragging /releasing

	//has something changed
		//update Scenegraph
		//add to scenegraph
		//resend scenegraph to Direct X renderer

	//Renderer Update Call
	m_d3dRenderer.Tick(&m_toolInputCommands);	

	MouseUpdate();
}

void ToolMain::UpdateInput(MSG * msg)
{
	switch (msg->message)
	{
		//Global inputs,  mouse position and keys etc
	case WM_KEYDOWN:
		m_keyArray[msg->wParam] = true;
		break;

	case WM_KEYUP:
		m_keyArray[msg->wParam] = false;
		break;

	case WM_MOUSEMOVE:
		mouse_x = GET_X_LPARAM(msg->lParam);
		mouse_y = GET_Y_LPARAM(msg->lParam);
		break;

	case WM_LBUTTONDOWN:	//mouse button down,  you will probably need to check when its up too
		//set some flag for the mouse button in inputcommands
		MouseLDown(msg);
		break;

	case WM_LBUTTONUP:	//mouse button up,  you will probably need to check when its up too
	//set some flag for the mouse button in inputcommands
		MouseLUp(msg);
		break;

	case WM_RBUTTONDOWN:	
		MouseRDown(msg);
		break;

	case WM_RBUTTONUP:
		MouseRUp(msg);
		break;

	}

	//here we update all the actual app functionality that we want.  This information will either be used int toolmain, or sent down to the renderer (Camera movement etc
	//Mouse controls
	if (m_toolInputCommands.mouseControls)
	{
		//WASD movement
		if (m_keyArray['W'])
		{
			m_toolInputCommands.forward = true;
		}
		else m_toolInputCommands.forward = false;

		if (m_keyArray['S'])
		{
			m_toolInputCommands.back = true;
		}
		else m_toolInputCommands.back = false;

		if (m_keyArray['A'])
		{
			m_toolInputCommands.left = true;
		}
		else m_toolInputCommands.left = false;

		if (m_keyArray['D'])
		{
			m_toolInputCommands.right = true;
		}
		else m_toolInputCommands.right = false;

		if (m_keyArray['E'])
		{
			m_toolInputCommands.up = true;
		}
		else m_toolInputCommands.up = false;

		if (m_keyArray['Q'])
		{
			m_toolInputCommands.down = true;
		}
		else m_toolInputCommands.down = false;

		if (m_keyArray[16])
		{
			m_toolInputCommands.camMove = 2.0f;

		}
		else m_toolInputCommands.camMove = 1.0f;

			if (m_once)
			{
				std::vector<int> rotate{ 0, 0 };
				if (mouse_x > m_width / 2)
				{
					m_toolInputCommands.rotRight = true;
					rotate[0] = std::abs((m_width / 2) - mouse_x);
				}
				else m_toolInputCommands.rotRight = false;

				if (mouse_x < m_width / 2)
				{
					m_toolInputCommands.rotLeft = true;
					rotate[0] = std::abs((m_width / 2) - mouse_x);
				}
				else  m_toolInputCommands.rotLeft = false;

				if (mouse_y < m_height / 2)
				{
					m_toolInputCommands.rotUp = true;
					rotate[1] = std::abs((m_height / 2) - mouse_y);
				}
				else m_toolInputCommands.rotUp = false;

				if (mouse_y > m_height / 2)
				{
					m_toolInputCommands.rotDown = true;
					rotate[1] = std::abs((m_height / 2) - mouse_y);
				}
				else m_toolInputCommands.rotDown = false;

				//m_toolInputCommands.camRotateX = std::sqrt((rotate[0] * rotate[0]) + (rotate[1] * rotate[1]));

				m_toolInputCommands.camRotateX = rotate[0];
				m_toolInputCommands.camRotateY = rotate[1];
			}
			m_once = true;
	}
	else
	{
		m_toolInputCommands.rotUp = false;
		m_toolInputCommands.rotDown = false;
		m_toolInputCommands.rotLeft = false;
		m_toolInputCommands.rotRight = false;

		m_toolInputCommands.forward = false;
		m_toolInputCommands.back = false;
		m_toolInputCommands.left = false;
		m_toolInputCommands.right = false;
		m_toolInputCommands.up = false;
		m_toolInputCommands.down = false;
	}

	MouseGrabbing();
}

void ToolMain::MouseUpdate()
{
	//Set cursor position to screen centre whilst mouse controls active.
	if (m_toolInputCommands.mouseControls)
	{		
		GetWindowRect(m_toolHandle, &WindowRECT);
		SetCursorPos(WindowRECT.CenterPoint().x, WindowRECT.CenterPoint().y);
	}


	if (mouseGrabbing != true)
	{
		//Save mouse pos and direction in world space
		ScreenPosToWorldSpaceReturn mouse_pos = ScreenPosToWorldSpace(mouse_x, mouse_y);

		// create a ray using mouse pos and direction
		Ray ray;
		ray.position = mouse_pos.pos;
		ray.direction = mouse_pos.direction;

		float dist = 0;
		if (ray.Intersects(m_axisBoxX, dist))
		{
			m_d3dRenderer.xAxisColor = DirectX::XMFLOAT4{ 0.5f, 0.0f, 0.0f, 1.0f };
			m_d3dRenderer.yAxisColor = DirectX::XMFLOAT4{ 0.0f, 0.0f, 1.0f, 1.0f };
			m_d3dRenderer.zAxisColor = DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 1.0f };
		}
		else if (ray.Intersects(m_axisBoxY, dist))
		{
			m_d3dRenderer.xAxisColor = DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f };
			m_d3dRenderer.yAxisColor = DirectX::XMFLOAT4{ 0.0f, 0.0f, 0.5f, 1.0f };
			m_d3dRenderer.zAxisColor = DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 1.0f };
		}
		else if (ray.Intersects(m_axisBoxZ, dist))
		{
			m_d3dRenderer.xAxisColor = DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f };
			m_d3dRenderer.yAxisColor = DirectX::XMFLOAT4{ 0.0f, 0.0f, 1.0f, 1.0f };
			m_d3dRenderer.zAxisColor = DirectX::XMFLOAT4{ 0.0f, 0.5f, 0.0f, 1.0f };
		}
		else
		{
			m_d3dRenderer.xAxisColor = DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 0.5f };
			m_d3dRenderer.yAxisColor = DirectX::XMFLOAT4{ 0.0f, 0.0f, 1.0f, 0.5f };
			m_d3dRenderer.zAxisColor = DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 0.5f };
		}
	}
}

std::vector<SceneObject> ToolMain::GameGraphToSceneGraph(std::vector<GameObject> in_gameGraph)
{
	std::vector<SceneObject> sceneGraph;
	
	SceneObject sceneObject;
	for (int i = 0; i < in_gameGraph.size(); i++)
	{
		sceneObject.ID = in_gameGraph[i].ID;
		sceneObject.chunk_ID = in_gameGraph[i].chunk_ID;
		sceneObject.model_path = in_gameGraph[i].model_path;
		sceneObject.tex_diffuse_path = in_gameGraph[i].tex_diffuse_path;
		sceneObject.posX = in_gameGraph[i].posX;
		sceneObject.posY = in_gameGraph[i].posY;
		sceneObject.posZ = in_gameGraph[i].posZ;
		sceneObject.rotX = in_gameGraph[i].rotX;
		sceneObject.rotY = in_gameGraph[i].rotY;
		sceneObject.rotZ = in_gameGraph[i].rotZ;
		sceneObject.scaX = in_gameGraph[i].scaX;
		sceneObject.scaY = in_gameGraph[i].scaY;
		sceneObject.scaZ = in_gameGraph[i].scaZ;
		sceneObject.render = in_gameGraph[i].render;
		sceneObject.collision = in_gameGraph[i].collision;
		sceneObject.collision_mesh = in_gameGraph[i].collision_mesh;
		sceneObject.collectable = in_gameGraph[i].collectable;
		sceneObject.destructable = in_gameGraph[i].destructable;
		sceneObject.health_amount = in_gameGraph[i].health_amount;
		sceneObject.editor_render = in_gameGraph[i].editor_render;
		sceneObject.editor_texture_vis = in_gameGraph[i].editor_texture_vis;
		sceneObject.editor_normals_vis = in_gameGraph[i].editor_normals_vis;
		sceneObject.editor_collision_vis = in_gameGraph[i].editor_collision_vis;
		sceneObject.editor_pivot_vis = in_gameGraph[i].editor_pivot_vis;
		sceneObject.pivotX = in_gameGraph[i].pivotX;
		sceneObject.pivotY = in_gameGraph[i].pivotY;
		sceneObject.pivotZ = in_gameGraph[i].pivotZ;
		sceneObject.snapToGround = in_gameGraph[i].snapToGround;
		sceneObject.AINode = in_gameGraph[i].AINode;
		sceneObject.audio_path = in_gameGraph[i].audio_path;
		sceneObject.volume = in_gameGraph[i].volume;
		sceneObject.pitch = in_gameGraph[i].pitch;
		sceneObject.pan = in_gameGraph[i].pan;
		sceneObject.one_shot = in_gameGraph[i].one_shot;
		sceneObject.play_on_init = in_gameGraph[i].play_on_init;
		sceneObject.play_in_editor = in_gameGraph[i].play_in_editor;
		sceneObject.min_dist = in_gameGraph[i].min_dist;
		sceneObject.max_dist = in_gameGraph[i].max_dist;
		sceneObject.camera = in_gameGraph[i].camera;
		sceneObject.path_node = in_gameGraph[i].path_node;
		sceneObject.path_node_start = in_gameGraph[i].path_node_start;
		sceneObject.path_node_end = in_gameGraph[i].path_node_end;
		sceneObject.parent_id = in_gameGraph[i].parent_id;
		sceneObject.editor_wireframe = in_gameGraph[i].editor_wireframe;
		sceneObject.name = in_gameGraph[i].name;

		sceneObject.light_type = in_gameGraph[i].light_type;
		sceneObject.light_diffuse_r = in_gameGraph[i].light_diffuse_r;
		sceneObject.light_diffuse_g = in_gameGraph[i].light_diffuse_g;
		sceneObject.light_diffuse_b = in_gameGraph[i].light_diffuse_g;
		sceneObject.light_specular_r = in_gameGraph[i].light_specular_r;
		sceneObject.light_specular_g = in_gameGraph[i].light_specular_g;
		sceneObject.light_specular_b = in_gameGraph[i].light_specular_b;
		sceneObject.light_spot_cutoff = in_gameGraph[i].light_spot_cutoff;
		sceneObject.light_constant = in_gameGraph[i].light_constant;
		sceneObject.light_linear = in_gameGraph[i].light_linear;
		sceneObject.light_quadratic = in_gameGraph[i].light_quadratic;

		sceneGraph.push_back(sceneObject);
	}

	return sceneGraph;
}

ScreenPosToWorldSpaceReturn ToolMain::ScreenPosToWorldSpace(float x, float y)
{
	ScreenPosToWorldSpaceReturn return_struct;

	// create 2 positions in screenspace using the cursor position
	Vector3 nearSource = Vector3(x, y, 0.0f);
	Vector3 farSource = Vector3(x, y, 1.0f);

	// find the two screen space positions in world space
	Vector3 nearPoint = DirectX::XMVector3Unproject(nearSource, m_d3dRenderer.GetActiveCameraLocation().x, m_d3dRenderer.GetActiveCameraLocation().y,
		m_width, m_height, 0.0f, 1.0f,
		m_d3dRenderer.GetProjMatrix(), m_d3dRenderer.GetViewMatrix(), m_d3dRenderer.GetWorldMatrix());

	Vector3 farPoint = DirectX::XMVector3Unproject(farSource, m_d3dRenderer.GetActiveCameraLocation().x, m_d3dRenderer.GetActiveCameraLocation().y,
		m_width, m_height, 0.0f, 1.0f,
		m_d3dRenderer.GetProjMatrix(), m_d3dRenderer.GetViewMatrix(), m_d3dRenderer.GetWorldMatrix());

	Vector3 direction = farPoint - nearPoint;
	direction.Normalize();

	return_struct.pos = nearPoint;
	return_struct.direction = direction;

	return return_struct;
}

void ToolMain::MouseGrabbing()
{
	//Mouse Grabbing objects
	if (mouseGrabbing)
	{
		switch (grabbedAxis)
		{
		case GrabbedAxis::x:

			switch (m_manipType)
			{
			case ManipulationType::Position:
				if (mouse_x < mouseGrabbedCoords[0] - 1)
				{
					m_toolInputCommands.moveObjRight = true;
					m_toolInputCommands.objMove = std::abs(mouseGrabbedCoords[0] - mouse_x);
				}
				else m_toolInputCommands.moveObjRight = false;

				if (mouse_x > mouseGrabbedCoords[0] + 1)
				{
					m_toolInputCommands.moveObjLeft = true;
					m_toolInputCommands.objMove = std::abs(mouseGrabbedCoords[0] - mouse_x);
				}
				else m_toolInputCommands.moveObjLeft = false;
				break;

			case ManipulationType::Rotation:
				break;

			case ManipulationType::Scale:
				break;

			}

			break;

		case GrabbedAxis::y:

			switch (m_manipType)
			{
			case ManipulationType::Position:
				if (mouse_y < mouseGrabbedCoords[1] - 1)
				{
					m_toolInputCommands.moveObjUp = true;
					m_toolInputCommands.objMove = std::abs(mouseGrabbedCoords[1] - mouse_y);
				}
				else m_toolInputCommands.moveObjUp = false;

				if (mouse_y > mouseGrabbedCoords[1] + 1)
				{
					m_toolInputCommands.moveObjDown = true;
					m_toolInputCommands.objMove = std::abs(mouseGrabbedCoords[1] - mouse_y);
				}
				else m_toolInputCommands.moveObjDown = false;
				break;
			}
			break;

		case GrabbedAxis::z:

			switch (m_manipType)
			{
			case ManipulationType::Position:
				if (mouse_x < mouseGrabbedCoords[0] - 1)
				{
					m_toolInputCommands.moveObjForward = true;
					m_toolInputCommands.objMove = std::abs(mouseGrabbedCoords[0] - mouse_x);
				}
				else m_toolInputCommands.moveObjForward = false;

				if (mouse_x > mouseGrabbedCoords[0] + 1)
				{
					m_toolInputCommands.moveObjBack = true;
					m_toolInputCommands.objMove = std::abs(mouseGrabbedCoords[0] - mouse_x);

				}
				else m_toolInputCommands.moveObjBack = false;
				break;
			}
			break;
		}

		mouseGrabbedCoords[0] = mouse_x;
		mouseGrabbedCoords[1] = mouse_y;

		//update axis arrows and collsion box position.
		DirectX::SimpleMath::Vector3 pos = m_d3dRenderer.GetDisplayObjPos(m_selectedObject);
		m_d3dRenderer.x_arrow = DirectX::SimpleMath::Vector3{ pos.x, pos.y, pos.z };
		m_d3dRenderer.y_arrow = DirectX::SimpleMath::Vector3{ pos.x, pos.y, pos.z };
		m_d3dRenderer.z_arrow = DirectX::SimpleMath::Vector3{ pos.x, pos.y, pos.z };

		m_axisBoxX.Center = pos + DirectX::SimpleMath::Vector3{ 1.0f, 0.0f, 0.0f };
		m_axisBoxY.Center = pos + DirectX::SimpleMath::Vector3{ 0.0f, 1.0f, 0.0f };
		m_axisBoxZ.Center = pos + DirectX::SimpleMath::Vector3{ 0.0f, 0.0f, 1.0f };

		std::vector<Quad> quads1 = m_d3dRenderer.BoxToQuads(m_axisBoxX.Center, m_axisBoxX.Extents);
		std::vector<Quad> quads2 = m_d3dRenderer.BoxToQuads(m_axisBoxY.Center, m_axisBoxY.Extents);
		std::vector<Quad> quads3 = m_d3dRenderer.BoxToQuads(m_axisBoxZ.Center, m_axisBoxZ.Extents);

		m_d3dRenderer.m_axisBoxList.clear();
		for (int j = 0; j < quads1.size(); j++)
		{
			m_d3dRenderer.m_axisBoxList.push_back(quads1[j]);
		}
		for (int j = 0; j < quads2.size(); j++)
		{
			m_d3dRenderer.m_axisBoxList.push_back(quads2[j]);
		}
		for (int j = 0; j < quads3.size(); j++)
		{
			m_d3dRenderer.m_axisBoxList.push_back(quads3[j]);
		}
	}
	else
	{
		m_toolInputCommands.moveObjRight = false;
		m_toolInputCommands.moveObjLeft = false;
		m_toolInputCommands.moveObjUp = false;
		m_toolInputCommands.moveObjDown = false;
		m_toolInputCommands.moveObjForward = false;
		m_toolInputCommands.moveObjBack = false;
	}
}

bool ToolMain::MouseCollision()
{
	//Save mouse pos and direction in world space
	ScreenPosToWorldSpaceReturn mouse_pos = ScreenPosToWorldSpace(mouse_x, mouse_y);

	// create a ray using mouse pos and direction
	Ray ray;
	ray.position = mouse_pos.pos;
	ray.direction = mouse_pos.direction;
	
	//has the mouse clicked an obj
	bool collision = MouseClickedObj(ray);

	return collision;
}

bool ToolMain::MouseClickedObj(DirectX::SimpleMath::Ray ray)
{
	float dist = 0;

	if (ray.Intersects(m_axisBoxX, dist))
	{
		grabbedAxis = GrabbedAxis::x;

		mouseGrabbing = true;
		mouseGrabbedCoords[0] = mouse_x;
		mouseGrabbedCoords[1] = mouse_y;

		return true;
	}
	else if (ray.Intersects(m_axisBoxY, dist))
	{
		grabbedAxis = GrabbedAxis::y;

		mouseGrabbing = true;
		mouseGrabbedCoords[0] = mouse_x;
		mouseGrabbedCoords[1] = mouse_y;

		return true;
	}
	else if (ray.Intersects(m_axisBoxZ, dist))
	{
		grabbedAxis = GrabbedAxis::z;

		mouseGrabbing = true;
		mouseGrabbedCoords[0] = mouse_x;
		mouseGrabbedCoords[1] = mouse_y;

		return true;
	}

	for (int i = 0; i < m_gameGraph.size(); i++)
	{
		if (ray.Intersects(m_gameGraph[i].bounding_sphere, dist))
		{
			//Set the selected object to be the object collision occurs with
			m_selectedObject = i;
			m_d3dRenderer.SetSelectedObj(i);

			//Axis arrows bounding box.
			m_axisBoxX.Center = DirectX::XMFLOAT3{ m_gameGraph[i].posX + 1.0f, m_gameGraph[i].posY, m_gameGraph[i].posZ };;
			m_axisBoxY.Center = DirectX::XMFLOAT3{ m_gameGraph[i].posX, m_gameGraph[i].posY + 1.0f, m_gameGraph[i].posZ };;
			m_axisBoxZ.Center = DirectX::XMFLOAT3{ m_gameGraph[i].posX, m_gameGraph[i].posY, m_gameGraph[i].posZ + 1.0f };;
	
			m_axisBoxX.Extents = DirectX::XMFLOAT3{ 1.0f, 0.1f,  0.1f };
			m_axisBoxY.Extents = DirectX::XMFLOAT3{ 0.1f, 1.0f,  0.1f };
			m_axisBoxZ.Extents = DirectX::XMFLOAT3{ 0.1f, 0.1f,  1.0f };

			m_d3dRenderer.renderAxisArrows = true;
			m_d3dRenderer.x_arrow = DirectX::XMFLOAT3{ m_gameGraph[i].posX, m_gameGraph[i].posY, m_gameGraph[i].posZ };
			m_d3dRenderer.y_arrow = DirectX::XMFLOAT3{ m_gameGraph[i].posX, m_gameGraph[i].posY, m_gameGraph[i].posZ };
			m_d3dRenderer.z_arrow = DirectX::XMFLOAT3{ m_gameGraph[i].posX, m_gameGraph[i].posY, m_gameGraph[i].posZ };

			std::vector<Quad> quads1 = m_d3dRenderer.BoxToQuads(m_axisBoxX.Center, m_axisBoxX.Extents);
			std::vector<Quad> quads2 = m_d3dRenderer.BoxToQuads(m_axisBoxY.Center, m_axisBoxY.Extents);
			std::vector<Quad> quads3 = m_d3dRenderer.BoxToQuads(m_axisBoxZ.Center, m_axisBoxZ.Extents);

			m_d3dRenderer.m_axisBoxList.clear();
			for (int j = 0; j < quads1.size(); j++)
			{
				m_d3dRenderer.m_axisBoxList.push_back(quads1[j]);
			}
			for (int j = 0; j < quads2.size(); j++)
			{
				m_d3dRenderer.m_axisBoxList.push_back(quads2[j]);
			}
			for (int j = 0; j < quads3.size(); j++)
			{
				m_d3dRenderer.m_axisBoxList.push_back(quads3[j]);
			}
			
			return true;
		}
	}

	m_d3dRenderer.renderAxisArrows = false;
	m_axisBoxX.Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };
	m_axisBoxY.Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };
	m_axisBoxZ.Extents = DirectX::XMFLOAT3{ 0.0f, 0.0f,  0.0f };

	return false;
}

void ToolMain::MouseLDown(MSG* msg)
{
	MouseCollision();
}	

void ToolMain::MouseLUp(MSG* msg)
{
	if (mouseGrabbing)
	{
		//Update the scene list to match the current display list info.
		m_d3dRenderer.UpdateSceneList(&m_gameGraph);
		mouseGrabbing = false;
	}
}

void ToolMain::MouseRDown(MSG* msg)
{
	m_toolInputCommands.mouseControls = true;
	m_once = false;
}

void ToolMain::MouseRUp(MSG* msg)
{
	m_toolInputCommands.mouseControls = false;
}
