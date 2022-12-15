//==========================================================
//	[water.cpp]
//
//									作成日 2022/07/06
//									作成者 松平康功
//==========================================================
#include "../../../Main/main.h"
#include "../../../System/renderer.h"
#include "../../../System/Model/model.h"
#include "../../../System/manager.h"
#include "../player.h"
#include"../../Camera/camera.h"
#include "wire.h"

namespace
{
	static float	hikyori;
	static int		g_cnt;		// 時間観測用
}

void Wire::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\tongue.obj");

	// 初期化
	m_position = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
	m_tongueVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	hikyori = 1.0f;
	g_cnt = 0;


	// カメラの向きを取得して弾の方向を決める
	m_camera = Manager::GetScene()->GetGameObject<Camera>(0);
	m_player = Manager::GetScene()->GetGameObject<Player>(1);


	// 目標地点の座標
	m_position = m_player->GetPosition();
	m_position.y += 1.0f;


	m_rotation.y = m_player->GetRotation().y;
	m_tongueState = STRETCH;

	m_Model->m_SubsetArray->Material.Material.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexShader.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "pixelShader.cso");
}

void Wire::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Wire::Update()
{
	//// 常に舌の位置にPlayerの位置を代入する
	//m_position = m_player->GetPosition();
	//m_position.y += 1.0f;

	//// 伸びていく状態
	//if (m_tongueState == STRETCH)
	//{
	//	hikyori += 1.0f;

	//	m_position += m_camera->GetCameraVector() * hikyori;
	//	m_scale.z += 2.0f;

	//	// 一定時間削除(現在の時間-生成時間が一定量を超えていたら削除)
	//	if (m_scale.z >= 20.0f)
	//	{
	//		SetDestroy();
	//		m_tongueState = SHRINK;
	//	}
	//}

	//// 縮んでいく状態
	//else if (m_tongueState == SHRINK)
	//{
	//	hikyori -= 1.0f;

	//	m_position += m_camera->GetCameraVector() * hikyori;
	//	m_scale.x -= 2.0f;

	//	if (m_scale.x <= 0.3f)
	//	{
	//		SetDestroy();
	//		hikyori = 1.0f;
	//		return;
	//	}
	//}

	//// 助走をつけている状態
	//else if (m_tongueState == BACK)
	//{
	//	hikyori += 0.08f;
	//	m_position += m_camera->GetCameraVector() * hikyori;
	//	m_scale.x += 0.08f;

	//	g_cnt++;

	//	if (g_cnt >= 50) {
	//		m_tongueState = ATACK;
	//		g_cnt = 0;
	//	}
	//}

	//// Enemyに向かって突進する状態
	//else if (m_tongueState == ATACK)
	//{
	//	hikyori -= 0.5f;
	//	m_position += m_camera->GetCameraVector() * hikyori;
	//	m_scale.x -= 1.0f;

	//	if (m_scale.x <= 0.3f)
	//	{
	//		SetDestroy();
	//		hikyori = 1.0f;
	//		return;
	//	}
	//}
}

void Wire::Draw()
{
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
