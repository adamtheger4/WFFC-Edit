//
// Game.h
//

#pragma once

#include "Camera.h"
#include "DeviceResources.h"
#include "StepTimer.h"
#include "GameObject.h"
#include "DisplayObject.h"
#include "DisplayChunk.h"
#include "ChunkObject.h"
#include "InputCommands.h"
#include <vector>

struct Quad
{
	DirectX::SimpleMath::Vector3 v1;
	DirectX::SimpleMath::Vector3 v2;
	DirectX::SimpleMath::Vector3 v3;
	DirectX::SimpleMath::Vector3 v4;
};

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:

	Game();
	~Game();

	// Initialization and management
	void Initialize(HWND window, int width, int height);
	void SetGridState(bool state);

	// Basic game loop
	void Tick(InputCommands * Input);
	void Render();

	// Rendering helpers
	void Clear();

	// IDeviceNotify
	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	//tool specific
	void BuildDisplayList(std::vector<SceneObject> * SceneGraph); //note vector passed by reference 
	void UpdateSceneList(std::vector<GameObject> * SceneGraph); //Updates input sceneGraph to match the corresponding objects in display list.
	void BuildDisplayChunk(ChunkObject *SceneChunk);
	void SaveDisplayChunk(ChunkObject *SceneChunk);	//saves geometry et al
	void ClearDisplayList();

	RayToDisplayChunkReturn RayToDisplayChunkCollision(DirectX::SimpleMath::Ray ray);
	RayToDisplayChunkReturn GetCurrentTerrainPoint();
	void SculptTerrain(int row, int column, DirectX::XMFLOAT3 offset, bool smooth_sculpt);
	void SmoothSculptTerrain(int row, int column, DirectX::XMFLOAT3 offset);
	std::vector<DirectX::SimpleMath::Vector3> HalfRay(DirectX::SimpleMath::Vector3 v1, DirectX::SimpleMath::Vector3 v2, bool top);

	inline DirectX::SimpleMath::Vector3 GetDisplayObjPos(int objID) { return m_displayList[objID].m_position; };
	inline DirectX::SimpleMath::Vector3 GetDisplayObjRotation(int objID) {return  m_displayList[objID].m_orientation;}
	inline DirectX::SimpleMath::Vector3 GetDisplayObjScale(int objID) { return  m_displayList[objID].m_scale; }

	//Object specific
	void MoveSelectedObject(int select_obj_ID, DirectX::SimpleMath::Vector3 in_vector);
	inline void RotateSelectedObject(int select_obj_ID, DirectX::SimpleMath::Vector3 in_vector) { m_displayList[select_obj_ID].RotateObject(in_vector, m_dt); };
	inline void ScaleSelectedObject(int select_obj_ID, DirectX::SimpleMath::Vector3 in_vector) { m_displayList[select_obj_ID].ScaleObject(in_vector, m_dt); };

	inline void SetSelectedObjectPosition(int select_obj_ID, DirectX::SimpleMath::Vector3 in_vector) { m_displayList[select_obj_ID].m_position = in_vector; }
	inline void SetSelectedObjectRotation(int select_obj_ID, DirectX::SimpleMath::Vector3 in_vector) { m_displayList[select_obj_ID].m_orientation = in_vector; }
	inline void SetSelectedObjectScale(int select_obj_ID, DirectX::SimpleMath::Vector3 in_vector) { m_displayList[select_obj_ID].m_scale = in_vector; }
	 
	bool renderAxisArrows = false;
	DirectX::SimpleMath::Vector3 x_arrow;
	DirectX::SimpleMath::Vector3 y_arrow;
	DirectX::SimpleMath::Vector3 z_arrow;

	inline DirectX::SimpleMath::Matrix GetViewMatrix() { return m_view; }
	inline DirectX::SimpleMath::Matrix GetProjMatrix() { return m_projection; }
	inline DirectX::SimpleMath::Matrix GetWorldMatrix() { return m_world; }

	inline DirectX::SimpleMath::Vector3 GetActiveCameraLocation() { return m_camera.m_camPosition; }

	inline DirectX::Mouse::State GetMouseState() { return m_mouse->GetState(); }

	std::vector<Quad> BoxToQuads(DirectX::SimpleMath::Vector3 center, DirectX::SimpleMath::Vector3 extents);

	void SetSelectedObj(int selectedID);

	//Used to draw axis arrows.
	std::vector<Quad> m_axisBoxList; 
	DirectX::XMFLOAT4 xAxisColor { 1.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 yAxisColor { 0.0f, 1.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 zAxisColor { 0.0f, 0.0f, 1.0f, 1.0f };

	float debug1; 
	float debug2;
	float debug3;

	bool showObjText = false;
	bool showTerrainText = false;
#ifdef DXTK_AUDIO
	void NewAudioDevice();
#endif

private:

	//Implemented Functions.

	//

	void Update(DX::StepTimer const& timer);

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	void XM_CALLCONV DrawGrid(DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color);
	void DrawAxisArrows();

	//tool specific
	std::vector<DisplayObject>			m_displayList;
	DisplayChunk						m_displayChunk;
	InputCommands						m_InputCommands;

	int m_selectedObject;						//ID of current Selection

	void HandleInput();

	//camera
	Camera m_camera;

	//control variables
	bool m_grid;							//grid rendering on / off
	// Device resources.
    std::shared_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;
	float									m_dt;

    // Input devices.
    std::unique_ptr<DirectX::GamePad>       m_gamePad;
    std::unique_ptr<DirectX::Keyboard>      m_keyboard;
    std::unique_ptr<DirectX::Mouse>         m_mouse;

    // DirectXTK objects.
    std::unique_ptr<DirectX::CommonStates>                                  m_states;
    std::unique_ptr<DirectX::BasicEffect>                                   m_batchEffect;
    std::unique_ptr<DirectX::EffectFactory>                                 m_fxFactory;
    std::unique_ptr<DirectX::GeometricPrimitive>                            m_shape;
    std::unique_ptr<DirectX::Model>                                         m_model;
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch;
    std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
    std::unique_ptr<DirectX::SpriteFont>                                    m_font;

#ifdef DXTK_AUDIO
    std::unique_ptr<DirectX::AudioEngine>                                   m_audEngine;
    std::unique_ptr<DirectX::WaveBank>                                      m_waveBank;
    std::unique_ptr<DirectX::SoundEffect>                                   m_soundEffect;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect1;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect2;
#endif

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture1;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture2;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>                               m_batchInputLayout;

#ifdef DXTK_AUDIO
    uint32_t                                                                m_audioEvent;
    float                                                                   m_audioTimerAcc;

    bool                                                                    m_retryDefault;
#endif

    DirectX::SimpleMath::Matrix                                             m_world;
    DirectX::SimpleMath::Matrix                                             m_view;
    DirectX::SimpleMath::Matrix                                             m_projection;


};

std::wstring StringToWCHART(std::string s);