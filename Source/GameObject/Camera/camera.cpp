#include "../../Main/main.h"
#include "../../System/manager.h"
#include "../../System/renderer.h"
#include "camera.h"
#include "../Player/player.h"
#include "../../System/input.h"
#include <math.h>



namespace {
	D3DXVECTOR2 g_oldMousePos;
	D3DXVECTOR2 g_mousePos;
}


void Camera::Init()
{
	m_player = Manager::GetScene()->GetGameObject<Player>(1);
	m_position = D3DXVECTOR3(0.0f, CAMERA_LOOK_AT_HEIGHT, -5.0f);
	m_target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// カメラの向きを初期化
	m_cameraHAngle = 0.0f;
	m_cameraVAngle = 40.0f;

	m_shakeUse = false;
	m_shakePower = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	g_oldMousePos = Input::GetMousePos();
}


void Camera::Uninit()
{

}


void Camera::Update()
{
	if (m_player == nullptr)
		m_player = Manager::GetScene()->GetGameObject<Player>(1);

	if (Input::GetKeyPress(VK_CONTROL))
		return;

	InputCamera();

	// カメラの位置と向きを設定
	{
		D3DXVECTOR3 TempPosition1;
		D3DXVECTOR3 TempPosition2;

		// 注視点はキャラクターモデルの座標から CAMERA_LOOK_AT_HEIGHT 分だけ高い位置
		m_target = m_player->GetPosition();
		m_target.y += CAMERA_LOOK_AT_HEIGHT;

		// カメラの位置はカメラの水平角度と垂直角度から算出
		// 最初に垂直角度を反映した位置を算出
		SinParam = sinf(m_cameraVAngle / 180.0f * D3DX_PI);
		CosParam = cosf(m_cameraVAngle / 180.0f * D3DX_PI);
		TempPosition1.x = 0.0f;
		TempPosition1.y = SinParam * CAMERA_LOOK_AT_DISTANCE;
		TempPosition1.z = -CosParam * CAMERA_LOOK_AT_DISTANCE;

		// 次に水平角度を反映した位置を算出
		SinParam = sinf(m_cameraHAngle / 180.0f * D3DX_PI);
		CosParam = cosf(m_cameraHAngle / 180.0f * D3DX_PI);
		TempPosition2.x = CosParam * TempPosition1.x - SinParam * TempPosition1.z;
		TempPosition2.y = TempPosition1.y;
		TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

		// 算出した座標に注視点の位置を加算したものがカメラの位置
		const D3DXVECTOR3 move = (TempPosition2 + m_target) - m_position;
		const float lerp = 5.0f;
		m_position += move / lerp;

	}

	m_vector = D3DXVECTOR3(m_target.x - m_position.x, m_target.y - m_position.y, m_target.z - m_position.z);

	if (Input::GetKeyTrigger('Q'))
	{
		StartShake(SHAKE_LOW_POWER);
		g_oldMousePos = g_oldMousePos;
		g_mousePos = g_mousePos;
	}

	ShakeCamera();
	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}


void Camera::Draw()
{

	D3DXVECTOR3 aaa(0.0f, 1.0f, 0.0f);

	//ビューマトリクス設定
	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &m_position,
		&m_target, &aaa);

	Renderer::SetViewMatrix(&viewMatrix);


	//プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);

}

void Camera::InputCamera()
{
	if (Input::GetKeyPress(VK_RIGHT))
	{
		m_cameraHAngle -= CAMERA_ANGLE_SPEED;
		if (m_cameraHAngle <= -180.0f)
		{
			m_cameraHAngle += 360.0f;
		}
	}
	if (Input::GetKeyPress(VK_LEFT))
	{
		m_cameraHAngle += CAMERA_ANGLE_SPEED;
		if (m_cameraHAngle >= 180.0f)
		{
			m_cameraHAngle -= 360.0f;
		}
	}
	if (Input::GetKeyPress(VK_UP))
	{
		m_cameraVAngle += CAMERA_ANGLE_SPEED;
		if (m_cameraVAngle >= CAMERA_VIEW_UPPER)
		{
			m_cameraVAngle = CAMERA_VIEW_UPPER;
		}
	}
	if (Input::GetKeyPress(VK_DOWN))
	{
		m_cameraVAngle -= CAMERA_ANGLE_SPEED;
		if (m_cameraVAngle <= CAMERA_VIEW_UNDER)
		{
			m_cameraVAngle = CAMERA_VIEW_UNDER;
		}
	}

	// マウスでの視点操作
	{
		// 現在のマウス座標を格納
		g_mousePos = Input::GetMousePos();

		// 動いた分を求める
		D3DXVECTOR2 diff = g_oldMousePos - g_mousePos;
		const float speedMax = 90.0f;
		const float adjustX = 0.1f;
		const float adjustY = 0.1f;

		// 速度の上限
		if (diff.x >= speedMax)
			diff.x = speedMax;
		if (diff.x <= -speedMax)
			diff.x = -speedMax;

		if (diff.y >= speedMax)
			diff.y = speedMax;
		if (diff.y <= -speedMax)
			diff.y = -speedMax;

		m_cameraHAngle += diff.x * adjustX;	// 実際にアングルを動かす
		m_cameraVAngle -= diff.y * adjustY;	// 実際にアングルを動かす

		if (m_cameraHAngle >= 180.0f)
			m_cameraHAngle -= 360.0f;

		if (m_cameraHAngle <= -180.0f)
			m_cameraHAngle += 360.0f;

		if (m_cameraVAngle >= CAMERA_VIEW_UPPER)
			m_cameraVAngle = CAMERA_VIEW_UPPER;

		if (m_cameraVAngle <= CAMERA_VIEW_UNDER)
			m_cameraVAngle = CAMERA_VIEW_UNDER;
	}
}

float Camera::GetCameraAngle()
{
	return m_cameraHAngle;
}

D3DXVECTOR3 Camera::GetCameraVector()
{
	return m_vector;
}

void Camera::StartShake(float power)
{
	if (!m_shakeUse) {
		m_shakeUse = true;
		m_shakePower = D3DXVECTOR3(-m_vector.z, 0.0f, m_vector.x) * power;
		m_position += m_shakePower;
	}
}

void Camera::ShakeCamera()
{
	m_position -= m_shakePower;
	m_target -= m_shakePower;
	m_shakePower *= -SHAKE_BACK_POWER; // 揺れを減らすために毎フレーム掛ける値
	m_position += m_shakePower;
	m_target += m_shakePower;
	const bool bCanStop = m_shakePower.x <= 0.0f;
	if (bCanStop) {
		m_shakeUse = false;
	}
}