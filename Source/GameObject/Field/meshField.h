#pragma once

#include "../../InterFace/gameObject.h"

#define FIELD_X (21)
#define FIELD_Z (21)

class MeshField : public GameObject
{

private:
	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11Buffer*				m_IndexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	VERTEX_3D					m_Vertex[FIELD_X][FIELD_Z];

	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertexLayout = NULL;


public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	float GetHeight(D3DXVECTOR3 Position);

};