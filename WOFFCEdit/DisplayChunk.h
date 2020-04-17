#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "ChunkObject.h"

//geometric resoltuion - note,  hard coded.
#define TERRAINRESOLUTION 128

struct RayToDisplayChunkReturn
{
	int row;
	int column;
	DirectX::SimpleMath::Vector3 hit_location;
	bool did_hit = false;
};

class DisplayChunk
{
public:
	DisplayChunk();
	~DisplayChunk();
	void PopulateChunkData(ChunkObject * SceneChunk);
	void RenderBatch(std::shared_ptr<DX::DeviceResources>  DevResources);
	void InitialiseBatch();	//initial setup, base coordinates etc based on scale
	void LoadHeightMap(std::shared_ptr<DX::DeviceResources>  DevResources);
	void SaveHeightMap();			//saves the heigtmap back to file.
	void UpdateTerrain();			//updates the geometry based on the heigtmap
	void UpdateHeightmap();			//updates the heightmap based on the geometry
	void GenerateHeightmap();		//creates or alters the heightmap
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalTexture>>  m_batch;
	std::unique_ptr<DirectX::BasicEffect>       m_terrainEffect;

	ID3D11ShaderResourceView *					m_texture_diffuse;				//diffuse texture
	Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_terrainInputLayout;

	inline DirectX::VertexPositionNormalTexture GetTerrainGeometry(int row, int column) { return m_terrainGeometry[row][column]; }
	inline void SetTerrainGeometryPosition(int row, int column, DirectX::SimpleMath::Vector3 p) {m_terrainGeometry[row][column].position = p; }
	inline DirectX::XMFLOAT3 GetTerrainGeometryPosition(int row, int column) { return m_terrainGeometry[row][column].position; }

	void SavePrevHeightmap();

	void CalculateTerrainNormals();

	void HeightmapUndo();

private:
	
	DirectX::VertexPositionNormalTexture m_terrainGeometry[TERRAINRESOLUTION][TERRAINRESOLUTION];

	BYTE m_heightMap[TERRAINRESOLUTION*TERRAINRESOLUTION];
	BYTE m_prevHeightMap[TERRAINRESOLUTION*TERRAINRESOLUTION];

	float	m_terrainHeightScale;
	int		m_terrainSize;				//size of terrain in metres
	float	m_textureCoordStep;			//step in texture coordinates between each vertex row / column
	float   m_terrainPositionScalingFactor;	//factor we multiply the position by to convert it from its native resolution( 0- Terrain Resolution) to full scale size in metres dictated by m_Terrainsize
	
	std::string m_name;
	int m_chunk_x_size_metres;
	int m_chunk_y_size_metres;
	int m_chunk_base_resolution;
	std::string m_heightmap_path;
	std::string m_tex_diffuse_path;
	std::string m_tex_splat_alpha_path;
	std::string m_tex_splat_1_path;
	std::string m_tex_splat_2_path;
	std::string m_tex_splat_3_path;
	std::string m_tex_splat_4_path;
	bool m_render_wireframe;
	bool m_render_normals;
	int m_tex_diffuse_tiling;
	int m_tex_splat_1_tiling;
	int m_tex_splat_2_tiling;
	int m_tex_splat_3_tiling;
	int m_tex_splat_4_tiling;

};

