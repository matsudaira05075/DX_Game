/*
シェーダークラス
*/

#pragma once

#define SHADER_MAX 7

// ComputeShader
enum class SHADER_TYPE
{
	SkinMesh,Particle, Particle_Bezier
};

// GeometryShader
enum class SHADER_TYPE_GS {
	
};

// VertexShader,PixelShader
enum class SHADER_TYPE_VSPS {
	Default,Unlit,Unlit_HighBrightness,WithNormal,WithHeight,Particle,Gizmos
};

class Shader
{
private:

	// 頂点シェーダー
	static std::vector < ID3D11VertexShader*> mVertexShaderArray;
	// ピクセルシェーダー
	static std::vector < ID3D11PixelShader*> mPixelShaderArray;
	// 計算シェーダー
	static std::vector < ID3D11ComputeShader*> mComputeShaderArray;
	// ジオメトリシェーダー
	static std::vector <ID3D11GeometryShader*> mGeometryShaderArray;

	// シェーダー生成
	static void CreateVertexShader(SHADER_TYPE_VSPS type);
	static void CreatePixelShader(SHADER_TYPE_VSPS type);
	static void CreateComputeShader(SHADER_TYPE type);
	static void CreateGeometryShader(SHADER_TYPE_GS type);

public:

	static void Init();
	static void Uninit();

	// 使用（VS+PS）
	static void Use(SHADER_TYPE_VSPS type);
	// Getter
	static std::vector < ID3D11ComputeShader*> GetComputeShaderArray() { return mComputeShaderArray; };
	static std::vector < ID3D11GeometryShader*> GetGeometryShaderArray() { return mGeometryShaderArray; };
};

