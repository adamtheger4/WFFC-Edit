#include <string>
#include <sstream>
#include <fstream> 
#include "DisplayChunk.h"
#include "Game.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

DisplayChunk::DisplayChunk()
{
	//terrain size in meters. note that this is hard coded here, we COULD get it from the terrain chunk along with the other info from the tool if we want to be more flexible.
	m_terrainSize = 512;
	m_terrainHeightScale = 0.25;  //convert our 0-256 terrain to 64
	m_textureCoordStep = 1.0 / (TERRAINRESOLUTION-1);	//-1 becuase its split into chunks. not vertices.  we want tthe last one in each row to have tex coord 1
	m_terrainPositionScalingFactor = m_terrainSize / (TERRAINRESOLUTION-1);
}


DisplayChunk::~DisplayChunk()
{
}

void DisplayChunk::PopulateChunkData(ChunkObject * SceneChunk)
{
	m_name = SceneChunk->name;
	m_chunk_x_size_metres = SceneChunk->chunk_x_size_metres;
	m_chunk_y_size_metres = SceneChunk->chunk_y_size_metres;
	m_chunk_base_resolution = SceneChunk->chunk_base_resolution;
	m_heightmap_path = SceneChunk->heightmap_path;
	m_tex_diffuse_path = SceneChunk->tex_diffuse_path;
	m_tex_splat_alpha_path = SceneChunk->tex_splat_alpha_path;
	m_tex_splat_1_path = SceneChunk->tex_splat_1_path;
	m_tex_splat_2_path = SceneChunk->tex_splat_2_path;
	m_tex_splat_3_path = SceneChunk->tex_splat_3_path;
	m_tex_splat_4_path = SceneChunk->tex_splat_4_path;
	m_render_wireframe = SceneChunk->render_wireframe;
	m_render_normals = SceneChunk->render_normals;
	m_tex_diffuse_tiling = SceneChunk->tex_diffuse_tiling;
	m_tex_splat_1_tiling = SceneChunk->tex_splat_1_tiling;
	m_tex_splat_2_tiling = SceneChunk->tex_splat_2_tiling;
	m_tex_splat_3_tiling = SceneChunk->tex_splat_3_tiling;
	m_tex_splat_4_tiling = SceneChunk->tex_splat_4_tiling;
}

void DisplayChunk::RenderBatch(std::shared_ptr<DX::DeviceResources>  DevResources)
{
	auto context = DevResources->GetD3DDeviceContext();

	context->IASetInputLayout(m_terrainInputLayout.Get());

	m_batch->Begin();	
	m_terrainEffect->SetTexture(m_texture_diffuse);
	m_terrainEffect->Apply(context);

	for (size_t i = 0; i < TERRAINRESOLUTION-1; i++)	//looping through QUADS.  so we subtrack one from the terrain array or it will try to draw a quad starting with the last vertex in each row. Which wont work
	{
		for (size_t j = 0; j < TERRAINRESOLUTION-1; j++)//same as above
		{
			m_batch->DrawQuad(m_terrainGeometry[i][j], m_terrainGeometry[i][j+1], m_terrainGeometry[i+1][j+1], m_terrainGeometry[i+1][j]); //bottom left bottom right, top right top left.
		}
	}
	m_batch->End();

	/*if (m_terrain.size() > 0)
	{
		m_batch->Begin();
		m_terrainEffect->SetTexture(m_terrainPaintDiffuse);
		m_terrainEffect->Apply(context);
		DrawTerrain(m_terrain);
		m_batch->End();
	}*/

	if (m_terrains.size() > 0)
	{
		for (int i = 0; i < m_terrains.size(); i++)
		{
			m_batch->Begin();
			m_terrainEffect->SetTexture(m_terrains[i].texture_diffuse);
			m_terrainEffect->Apply(context);
			DrawTerrain(m_terrains[i].terrain);
			m_batch->End();
		}
	}

}

void DisplayChunk::InitialiseBatch()
{
	//build geometry for our terrain array
	//iterate through all the vertices of our required resolution terrain.
	int index = 0;

	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_terrainGeometry[i][j].position =			Vector3(j*m_terrainPositionScalingFactor-(0.5*m_terrainSize), (float)(m_heightMap[index])*m_terrainHeightScale, i*m_terrainPositionScalingFactor-(0.5*m_terrainSize));	//This will create a terrain going from -64->64.  rather than 0->128.  So the center of the terrain is on the origin
			m_terrainGeometry[i][j].normal =			Vector3(0.0f, 1.0f, 0.0f);						//standard y =up
			m_terrainGeometry[i][j].textureCoordinate =	Vector2(((float)m_textureCoordStep*j)*m_tex_diffuse_tiling, ((float)m_textureCoordStep*i)*m_tex_diffuse_tiling);				//Spread tex coords so that its distributed evenly across the terrain from 0-1
			
		}
	}
	CalculateTerrainNormals();
}

void DisplayChunk::InitTerrainLayers(int numLayers)
{
	for (int i = 0; i < numLayers; i++)
	{
		TerrainType t;
		t.texture_diffuse = m_texture_diffuse;

		m_terrains.push_back(t);
	}
}

void DisplayChunk::LoadHeightMap(std::shared_ptr<DX::DeviceResources>  DevResources)
{
	auto device = DevResources->GetD3DDevice();
	auto devicecontext = DevResources->GetD3DDeviceContext();

	//load in heightmap .raw
	FILE *pFile = NULL;

	// Open The File In Read / Binary Mode.

	pFile = fopen(m_heightmap_path.c_str(), "rb");
	// Check To See If We Found The File And Could Open It
	if (pFile == NULL)
	{
		// Display Error Message And Stop The Function
		MessageBox(NULL, L"Can't Find The Height Map!", L"Error", MB_OK);
		return;
	}

	// Here We Load The .RAW File Into Our pHeightMap Data Array
	// We Are Only Reading In '1', And The Size Is (Width * Height)
	fread(m_heightMap, 1, TERRAINRESOLUTION*TERRAINRESOLUTION, pFile);

	fclose(pFile);

	//load in texture diffuse
	
	//load the diffuse texture
	std::wstring texturewstr = StringToWCHART(m_tex_diffuse_path);
	HRESULT rs;	
	rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), NULL, &m_texture_diffuse);	//load tex into Shader resource	view and resource
	
	//setup terrain effect
	m_terrainEffect = std::make_unique<BasicEffect>(device);
	m_terrainEffect->EnableDefaultLighting();
	m_terrainEffect->SetLightingEnabled(true);
	m_terrainEffect->SetTextureEnabled(true);
	m_terrainEffect->SetTexture(m_texture_diffuse);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_terrainEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	//setup batch
	DX::ThrowIfFailed(
		device->CreateInputLayout(VertexPositionNormalTexture::InputElements,
			VertexPositionNormalTexture::InputElementCount,
			shaderByteCode,
			byteCodeLength,
			m_terrainInputLayout.GetAddressOf())
		);

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormalTexture>>(devicecontext);

	
}

void DisplayChunk::SaveHeightMap()
{
	UpdateHeightmap();
	
	FILE *pFile = NULL;

	// Open The File In Read / Binary Mode.
	pFile = fopen(m_heightmap_path.c_str(), "wb+");;
	// Check To See If We Found The File And Could Open It
	if (pFile == NULL)
	{
		// Display Error Message And Stop The Function
		MessageBox(NULL, L"Can't Find The Height Map!", L"Error", MB_OK);
		return;
	}

	fwrite(m_heightMap, 1, TERRAINRESOLUTION*TERRAINRESOLUTION, pFile);
	fclose(pFile);
	
}

void DisplayChunk::SaveAllTextures()
{
	for (int i = 0; i < m_terrains.size(); i++)
	{
		std::string path = "database/terrain";
		path += std::to_string(i);
		path += ".csv";

		SaveTexture(path, m_terrains[i].terrain, m_terrains[i].texPath);
	}
}

void DisplayChunk::SaveTexture(std::string filePath, std::vector<std::pair<int, int>> inTerrain, std::string texPath)
{
	std::ofstream f(filePath);

	f << texPath << "\n";

	for (int i = 0; i < inTerrain.size(); i++)
	{
		f << inTerrain[i].first << ",";
		f << inTerrain[i].second << "\n";
	}

	f.close();
}

void DisplayChunk::LoadAllTextures()
{
	for (int i = 0; i < m_terrains.size(); i++)
	{
		std::string path = "database/terrain";
		path += std::to_string(i);
		path += ".csv";

		LoadTexture(path, m_terrains[i].terrain, m_terrains[i].texPath);
	}
}

void DisplayChunk::LoadTexture(std::string filePath, std::vector<std::pair<int, int>>& inTerrain, std::string &texPath)
{
	std::ifstream f(filePath);
	bool bDoOnce = true; // DoOnce to read the top line of the file.

	std::string line;
	while (std::getline(f, line))
	{
		std::pair<int, int> p; // current row/column
		std::string s;
		
			std::stringstream ss(line);

			if (bDoOnce)
			{
				bDoOnce = false;

				std::string x;
				getline(ss, x);
				std::stringstream(x) >> s;

				texPath = s;
			}
			else
			{
				std::string x;
				getline(ss, x, ',');
				std::stringstream(x) >> p.first;

			
				std::string y;
				getline(ss, y);
				std::stringstream(y) >> p.second;

				inTerrain.push_back(p);
			}
	}

	f.close();
}

void DisplayChunk::UpdateTerrain()
{
	//all this is doing is transferring the height from the heigtmap into the terrain geometry.
	int index;
	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_terrainGeometry[i][j].position.y = (float)(m_heightMap[index])*m_terrainHeightScale;	
		}
	}
	CalculateTerrainNormals();

}

void DisplayChunk::UpdateHeightmap()
{
	//all this is doing is transferring the height from the terrain geometry into the heightmap.
	int index;
	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_heightMap[index] = m_terrainGeometry[i][j].position.y / m_terrainHeightScale;
		}
	}
}

void DisplayChunk::GenerateHeightmap()
{
	//insert how YOU want to update the heigtmap here! :D
	for (int i = 0; i < (TERRAINRESOLUTION); i++)
	{
		for (int j = 0; j < (TERRAINRESOLUTION ); j++)
		{
			m_terrainGeometry[i][j].position.x++;
		}
	}

	CalculateTerrainNormals();
}

void DisplayChunk::CalculateTerrainNormals()
{
	int index1, index2, index3, index4;
	DirectX::SimpleMath::Vector3 upDownVector, leftRightVector, normalVector;

	for (int i = 0; i<(TERRAINRESOLUTION - 1); i++)
	{
		for (int j = 0; j<(TERRAINRESOLUTION - 1); j++)
		{
			upDownVector.x = (m_terrainGeometry[i + 1][j].position.x - m_terrainGeometry[i - 1][j].position.x);
			upDownVector.y = (m_terrainGeometry[i + 1][j].position.y - m_terrainGeometry[i - 1][j].position.y);
			upDownVector.z = (m_terrainGeometry[i + 1][j].position.z - m_terrainGeometry[i - 1][j].position.z);

			leftRightVector.x = (m_terrainGeometry[i][j - 1].position.x - m_terrainGeometry[i][j + 1].position.x);
			leftRightVector.y = (m_terrainGeometry[i][j - 1].position.y - m_terrainGeometry[i][j + 1].position.y);
			leftRightVector.z = (m_terrainGeometry[i][j - 1].position.z - m_terrainGeometry[i][j + 1].position.z);


			leftRightVector.Cross(upDownVector, normalVector);	//get cross product
			normalVector.Normalize();			//normalise it.

			m_terrainGeometry[i][j].normal = normalVector;	//set the normal for this point based on our result
		}
	}
}

void DisplayChunk::PaintTerrain(int row, int column, bool add)
{
	if (add)
	{
		//If this terrain isnt already painted on with this texture layer
		if(!CheckTerrainDuplicates(row, column))
		m_terrains[m_terrainIndex].terrain.push_back(std::pair<int, int>(row, column));
	}
	else
	{
		RemoveTerrain(row, column);
	}
}

bool DisplayChunk::CheckTerrainDuplicates(int row, int column)
{
	for (int i = 0; i < m_terrains[m_terrainIndex].terrain.size(); i++)
	{
		if (m_terrains[m_terrainIndex].terrain[i].first == row && m_terrains[m_terrainIndex].terrain[i].second == column)
		{
			return true;
		}
	}

	return false;
}

void DisplayChunk::RemoveTerrain(int row, int column)
{
	for (int i = 0; i < m_terrains[m_terrainIndex].terrain.size(); i++)
	{
		if (m_terrains[m_terrainIndex].terrain[i].first == row && m_terrains[m_terrainIndex].terrain[i].second == column)
		{
			m_terrains[m_terrainIndex].terrain.erase(m_terrains[m_terrainIndex].terrain.begin() + i);
		}
	}
}

void DisplayChunk::DeleteLayer()
{
	m_terrains[m_terrainIndex].terrain.clear();
}

void DisplayChunk::SavePrevHeightmap()
{
	//all this is doing is transferring the height from the terrain geometry into the heightmap.
	int index;
	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_prevHeightMap[index] = m_terrainGeometry[i][j].position.y / m_terrainHeightScale;
		}
	}
}

void DisplayChunk::HeightmapUndo()
{
	//all this is doing is transferring the height from the terrain geometry into the heightmap.
	int index;
	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_heightMap[index] = (float)(m_prevHeightMap[index]);
		}
	}

	UpdateTerrain();
	CalculateTerrainNormals();
}

void DisplayChunk::SavePrevTerrainTexture()
{
	//m_prevTerrain = m_terrain;
	m_prevTerrains = m_terrains;
}

void DisplayChunk::TerrainPaintUndo()
{
	//m_terrain = m_prevTerrain;
	m_terrains = m_prevTerrains;	
}

void DisplayChunk::DrawTerrain(std::vector<std::pair<int, int>> inTerrain)
{
	for (int i = 0; i < inTerrain.size(); i++)
	{
		m_batch->DrawQuad(
			m_terrainGeometry[inTerrain[i].first][inTerrain[i].second],
			m_terrainGeometry[inTerrain[i].first][inTerrain[i].second + 1],
			m_terrainGeometry[inTerrain[i].first + 1][inTerrain[i].second + 1],
			m_terrainGeometry[inTerrain[i].first + 1][inTerrain[i].second]		
		);
	}
}

void DisplayChunk::SetTerrainLayerTexture(ID3D11ShaderResourceView * texture, std::string texPath, int texIndex)
{
	m_terrains[m_terrainIndex].texture_diffuse = texture;
	m_terrains[m_terrainIndex].texIndex = texIndex;
	m_terrains[m_terrainIndex].texPath = texPath;
}

void DisplayChunk::SetTerrainLayerTerrain(std::vector<std::pair<int, int>> terrain)
{
	m_terrains[m_terrainIndex].terrain = terrain;
}

void DisplayChunk::SetTerrainLayerIndex(int index)
{
	if (index < m_terrains.size())
	{
		m_terrainIndex = index;
	}
}

int DisplayChunk::GetTerrainLayerTexIndex()
{
	return m_terrains[m_terrainIndex].texIndex;
}
