#include "TerrainTool.h"
#include <string>


TerrainTool::TerrainTool()
{
	manipulationOffset = DirectX::XMFLOAT3{ 0.0f, 10.0f, 0.0f };
	targetHeight = 1.0f;
}


TerrainTool::~TerrainTool()
{
}
