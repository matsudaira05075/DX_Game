//==========================================================
//	[UIクラス<InterFace>]
//
//									作成日 2022/07/04
//									作成者 松平康功
//==========================================================
#pragma once
#include "../Main/main.h"

class UI
{
protected: // アクセス指定子
	// playerやfieldにあった共通の機能を纏めた
	bool		m_destroy = false;

	D3DXVECTOR2 m_position;
	D3DXVECTOR2 m_rotation;
	D3DXVECTOR2 m_size;

public:
	UI() {}				// コンストラクタ
	virtual ~UI() {}		// デストラクタ(仮想関数)
	virtual void Init() = 0;	// 純粋仮想関数
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void		SetPosition(D3DXVECTOR2 position) { m_position = position; }
	void		SetSize(D3DXVECTOR2 size) { m_size = size; }

	void SetDestroy() { m_destroy = true; } // 消す予約


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