#pragma once
#include <afxext.h>
#include "pch.h"
#include "DisplayChunk.h"

enum VertexSculptType
{
	Add,
	Dig,
	Flatten
};

enum VertexSculptMode
{
	Hill,
	Plateau
};

enum TerrainSculptMode
{
	Vertex,
	Paint
};


class TerrainTool
{
public:
	TerrainTool();
	~TerrainTool();

	bool	Active = false; // Currently sculpting
	bool	smoothSculpt = true;
	bool	updateTargetHeight = true;
	float	targetHeight = 0.0f;
	RayToDisplayChunkReturn mouseTerrainManipReturn;

	//Terrain tool toggle do once.
	bool	m_Tonce = true;

	TerrainSculptMode m_terrainSculptMode = TerrainSculptMode::Paint;
	VertexSculptMode m_vertexSculptMode = VertexSculptMode::Hill;
	
	inline bool	 GetEnable()	{ return Enabled; }
	inline VertexSculptType GetSculptType()	{ return m_sculptType; }
	inline DirectX::XMFLOAT3 GetManipulationOffset()		{ return manipulationOffset; }

	inline void SetEnable(bool do_enable) { Enabled = do_enable; }
	inline void SetSculptType(VertexSculptType sculptType) { m_sculptType = sculptType; }
	inline void SetManipulationOffset (DirectX::XMFLOAT3 offset){ manipulationOffset = offset; }
;
	void LoadAllTexturePaths();

	std::string m_paintTexturePath = "database/data/terraintextures/rock.dds";
	CStringArray   m_terrainTexturePaths;

private:

	bool	Enabled = false; // Can use tool
	VertexSculptType m_sculptType = VertexSculptType::Add;
	DirectX::XMFLOAT3 manipulationOffset;
};

