#pragma once
#include "pch.h"
#include "DisplayChunk.h"

enum TerrainSculptType
{
	Add,
	Dig,
	Flatten
};

enum TerrainSculptMode
{
	Hill,
	Plateau
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

	TerrainSculptMode m_terrainSculptMode = TerrainSculptMode::Hill;

	inline bool	 GetEnable()	{ return Enabled; }
	inline TerrainSculptType GetSculptType()	{ return m_sculptType; }
	inline DirectX::XMFLOAT3 GetManipulationOffset()		{ return manipulationOffset; }

	inline void SetEnable(bool do_enable) { Enabled = do_enable; }
	inline void SetSculptType(TerrainSculptType sculptType) { m_sculptType = sculptType; }
	inline void SetManipulationOffset (DirectX::XMFLOAT3 offset){ manipulationOffset = offset; }
private:

	bool	Enabled = false; // Can use tool
	TerrainSculptType m_sculptType = TerrainSculptType::Add;
	DirectX::XMFLOAT3 manipulationOffset;
};

