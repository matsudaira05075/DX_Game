#include "main.h"
#include "Renderer.h"
#include "Shader.h"
#include <io.h>

std::vector < ID3D11VertexShader*> Shader::mVertexShaderArray;
std::vector < ID3D11PixelShader*> Shader::mPixelShaderArray;
std::vector < ID3D11ComputeShader*> Shader::mComputeShaderArray;
std::vector < ID3D11GeometryShader*> Shader::mGeometryShaderArray;

void Shader::Init() {

	mVertexShaderArray.resize(SHADER_MAX);
	mPixelShaderArray.resize(SHADER_MAX);
	mComputeShaderArray.resize(3);
	mGeometryShaderArray.resize(0);

	// シェーダー生成
	CreateVertexShader(SHADER_TYPE_VSPS::Default);
	CreatePixelShader(SHADER_TYPE_VSPS::Default);
	CreateVertexShader(SHADER_TYPE_VSPS::Unlit);
	CreatePixelShader(SHADER_TYPE_VSPS::Unlit);
	CreateVertexShader(SHADER_TYPE_VSPS::Unlit_HighBrightness);
	CreatePixelShader(SHADER_TYPE_VSPS::Unlit_HighBrightness);
	CreateVertexShader(SHADER_TYPE_VSPS::WithNormal);
	CreatePixelShader(SHADER_TYPE_VSPS::WithNormal);
	CreateVertexShader(SHADER_TYPE_VSPS::WithHeight);
	CreatePixelShader(SHADER_TYPE_VSPS::WithHeight);
	CreateVertexShader(SHADER_TYPE_VSPS::Particle);
	CreatePixelShader(SHADER_TYPE_VSPS::Particle);
	CreateVertexShader(SHADER_TYPE_VSPS::Gizmos);
	CreatePixelShader(SHADER_TYPE_VSPS::Gizmos);

	CreateComputeShader(SHADER_TYPE::SkinMesh);
	CreateComputeShader(SHADER_TYPE::Particle);
	CreateComputeShader(SHADER_TYPE::Particle_Bezier);
}

void Shader::Uninit() {

	for (ID3D11VertexShader* v : mVertexShaderArray) {
		v->Release();
	}

	for (ID3D11PixelShader* p : mPixelShaderArray) {
		p->Release();
	}

	for (ID3D11ComputeShader* c : mComputeShaderArray) {
		c->Release();
	}

	for (ID3D11GeometryShader* g : mGeometryShaderArray) {
		g->Release();
	}

	std::vector<ID3D11VertexShader*>().swap(mVertexShaderArray);
	std::vector<ID3D11PixelShader*>().swap(mPixelShaderArray);
	std::vector<ID3D11ComputeShader*>().swap(mComputeShaderArray);
	std::vector<ID3D11GeometryShader*>().swap(mGeometryShaderArray);
}

void Shader::Use(SHADER_TYPE_VSPS type) {

	Renderer::GetDeviceContext()->VSSetShader(mVertexShaderArray[(int)type], NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(mPixelShaderArray[(int)type], NULL, 0);

}

void Shader::CreateVertexShader(SHADER_TYPE_VSPS type) {

	const char* vspass = "";

	if (type == SHADER_TYPE_VSPS::Default) {
		vspass = "vertexShader.cso";
	}
	else if (type == SHADER_TYPE_VSPS::Unlit) {
		vspass = "UnlitVS.cso";
	}
	else if (type == SHADER_TYPE_VSPS::Unlit_HighBrightness) {
		vspass = "UnlitVS.cso";
	}
	else if (type == SHADER_TYPE_VSPS::WithNormal) {
		vspass = "vertexShader.cso";
	}
	else if (type == SHADER_TYPE_VSPS::WithHeight) {
		vspass = "vertexShader.cso";
	}
	else if (type == SHADER_TYPE_VSPS::Particle) {
		vspass = "ParticleVS.cso";
	}
	else if (type == SHADER_TYPE_VSPS::Gizmos) {
		vspass = "vertexShader.cso";
	}

	// 頂点シェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen(vspass, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		Renderer::GetDevice()->CreateVertexShader(buffer, fsize, NULL, &mVertexShaderArray[int(type)]);

		UINT numElements;
		// 入力レイアウト生成
		if (type == SHADER_TYPE_VSPS::Particle) {
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			numElements = ARRAYSIZE(layout);
			Renderer::GetDevice()->CreateInputLayout(layout, numElements, buffer, fsize, Renderer::GetInputLayoutParticle());
		}
		else {
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			numElements = ARRAYSIZE(layout);
			Renderer::GetDevice()->CreateInputLayout(layout, numElements, buffer, fsize, Renderer::GetInputLayout());
		}

		delete[] buffer;
	}

}

void Shader::CreatePixelShader(SHADER_TYPE_VSPS type) {
	const char* pspass = "";

	if (type == SHADER_TYPE_VSPS::Default) {
		pspass = "pixelShader.cso";
	}
	else if (type == SHADER_TYPE_VSPS::Unlit) {
		pspass = "UnlitPS.cso";
	}
	else if (type == SHADER_TYPE_VSPS::Unlit_HighBrightness) {
		pspass = "UnlitPS_HighBrightness.cso";
	}
	else if (type == SHADER_TYPE_VSPS::WithNormal) {
		pspass = "pixelShader_WithNormal.cso";
	}
	else if (type == SHADER_TYPE_VSPS::WithHeight) {
		pspass = "pixelShader_WithHeight.cso";
	}
	else if (type == SHADER_TYPE_VSPS::Particle) {
		pspass = "ParticlePS.cso";
	}
	else if (type == SHADER_TYPE_VSPS::Gizmos) {
		pspass = "GizmosPS.cso";
	}

	// ピクセルシェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen(pspass, "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		Renderer::GetDevice()->CreatePixelShader(buffer, fsize, NULL, &mPixelShaderArray[int(type)]);

		delete[] buffer;
	}
}

void Shader::CreateComputeShader(SHADER_TYPE type) {

	const char* cspass = "";

	if (type == SHADER_TYPE::SkinMesh) {
		cspass = "MeshAnimationCS.cso";
	}
	else if (type == SHADER_TYPE::Particle) {
		cspass = "ParticleCS.cso";
	}
	else if (type == SHADER_TYPE::Particle_Bezier) {
		cspass = "ParticleCS_Bezier.cso";
	}

	FILE* file;
	long int fsize;

	file = fopen(cspass, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	Renderer::GetDevice()->CreateComputeShader(buffer, fsize, nullptr, &mComputeShaderArray[int(type)]);

	delete[] buffer;

}

void Shader::CreateGeometryShader(SHADER_TYPE_GS type) {

	const char* gspass = "";

	
	FILE* file;
	long int fsize;

	file = fopen(gspass, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	Renderer::GetDevice()->CreateGeometryShader(buffer, fsize, nullptr, &mGeometryShaderArray[int(type)]);
	
	delete[] buffer;
}