#include "main.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Debug.h"
#include "Shader.h"
#include "Application.h"

bool Mesh::SetupMesh() {

	HRESULT hr;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = UINT(sizeof(VERTEX_3D) * Vertices_Size);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &Vertices[0];

	hr = Renderer::GetDevice()->CreateBuffer(&vbd, &initData, &VertexBuffer);
	if (FAILED(hr))
		return false;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(UINT) * UINT(Indices.size());
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;

	initData.pSysMem = &Indices[0];

	hr = Renderer::GetDevice()->CreateBuffer(&ibd, &initData, &IndexBuffer);
	if (FAILED(hr))
		return false;

	D3D11_BUFFER_DESC cbd;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(col);
	cbd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;

	initData.pSysMem = &col[0];

	hr = Renderer::GetDevice()->CreateBuffer(&cbd, &initData, &ColorBuffer);
	if (FAILED(hr))
		return false;

	return true;
}

void Mesh::CreateComputeResource() {

	HRESULT hr;

	// Buffer
	hr = Renderer::CreateStructuredBuffer_DYN(sizeof(COMPUTEMATRIX), (UINT)Vertices_Size, nullptr, &mpComputeBuf);
	assert(SUCCEEDED(hr));
	hr = Renderer::CreateStructuredBuffer_DYN(sizeof(VERTEX_3D), (UINT)Vertices_Size, nullptr, &mpVerticesBuf);
	assert(SUCCEEDED(hr));

	hr = Renderer::CreateBufferSRV(mpComputeBuf, &mpBoneBufSRV);
	assert(SUCCEEDED(hr));
	hr = Renderer::CreateBufferSRV(mpVerticesBuf, &mpVerticesBufSRV);
	assert(SUCCEEDED(hr));

	hr = Renderer::CreateStructuredBuffer(sizeof(VERTEX_3D), (UINT)Vertices_Size, nullptr, &mpResultbuffer);
	assert(SUCCEEDED(hr));

	hr = Renderer::CreateBufferUAV(mpResultbuffer, &mpBufUAV);
	assert(SUCCEEDED(hr));

}

void Mesh::Update() {

	// TODO:Å“K‰»
	FillComputeMatrix();
	FillVertex();

	// Compute
	ID3D11ShaderResourceView* pSRVs[2] = { mpBoneBufSRV, mpVerticesBufSRV };
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 2, pSRVs);
	Renderer::GetDeviceContext()->CSSetShader(Shader::GetComputeShaderArray()[0], nullptr, 0);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &mpBufUAV, 0);
	Renderer::GetDeviceContext()->Dispatch(256, 1, 1);

	// CopyResult
	D3D11_MAPPED_SUBRESOURCE subRes;
	ID3D11Buffer* pBufResult = Renderer::CreateAndCopyToBuffer(mpResultbuffer);

	// GetResult
	Renderer::GetDeviceContext()->Map(pBufResult, 0, D3D11_MAP_READ, 0, &subRes);
	VERTEX_3D* pBufType = (VERTEX_3D*)subRes.pData;

	// ToVertexBuffer
	D3D11_MAPPED_SUBRESOURCE ms;
	Renderer::GetDeviceContext()->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	VERTEX_3D* vertex = (VERTEX_3D*)ms.pData;
	memcpy(vertex, pBufType, sizeof(VERTEX_3D) * Vertices_Size);

	Renderer::GetDeviceContext()->Unmap(VertexBuffer, 0);
	Renderer::GetDeviceContext()->Unmap(pBufResult, 0);

	pBufResult->Release();


	
}

void Mesh::Draw(){

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	Renderer::GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	if (!Renderer::mLineMode) {
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	else {
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}

	Renderer::SetMaterial(Material);

	Renderer::GetDeviceContext()->DrawIndexed((UINT)Indices.size(), 0, 0);
}

void Mesh::Close() {

	VertexBuffer->Release();
	IndexBuffer->Release();
	ColorBuffer->Release();

	if (mpBuf) {
		mpBuf->Release();
	}
	if (mpComputeBuf) {
		mpComputeBuf->Release();
	}
	if (mpVerticesBuf) {
		mpVerticesBuf->Release();
	}

	if (mpBufSRV) {
		mpBufSRV->Release();
	}
	if (mpBoneBufSRV) {
		mpBoneBufSRV->Release();
	}
	if (mpVerticesBufSRV) {
		mpVerticesBufSRV->Release();
	}

	delete Vertices;
	Vertices = nullptr;

	Indices.clear();
	Indices.shrink_to_fit();

	Textures.clear();
	Textures.shrink_to_fit();

	TexturesNormal.clear();
	TexturesNormal.shrink_to_fit();

	mDeformVertex.clear();
	mDeformVertex.shrink_to_fit();

	mBone.clear();

}

void Mesh::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix) {

	BONE* bone = &mBone[node->mName.C_Str()];

	aiMatrix4x4 world;
	world = matrix;
	world *= bone->mAnimationMatrix;

	bone->mMatrix = world;
	bone->mMatrix *= bone->mOffsetMatirx;

	for (unsigned int n = 0; n < node->mNumChildren; n++) {
		UpdateBoneMatrix(node->mChildren[n], world);
	}

}

void Mesh::FillComputeMatrix() {

	// TODO:Å“K‰»
	D3D11_MAPPED_SUBRESOURCE subRes_1;
	Renderer::GetDeviceContext()->Map(mpComputeBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes_1);
	COMPUTEMATRIX* pBufType_cm = (COMPUTEMATRIX*)subRes_1.pData;
	for (unsigned int v = 0; v < Vertices_Size; v++) {
		DEFORM_VERTEX* deform = &mDeformVertex[v];
		std::string* str = deform->mBoneName;

		for (int i = 0; i < 4; i++) {
			if (str[i] == "") {
				continue;
			}
			pBufType_cm[v].cmatrix[i] = mBone[str[i]].mMatrix;
		
		}

		pBufType_cm[v].mBoneWeight[0] = deform->mBoneWeight[0];
		pBufType_cm[v].mBoneWeight[1] = deform->mBoneWeight[1];
		pBufType_cm[v].mBoneWeight[2] = deform->mBoneWeight[2];
		pBufType_cm[v].mBoneWeight[3] = deform->mBoneWeight[3];
	}
	Renderer::GetDeviceContext()->Unmap(mpComputeBuf, 0);
}

void Mesh::FillVertex() {

	// TODO:Å“K‰»
	D3D11_MAPPED_SUBRESOURCE subRes_2;
	Renderer::GetDeviceContext()->Map(mpVerticesBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes_2);
	VERTEX_3D* pBufType_vtx = (VERTEX_3D*)subRes_2.pData;
	for (unsigned int v = 0; v < Vertices_Size; v++) {
		pBufType_vtx[v] = Vertices[v];
	}
	Renderer::GetDeviceContext()->Unmap(mpVerticesBuf, 0);
}