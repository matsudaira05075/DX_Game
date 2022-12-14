//==========================================================
//	[water.h]
//
//									ì¬“ú 2022/07/04
//									ì¬Ò ¼•½NŒ÷
//==========================================================
#pragma once
#include "../../InterFace/component.h"

class Collider : public Component
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	//VERTEX_3D					m_vertex[4];
	void SetVertex();

public:
}; 
