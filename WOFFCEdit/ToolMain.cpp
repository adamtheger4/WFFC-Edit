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
}

ToolMain::~ToolMain()
{
	sqlite3_close(m_databaseConnection);		//close the database connection
}

void ToolMain::onActionInitialise(HWND handle, int width, int height)
{
	//window size, handle etc for directX
	m_width		= width;
	m_height	= height;
	
	m_d3dRenderer.Initialize(handle, m_width, m_height);
	m_toolHandle = handle;
	GetWindowRect(m_toolHandle, &WindowRECT);

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



	UpdateTerrainText();

	onActionLoad();
}

void ToolMain::onActionLoad()
{
	//load current chunk and objects into lists
	if (m_gameGraph.empty())
	{
		m_gameGraph.clear();
	}

	m_terrainTool.LoadAllTexturePaths();

	//Init a terrain layer for each texture loaded.
	m_d3dRenderer.InitTerrainLayers(m_terrainTool.m_terrainTexturePaths.GetSize());

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

	for (int i = 0; i < m_gameGraph.size(); i++)
	{
		m_mouseTool.m_gameGraph.push_back(&m_gameGraph[i]);
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
	m_mouseTool.Update(msg, WindowRECT);

	//Renderer Update Call
	m_d3dRenderer.Tick(&m_toolInputCommands);	

	//Destroy any objects marked to be destroyed at the end of this tick.
	if(bDoDestroy)
	DestroyObjects();	
}

void ToolMain::UpdateInput(MSG * msg)
{
	float wheelDelta = 0;

	switch (msg->message)
	{
		//Global inputs,  mouse position and keys etc
	case WM_KEYDOWN:
		m_keyArray[msg->wParam] = true;
		m_terrainTool.m_Tonce = true;
		break;

	case WM_KEYUP:
		m_keyArray[msg->wParam] = false;
		break;

	case WM_MOUSEMOVE:
		m_mouseTool.x = GET_X_LPARAM(msg->lParam);
		m_mouseTool.y = GET_Y_LPARAM(msg->lParam);
		break;
		
	case WM_MOUSEWHEEL:
		wheelDelta = GET_WHEEL_DELTA_WPARAM(msg->wParam);
		break; 

	case WM_LBUTTONDOWN:  //mouse button down
		if(!windowOpen)
			m_mouseTool.LDown(msg);
		break;

	case WM_LBUTTONUP:	  //mouse button up

		m_mouseTool.LUp(msg);

		SaveLastAction();

		m_d3dRenderer.UpdateSceneList(&m_gameGraph);
		break;

	case WM_RBUTTONDOWN:	
		m_mouseTool.RDown(msg);
		break;

	case WM_RBUTTONUP:
		m_mouseTool.RUp(msg);
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

		if (wheelDelta > 0)
		{
			if(m_d3dRenderer.m_camera.m_arcBallMovement == false) m_toolInputCommands.camMovementScalar += 0.1f;
			else m_toolInputCommands.scforward = true;
		}
		else if (wheelDelta == 0)  m_toolInputCommands.scforward = false;

		if (wheelDelta < 0 && m_toolInputCommands.camMovementScalar > 0.1f)
		{
			if (m_d3dRenderer.m_camera.m_arcBallMovement == false) m_toolInputCommands.camMovementScalar -= 0.1f;
			else m_toolInputCommands.scback = true;
		}
		else if (wheelDelta == 0) m_toolInputCommands.scback = false;

		if (m_keyArray[16])
		{
			m_toolInputCommands.camMove = m_toolInputCommands.camMovementScalar * 2;

		}
		else m_toolInputCommands.camMove = m_toolInputCommands.camMovementScalar;
	}
	else // if mouse controls are not active, disable any mouse input variables.
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

	//Terrain Tool enabled controls.
	if (m_terrainTool.GetEnable())
	{
		if (m_keyArray[16]) // L SHIFT
		{
			m_terrainTool.SetSculptType(VertexSculptType::Flatten);
		}
		else if (m_keyArray[17]) // L CTRL
		{
			m_terrainTool.SetSculptType(VertexSculptType::Dig);
		}
		else  m_terrainTool.SetSculptType(VertexSculptType::Add);
	}
	else //Controls when no tools active.
	{
		if (m_keyArray[17]) // L CTRL
		{
			m_mouseTool.m_pickMultiple = true;

			if (m_keyArray['C'])
			{
				//If there is a selected item copy it
				if (m_mouseTool.m_selectedObjects.size() > 0)
				{
					std::vector<GameObject> copyObjs;

					for (int i = 0; i < m_mouseTool.m_selectedObjects.size(); i++)
					{
						copyObjs.push_back(m_gameGraph[m_mouseTool.m_selectedObjects[i]]);
					}

					CopyObjects(copyObjs);
				}
			}

			if (m_keyArray['V'])
			{
				//If there is a selected item copy it
				SaveLastAction();
				PasteObjects();
				m_keyArray['V'] = false;
			}

			//Undo action
			if (m_keyArray['Z'])
			{
				m_keyArray['Z'] = false;
				UndoAction();
				
				if (m_mouseTool.m_selectedObjects.size() > 0)
				{
					m_mouseTool.UpdateAxisArrows(m_mouseTool.m_selectedObjects[m_mouseTool.m_selectedObjects.size() - 1]);
				}
			}

			//Redo action
			if (m_keyArray['Y'])
			{
				m_keyArray['Y'] = false;
				RedoAction();

				if (m_mouseTool.m_selectedObjects.size() > 0)
				{
					m_mouseTool.UpdateAxisArrows(m_mouseTool.m_selectedObjects[m_mouseTool.m_selectedObjects.size() - 1]);
				}
			}
		}
		else m_mouseTool.m_pickMultiple = false;  m_d3dRenderer.m_camera.m_arcBallMovement = false;
	}
	
	if (m_keyArray['F'])
	{
		if (m_mouseTool.m_selectedObjects.size() > 0)
		{
			m_d3dRenderer.m_camera.m_arcBallMovement = true;
			Vector3 p{ m_gameGraph[m_mouseTool.m_selectedObjects[0]].posX, m_gameGraph[m_mouseTool.m_selectedObjects[0]].posY, m_gameGraph[m_mouseTool.m_selectedObjects[0]].posZ };

			m_d3dRenderer.m_camera.m_arcBallOrigin = p;
		}
		else m_d3dRenderer.m_camera.m_arcBallMovement = false;
	}
	else m_d3dRenderer.m_camera.m_arcBallMovement = false;

	//Delete Objects
	if (m_keyArray[46]) // Del
	{
		if (m_mouseTool.m_selectedObjects.size() > 0)
		{
			SaveLastAction();
			DeleteObjects(m_mouseTool.m_selectedObjects);
		}
		m_keyArray[46] = false;

		m_d3dRenderer.renderAxisArrows = false;
		m_mouseTool.m_selectedObjects.clear();
	}

	UpdateTerrainText();
}

void ToolMain::EnableTerrainText(bool enable)
{
	if (enable)
	{
		m_d3dRenderer.showTerrainText = true;
		m_d3dRenderer.renderAxisArrows = false;

		if (m_terrainTool.m_terrainSculptMode == TerrainSculptMode::Vertex)
		{
			m_d3dRenderer.vertexPaint = true;
		}
		else
		{
			m_d3dRenderer.vertexPaint = false;
		}
	}
	else
	{
		m_d3dRenderer.showTerrainText = false;
	}
}

void ToolMain::UpdateTerrainText()
{
	if (m_terrainTool.m_terrainSculptMode == TerrainSculptMode::Vertex)
	{
		m_d3dRenderer.debug1 = m_terrainTool.GetSculptType();
		m_d3dRenderer.debug2 = m_terrainTool.GetManipulationOffset().y;
	}
	else if (m_terrainTool.m_terrainSculptMode == TerrainSculptMode::Paint)
	{
	
	}
}

void ToolMain::SaveHeightmap()
{
	m_d3dRenderer.SaveHeightmap();
}

void ToolMain::SavePreviousHeightmap()
{
	m_d3dRenderer.SavePreviousHeightmap();
}

void ToolMain::UndoHeightmapChanges()
{
	m_d3dRenderer.UndoHeightmapChanges();
}

void ToolMain::LoadTextureToPaint(std::string inPath, int texIndex)
{
	m_d3dRenderer.LoadTextureToPaint(inPath, texIndex);
}

void ToolMain::SaveTerrainTextures()
{
	m_d3dRenderer.SaveTerrainTextures();
}

void ToolMain::SavePreviousTerrainTextures()
{
	m_d3dRenderer.SavePreviousTerrainPaint();
}

void ToolMain::SetTerrainLayerIndex(int index)
{
	m_d3dRenderer.SetTerrainPaintLayer(index);
}

int ToolMain::GetTerrainLayerTextureIndex()
{
	return m_d3dRenderer.GetTerrainLayerTexIndex();
}

int ToolMain::GetTerrainLayerIndex()
{
	return m_d3dRenderer.GetTerrainLayerIndex();
}

void ToolMain::UndoTerrainPaintChanges()
{
	m_d3dRenderer.UndoTerrainPaintChanges();
}

void ToolMain::DeleteObjects(std::vector<int> objectIDs)
{
	for (int i = 0; i < objectIDs.size(); i++)
	{
		m_gameGraph[objectIDs[i]].bMarkedForDelete = true;
	}

	bDoDestroy = true;
}

void ToolMain::CopyObjects(std::vector<GameObject> in_gameObjects)
{
	copyObjects.clear();
	copyObjects = in_gameObjects;
}

std::vector<GameObject> ToolMain::PasteObjects()
{
	for (int i = 0; i < copyObjects.size(); i++)
	{
		// add the object to scenegraph
		m_gameGraph.push_back(copyObjects[i]);

		//Process into renderable object
		std::vector<SceneObject> sceneGraph = GameGraphToSceneGraph(m_gameGraph);
		m_d3dRenderer.BuildDisplayList(&sceneGraph);
		m_d3dRenderer.UpdateSceneList(&m_gameGraph);

		m_mouseTool.m_gameGraph.clear();
		for (int i = 0; i < m_gameGraph.size(); i++)
		{
			m_mouseTool.m_gameGraph.push_back(&m_gameGraph[i]);
		}
	}

	return copyObjects;
}

void ToolMain::DestroyObjects()
{
	for (int i = 0; i < m_gameGraph.size(); i++)
	{
		if (m_gameGraph[i].bMarkedForDelete == true)
		{
			m_gameGraph.erase(m_gameGraph.begin() + i);
			i = -1;
		}
	}

	//Process into renderable object
	std::vector<SceneObject> sceneGraph = GameGraphToSceneGraph(m_gameGraph);
	m_d3dRenderer.BuildDisplayList(&sceneGraph);
	m_d3dRenderer.UpdateSceneList(&m_gameGraph);

	m_mouseTool.m_gameGraph.clear();
	for (int i = 0; i < m_gameGraph.size(); i++)
	{
		m_mouseTool.m_gameGraph.push_back(&m_gameGraph[i]);
	}

	bDoDestroy = false;
}

void ToolMain::SaveLastAction()
{
	m_undoGameGraph.push_back(m_gameGraph);

	m_undoChunk.push_back(m_chunk);
}

void ToolMain::UndoAction()
{
	if (m_undoGameGraph.size() > 0)
	{
		m_redoGameGraph.push_back(m_gameGraph);

		//Set the game graph to be the one from the last action.
		m_gameGraph = m_undoGameGraph[m_undoGameGraph.size() - 1];

		//Remove the last action from the action list.
		m_undoGameGraph.erase(m_undoGameGraph.begin() + m_undoGameGraph.size() - 1);
	}

	if (m_undoChunk.size() > 0)
	{
		m_chunk = m_undoChunk[m_undoChunk.size() - 1];
		m_undoChunk.erase(m_undoChunk.begin() + m_undoChunk.size() - 1);
	}

	//Process into renderable object
	std::vector<SceneObject> sceneGraph = GameGraphToSceneGraph(m_gameGraph);
	m_d3dRenderer.BuildDisplayList(&sceneGraph);
	m_d3dRenderer.UpdateSceneList(&m_gameGraph);

	m_mouseTool.m_gameGraph.clear();
	for (int i = 0; i < m_gameGraph.size(); i++)
	{
		m_mouseTool.m_gameGraph.push_back(&m_gameGraph[i]);
	}
}

void ToolMain::RedoAction()
{
	if (m_redoGameGraph.size() > 0)
	{
		m_undoGameGraph.push_back(m_gameGraph);

		//Set the game graph to be the one from the last action.
		m_gameGraph = m_redoGameGraph[m_redoGameGraph.size() - 1];

		//Remove the last action from the action list.
		m_redoGameGraph.erase(m_redoGameGraph.begin() + m_redoGameGraph.size() - 1);
	}

	if (m_redoChunk.size() > 0)
	{
		m_chunk = m_redoChunk[m_redoChunk.size() - 1];
		m_redoChunk.erase(m_redoChunk.begin() + m_redoChunk.size() - 1);
	}

	//Process into renderable object
	std::vector<SceneObject> sceneGraph = GameGraphToSceneGraph(m_gameGraph);
	m_d3dRenderer.BuildDisplayList(&sceneGraph);
	m_d3dRenderer.UpdateSceneList(&m_gameGraph);

	m_mouseTool.m_gameGraph.clear();
	for (int i = 0; i < m_gameGraph.size(); i++)
	{
		m_mouseTool.m_gameGraph.push_back(&m_gameGraph[i]);
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


