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

void TerrainTool::LoadAllTexturePaths()
{
	CFileFind finder;

	// start working for files
	BOOL bFind = finder.FindFile(_T("database/data/terraintextures/*.dds"));

	while (bFind)
	{
		bFind = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			//wprintf(_T("[%s]\n"), finder.GetFileName());

			m_terrainTexturePaths.Add(finder.GetFileName());

			//m_terrainTexturePaths.Add(s);
		}
		else
			m_terrainTexturePaths.Add(finder.GetFileName());
			//wprintf(_T("%s\n"), finder.GetFileName());
	}

	finder.Close();
}
