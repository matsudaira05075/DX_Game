/*
メッシュクラス
*/

#pragma once

// テクスチャ
struct Texture {
	std::string type;
	std::string path;
	ID3D11ShaderResourceView* texture;
};

// 変形
struct DEFORM_VERTEX {
	aiVector3D Position;
	aiVector3D Normal;
	int mBoneNum;
	std::string mBoneName[4];
	float mBoneWeight[4];
};

// 骨
struct BONE {
	aiMatrix4x4 mMatrix;
	aiMatrix4x4 mAnimationMatrix;
	aiMatrix4x4 mOffsetMatirx;
};
// Compute Shader 用
struct COMPUTEMATRIX {
	aiMatrix4x4 cmatrix[4];
	float mBoneWeight[4];
};

class Mesh
{
private:

	bool SetupMesh();
	// Compute Shader 用
	void CreateComputeResource();
	void FillComputeMatrix();
	void FillVertex();

public:

	// バッファ
	ID3D11Buffer* VertexBuffer, * IndexBuffer, * ColorBuffer;

	ID3D11Buffer* mpBuf = nullptr;
	ID3D11Buffer* mpComputeBuf = nullptr;
	ID3D11Buffer* mpVerticesBuf = nullptr;
	ID3D11Buffer* mpResultbuffer = nullptr;
	// SRV
	ID3D11ShaderResourceView* mpBufSRV = nullptr;
	ID3D11ShaderResourceView* mpBoneBufSRV = nullptr;
	ID3D11ShaderResourceView* mpVerticesBufSRV = nullptr;
	// UAV
	ID3D11UnorderedAccessView* mpBufUAV = nullptr;

	// 名前
	std::string Name;
	// テクスチャパス
	std::string TexturePass;
	// 頂点
	VERTEX_3D* Vertices;
	// 頂点数
	unsigned int Vertices_Size;
	// インデックス
	std::vector <UINT> Indices;
	// テクスチャ
	std::vector <Texture> Textures;
	// テクスチャ
	std::vector <Texture> TexturesNormal;
	// 変形頂点
	std::vector <DEFORM_VERTEX> mDeformVertex;
	// Compute Shader 用
	std::vector <COMPUTEMATRIX> mComputeMatrix;
	// 骨
	std::unordered_map <std::string, BONE> mBone;
	// 有効
	bool Enable;
	// カラー
	float col[4] = { 1.0f,1.0f,1.0f,1.0f };
	// マテリアル
	MATERIAL Material;
	// アニメーションあり
	Mesh(std::string name, VERTEX_3D* vertices, unsigned int vertices_size,std::vector<UINT> indices, std::vector <Texture> textures, MATERIAL material, std::vector <DEFORM_VERTEX> deform, std::unordered_map  <std::string, BONE> bone) {

		this->Name = name;
		this->Vertices = vertices;
		this->Indices = indices;
		this->Textures = textures;
		this->Material = material;
		this->mDeformVertex = deform;
		this->mBone = bone;
		this->Vertices_Size = vertices_size;

		this->Enable = true;
		this->SetupMesh();
		this->CreateComputeResource();

		D3DXQuaternionIdentity(&Quaternion);

	};
	// アニメーションなし
	Mesh(std::string name, VERTEX_3D* vertices, unsigned int vertices_size,std::vector<UINT> indices, std::vector <Texture> textures, std::vector <Texture> texturesnormal, MATERIAL material) {

		this->Name = name;
		this->Vertices = vertices;
		this->Indices = indices;
		this->Textures = textures;
		this->Material = material;
		this->TexturesNormal = TexturesNormal;
		this->Vertices_Size = vertices_size;

		this->Enable = true;
		this->SetupMesh();

		D3DXQuaternionIdentity(&Quaternion);
	};

	D3DXQUATERNION Quaternion;

	D3DXVECTOR3 Position = D3DXVECTOR3(0.0, 0.0, 0.0);
	D3DXVECTOR3 Rotation = D3DXVECTOR3(0.0, 0.0, 0.0);
	D3DXVECTOR3 Scale = D3DXVECTOR3(1.0, 1.0, 1.0);

	void Update();

	void Draw();

	void Close();

	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);
};

