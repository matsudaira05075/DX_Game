//==========================================================
//	[water.h]
//
//									çÏê¨ì˙ 2022/07/04
//									çÏê¨é“ èºïΩçNå˜
//==========================================================
#pragma once
#include "../../../InterFace/gameObject.h"

enum WIRESTATE
{
	STRETCH,
	SHRINK,
	BACK,
	ATACK,
};

class Wire : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;

	class			Model* m_Model;
	class			Camera* m_camera;
	class			Player* m_player;
	D3DXVECTOR3				m_tongueVec; // íeÇÃîÚÇÒÇ≈Ç≠ï˚äp
	void SetVertex();

public:
	WIRESTATE m_tongueState;

	void	Init() override;
	void	Uninit() override;
	void	Update() override;
	void	Draw() override;
};
