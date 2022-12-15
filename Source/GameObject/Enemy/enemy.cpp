#include "../../Main/main.h"
#include "../../System/manager.h"
#include "../../System/renderer.h"
#include "../Camera/camera.h"
#include "../../System/input.h"
#include "../../System/Model/animationModel.h"
#include "enemy.h"
#include "../Field/meshField.h"




void Enemy::Init()
{

	m_Model = new AnimationModel();
	m_Model->Load("asset\\animation\\player_Idle.fbx");

	m_Model->LoadAnimation("asset\\animation\\player_Idle.fbx", "Idle");
	//m_Model->LoadAnimation("asset\\animation\\player_Run.fbx","Run");
	//m_Model->LoadAnimation("asset\\animation\\player_Jump.fbx", "RunJump");

	m_AnimationName = "Idle";

	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	m_velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_runSpeed = 0.004f;			// プレイヤーの移動速度
	m_maxRunSpeed = 0.15f;
	m_jumpPower = 0.2f;			// プレイヤーのジャンプ力
	m_jumpFlg = false;

	m_moveFlg = false;
	m_Frame = 0;

	/*m_stateIdle = new EnemyState_Idle;
	m_stateRun = new EnemyState_Run;
	m_stateJump = new EnemyState_Jump;

	m_state = m_stateIdle;*/
}


void Enemy::Uninit()
{


}


void Enemy::Update()
{

	//
	////m_state->Update(this);

	//UpdateGravity();
	//

	//// メッシュフィールドの高さ取得
	//MeshField* field = Manager::GetScene()->GetGameObject<MeshField>(1);
	//float groundHeight = field->GetHeight(m_position);

	//// メッシュフィールドとの接地判定
	//if (m_position.y <= groundHeight)
	//{
	//	m_position.y = groundHeight;

	//	if (m_jumpFlg)
	//		m_jumpFlg = false;

	//	/*if (typeid(*m_state) == typeid(EnemyState_Jump)) {
	//		m_state = m_stateIdle;
	//		m_AnimationName = "Idle";
	//	}*/
	//}

	//m_Frame++;
}




void Enemy::Draw()
{
	m_Model->Update(/*m_AnimationName.c_str()*/"Idle", m_Frame);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

}

void Enemy::UpdateMove()
{
	/*D3DXVECTOR3 vec = g_camera->GetCameraVector();
	float angle = (-g_camera->GetCameraAngle()) * D3DX_PI / 180.0f;

	if (typeid(*m_state) != typeid(EnemyState_Jump)) {
		if ((Input::GetKeyPress('W') || Input::GetKeyPress('A') ||
			Input::GetKeyPress('S') || Input::GetKeyPress('D'))) {
			m_state = m_stateRun;
			m_AnimationName = "Run";
		}
		else {
			m_state = m_stateIdle;
			m_AnimationName = "Idle";

		}
	}

		if ((Input::GetKeyPress('W') && Input::GetKeyPress('A')))
		{
			m_velocity += D3DXVECTOR3(-vec.z, 0.0f, vec.x) * m_runSpeed;
			m_velocity += D3DXVECTOR3(vec.x, 0.0f, vec.z) * m_runSpeed;
			m_rotVelocity.y = angle + (D3DX_PI / 4) * 3;
		}
		else if ((Input::GetKeyPress('W') && Input::GetKeyPress('D')))
		{
			m_velocity -= D3DXVECTOR3(-vec.z, 0.0f, vec.x) * m_runSpeed;
			m_velocity += D3DXVECTOR3(vec.x, 0.0f, vec.z) * m_runSpeed;
			m_rotVelocity.y = angle - (D3DX_PI / 4) * 3;

		}
		else if ((Input::GetKeyPress('S') && Input::GetKeyPress('A')))
		{
			m_velocity += D3DXVECTOR3(-vec.z, 0.0f, vec.x) * m_runSpeed;
			m_velocity -= D3DXVECTOR3(vec.x, 0.0f, vec.z) * m_runSpeed;
			m_rotVelocity.y = angle + D3DX_PI / 4;

		}
		else if ((Input::GetKeyPress('S') && Input::GetKeyPress('D')))
		{
			m_velocity -= D3DXVECTOR3(-vec.z, 0.0f, vec.x) * m_runSpeed;
			m_velocity -= D3DXVECTOR3(vec.x, 0.0f, vec.z) * m_runSpeed;
			m_rotVelocity.y = angle - D3DX_PI / 4;

		}
		else
		{
			if (Input::GetKeyPress('A'))
			{
				m_velocity += D3DXVECTOR3(-vec.z, 0.0f, vec.x) * m_runSpeed;
				m_rotVelocity.y = angle + D3DX_PI / 2;
			}
			if (Input::GetKeyPress('D'))
			{
				m_velocity -= D3DXVECTOR3(-vec.z, 0.0f, vec.x) * m_runSpeed;
				m_rotVelocity.y = angle - D3DX_PI / 2;
			}
			if (Input::GetKeyPress('W'))
			{
				m_velocity += D3DXVECTOR3(vec.x, 0.0f, vec.z) * m_runSpeed;
				m_rotVelocity.y = angle + D3DX_PI;

			}
			if (Input::GetKeyPress('S'))
			{
				m_velocity -= D3DXVECTOR3(vec.x, 0.0f, vec.z) * m_runSpeed;
				m_rotVelocity.y = angle;
			}
		}*/


	if (m_velocity.x >= m_maxRunSpeed)
		m_velocity.x = m_maxRunSpeed;
	if (m_velocity.x <= -m_maxRunSpeed)
		m_velocity.x = -m_maxRunSpeed;
	if (m_velocity.z >= m_maxRunSpeed)
		m_velocity.z = m_maxRunSpeed;
	if (m_velocity.z <= -m_maxRunSpeed)
		m_velocity.z = -m_maxRunSpeed;

	float dist = m_rotVelocity.y - m_rotation.y;

	if (dist > D3DX_PI)
		dist -= D3DX_PI * 2.0f;
	else if (dist < -D3DX_PI)
		dist += D3DX_PI * 2.0f;

	m_rotation.y += dist * 0.1f;

	if (m_rotation.y > D3DX_PI)
		m_rotation.y -= D3DX_PI * 2.0f;
	else if (m_rotation.y < -D3DX_PI)
		m_rotation.y += D3DX_PI * 2.0f;

}

void Enemy::ActionJump()
{
	m_velocity.y = m_jumpPower;
	//m_AnimationName = ENEMY_JUMP;
}


void Enemy::UpdateGravity()
{
	// 重力をかける
	m_velocity.y -= 0.01f;

	// 抵抗
	m_velocity.x -= m_velocity.x * 0.1f;
	m_velocity.y -= m_velocity.y * 0.01f;
	m_velocity.z -= m_velocity.z * 0.1f;

	// 移動
	m_position += m_velocity;
}


