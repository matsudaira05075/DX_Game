//==========================================================
//	[sky.cpp]
//
//									作成日 2022/07/06
//									作成者 松平康功
//==========================================================
#include "../../Main/main.h"
#include "../../System/manager.h"
#include "../../System/renderer.h"
#include "../../System/Model/model.h"
#include "skyBox.h"
#include "../Camera/camera.h"

void SkyBox::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\sky.obj");


	//==========================================================
	// 初期化
	//==========================================================
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(200.0f, 200.0f, 200.0f);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexShader.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "pixelShader.cso");
}

void SkyBox::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void SkyBox::Update()
{
	//==========================================================
	// プレイヤーの座標を常に中心に置く
	//==========================================================
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>(0);

	const float height = -10.0f;
	D3DXVECTOR3 cameraPosition = camera->GetPosition();
	m_position = cameraPosition;
	m_position.y = height;
}

void SkyBox::Draw()
{
	////==========================================================
	//// 入力レイアウト設定
	////==========================================================
	//Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);


	////==========================================================
	//// シェーダ設定
	////==========================================================
	//Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	//Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	////==========================================================
	//// ワールドマトリックス設定
	////==========================================================
	//D3DXMATRIX world, scale, rot, trans;
	//D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);
	//D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	//world = scale * rot * trans;

	//Renderer::SetWorldMatrix(&world);

	//m_Model->Draw();

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);


	m_Model->Draw();
}