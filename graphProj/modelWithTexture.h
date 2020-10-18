#pragma once

#include "Mesh.h"

using namespace DirectX;

class modelWithTexture
{
public:
	XMFLOAT3 centerbound;
	float radiusBound;


	bool Initialize(const std::string & filePath,ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	void Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewProjectionMatrix);
	void RenderDepth();
	void SetTexture(ID3D11ShaderResourceView * texture);
	ID3D11ShaderResourceView* GetTexture();
	void CalcBound(std::vector<Vertex_Texture>& vertex);
	std::vector<Mesh> meshes;

protected:
	
	bool LoadModel(const std::string & filePath);
	void ProcessNode(aiNode * node, const aiScene * scene);
	Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);
	

	ID3D11Device * device = nullptr;
	ID3D11DeviceContext * deviceContext = nullptr;
	ConstantBuffer<CB_VS_vertexshader> * cb_vs_vertexshader = nullptr;
	ID3D11ShaderResourceView * texture = nullptr;











};