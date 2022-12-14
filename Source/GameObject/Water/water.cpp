//==========================================================
//	[water.cpp]
//
//									作成日 2022/07/06
//									作成者 松平康功
//==========================================================
#include "../../Main/main.h"
#include "../../System/renderer.h"
#include "water.h"

namespace 
{
	D3DXVECTOR2	g_uv;
	float		g_opa; // 透明度
}

void Water::Init()
{
	// 初期化
	m_position = D3DXVECTOR3(0.0f, -0.4f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_opa = 0.6f;
	m_size = D3DXVECTOR3(100.0f,0.0f,100.0f);

	SetVertex();
	
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // 0

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\water.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexShader.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "pixelShader.cso");

}

void Water::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Water::Update()
{
	// 水の流れを再現する
	g_uv.x -= 0.001f;
	g_uv.y += 0.001f;
}

void Water::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);


	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	const float split = 5.0f;

	vertex[0].Position = D3DXVECTOR3(-m_size.x, 0.0f, m_size.z);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, g_opa);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f + g_uv.x, 0.0f + g_uv.y);

	vertex[1].Position = D3DXVECTOR3(m_size.x, 0.0f, m_size.z);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, g_opa);
	vertex[1].TexCoord = D3DXVECTOR2(m_size.x/ split + g_uv.x, 0.0f + g_uv.y);

	vertex[2].Position = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, g_opa);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f + g_uv.x, m_size.z / split + g_uv.y);

	vertex[3].Position = D3DXVECTOR3(m_size.x, 0.0f, -m_size.z);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, g_opa);
	vertex[3].TexCoord = D3DXVECTOR2(m_size.x / split + g_uv.x, m_size.z / split + g_uv.y);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// ワールドマトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);


	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Water::SetVertex()
{
	const float split = 5.0f;
	// 頂点設定
	m_vertex[0].Position = D3DXVECTOR3(m_position.x - m_size.x, 0.0f, m_position.z + m_size.z);
	m_vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, g_opa);
	m_vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	m_vertex[1].Position = D3DXVECTOR3(m_position.x + m_size.x, 0.0f, m_position.z + m_size.z);
	m_vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, g_opa);
	m_vertex[1].TexCoord = D3DXVECTOR2(m_size.x / split, 0.0f);

	m_vertex[2].Position = D3DXVECTOR3(m_position.x - m_size.x, 0.0f, m_position.z - m_size.z);
	m_vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, g_opa);
	m_vertex[2].TexCoord = D3DXVECTOR2(0.0f, m_size.z / split);

	m_vertex[3].Position = D3DXVECTOR3(m_position.x + m_size.x, 0.0f, m_position.z - m_size.z);
	m_vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, g_opa);
	m_vertex[3].TexCoord = D3DXVECTOR2(m_size.x / split, m_size.z / split);
}