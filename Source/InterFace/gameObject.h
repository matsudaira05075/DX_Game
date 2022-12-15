//==========================================================
//	[GameObjectクラス<InterFace>]
//
//									作成日 2022/07/04
//									作成者 松平康功
//==========================================================
#pragma once
#include <list>
#include <vector>
#include <typeinfo>

class GameObject
{
private:
	bool m_alive = true;	// 生存

protected:
	bool		m_destroy = false;

public:
	GameObject() {}
	virtual ~GameObject() {}
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_size;

	std::string m_name;
	std::string m_tag;
	std::string m_type;

	// セッター
	void		SetPosition(D3DXVECTOR3 position) { m_position = position; }
	void		SetScale(D3DXVECTOR3 scale) { m_scale = scale; }

	// ゲッター
	D3DXVECTOR3 GetPosition() { return m_position; }
	D3DXVECTOR3 GetScale() { return m_scale; }
	bool GetAlive() { return this->m_alive; };

	void SetDestroy() { m_destroy = true; }	// 消す予約


	// 前方向ベクトル取得
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	// 横方向ベクトル取得
	D3DXVECTOR3 GetForwardX()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;

		return forward;
	}

	// 実際に消す関数
	bool Destroy()
	{
		if (m_destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}
};