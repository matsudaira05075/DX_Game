#pragma once
#include "../../InterFace/gameObject.h"

//==========================================================
// マクロ定義
//==========================================================
#define CAMERA_ANGLE_SPEED (3.0f)		// カメラの回転速度
#define CAMERA_LOOK_AT_HEIGHT (2.5f)	// カメラの注視点の高さ
#define CAMERA_LOOK_AT_DISTANCE	(6.0f)	// カメラと注視点の距離

#define CAMERA_VIEW_UNDER (-30.0f)
#define CAMERA_VIEW_UPPER (80.0f)

#define SHAKE_BACK_POWER (0.9f)
#define SHAKE_LOW_POWER (0.01f)
#define SHAKE_MIDLLE_POWER (0.02f)
#define SHAKE_HIGH_POWER (0.05f)

class Camera : public GameObject
{

private:
	class Player* m_player = nullptr;
	D3DXVECTOR3			m_target;	// playerの位置
	D3DXVECTOR3			m_vector;
	D3DXVECTOR3			m_shakePower;
	bool				m_shakeUse;
	float				m_cameraHAngle;
	float				m_cameraVAngle;

public:
	float  SinParam;
	float  CosParam;

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void InputCamera();
	float			GetCameraAngle();
	D3DXVECTOR3		GetCameraVector();
	void			StartShake(float power);
	void			ShakeCamera();
};