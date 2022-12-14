#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"




// 頂点構造体
struct VERTEX_3D
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR2 TexCoord;
};

// パーティクル用頂点レイアウト（法線、ライト無し）
struct VERTEX_3D_PARTICLE
{
	D3DXVECTOR3 Position;
	D3DXVECTOR2 TexCoord;
};

// マテリアル構造体
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];//16byte境界用
};




// マテリアル構造体
struct DX11_MODEL_MATERIAL
{
	MATERIAL		Material;
	class CTexture* Texture;
};


// 描画サブセット構造体
struct DX11_SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];//16byte境界用
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;
};



class Renderer
{
private:
#ifdef _DEBUG
	// Our state
	inline static bool show_demo_window = true;
	inline static bool show_another_window = false;
	inline static bool show_hello_world = true;
	inline static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	// inlineは使えなかったら、C++のバージョンを上げるか、Renderer.cpp内で宣言し直してください。
	// プロパティ/ 構成プロパティ /　全般　/C++言語標準 ->自分はC++17にしてます。

#endif // _DEBUG

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device* m_D3DDevice;
	static ID3D11DeviceContext* m_ImmediateContext;
	static IDXGISwapChain* m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;



	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11InputLayout* m_VertexLayoutParticle;
	static ID3D11Buffer* m_WorldBuffer;
	static ID3D11Buffer* m_ViewBuffer;
	static ID3D11Buffer* m_ProjectionBuffer;
	static ID3D11Buffer* m_MaterialBuffer;
	static ID3D11Buffer* m_LightBuffer;


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;




public:
#ifdef _DEBUG
	static void imguiDraw();
#endif // _DEBUG
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	// バッファ生成
	static HRESULT CreateStructuredBuffer(UINT elementSize, UINT count, void* pInitData, ID3D11Buffer** ppBufferOut);
	static HRESULT CreateStructuredBuffer_DYN(UINT elementSize, UINT count, void* pInitData, ID3D11Buffer** ppBufferOut);
	static HRESULT CreateBufferSRV(ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut);
	static HRESULT CreateBufferUAV(ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUAVOut);


	// Compute Shader
	static void RunComputeShader(ID3D11ComputeShader* pComputeShader, UINT numViews, ID3D11ShaderResourceView** pSRVs, ID3D11Buffer* pCBCS, void* pCSData, DWORD numDataBytes, ID3D11UnorderedAccessView* pUAV, UINT x, UINT y, UINT z);
	static ID3D11Buffer* CreateAndCopyToBuffer(ID3D11Buffer* pBuffer);


	// 描画モード
	static bool m_lineMode;

	static void SetDepthEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);

	static ID3D11Device* GetDevice(void) { return m_D3DDevice; }
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_ImmediateContext; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	static ID3D11InputLayout** GetInputLayout() { return &m_VertexLayout; };
	static ID3D11InputLayout** GetInputLayoutParticle() { return &m_VertexLayoutParticle; };
};
