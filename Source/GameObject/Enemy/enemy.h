#pragma once
#include "../../InterFace/gameObject.h"
//#include "PlayerState/playerState.h"



class Enemy : public GameObject
{
private:
	class AnimationModel*	m_Model;
	

	int						m_Frame;
	float					m_BlendRate;

	float					m_runSpeed;		// 移動速度
	float					m_maxRunSpeed;	// 最大移動速度
	float					m_jumpPower; // ジャンプ力
	bool					m_jumpFlg; // ジャンプ中かどうか

	bool					m_moveFlg;
	float					m_angle;

	float	m_vDelta;	// 左右回転の差分を測る
	float	m_vShort;	// 左右回転の短さを比較する

	std::string			m_AnimationName;

	D3DXVECTOR3 m_velocity;
	D3DXVECTOR3 m_rotVelocity;

	
	void UpdateGravity();

public:
	/*class PlayerState* m_state;
	class PlayerState_Idle* m_stateIdle = nullptr;
	class PlayerState_Run*	m_stateRun = nullptr;
	class PlayerState_Jump* m_stateJump = nullptr;*/

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void UpdateMove();
	void ActionJump();
};