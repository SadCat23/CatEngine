#include "modelWithTexture.h"
#include <iostream>


bool modelWithTexture::Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->texture = texture;
	this->cb_vs_vertexshader = &cb_vs_vertexshader;


	if (!this->LoadModel(filePath))
		return false;


	return true;
}

void modelWithTexture::SetTexture(ID3D11ShaderResourceView * texture)
{
	this->texture = texture;
}

ID3D11ShaderResourceView* modelWithTexture::GetTexture()
{
	return texture;
}

void modelWithTexture::CalcBound(std::vector<Vertex_Texture>& vertex)
{
	float radiusBound;

	XMFLOAT3 centerBound = XMFLOAT3(0.0f, 0.0f, 0.0f);
	int count = vertex.size();

	for (int i = 0; i < count; i++)
	{
		centerBound.x += vertex[i].pos.x;
		centerBound.y += vertex[i].pos.y;
		centerBound.z += vertex[i].pos.z;
	}

	centerBound.x /= (float)count;
	centerBound.y /= (float)count;
	centerBound.z /= (float)count;


	// find farthest point in set

	radiusBound = 0.0f;

	for (int i = 0; i < count; i++)
	{

		float dist;

		dist = sqrtf(powf(centerBound.x - vertex[i].pos.x, 2) + powf(centerBound.y - vertex[i].pos.y, 2) + powf(centerBound.z - vertex[i].pos.z, 2));

		if (dist > radiusBound)

			radiusBound = dist;
	}

	radiusBound = sqrtf(radiusBound);
	
	this->radiusBound = radiusBound;
	this->centerbound = centerBound;
	std::cout << radiusBound<<"\n";
	std::cout << centerBound.x<<"\n";;
	std::cout << centerBound.y << "\n";;
	std::cout << centerBound.z << "\n";;
	std::cout << "++++++++++++++++++++\n";;
}


void modelWithTexture::Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewProjectionMatrix)
{
	//Update Constant buffer with WVP Matrix
	//this->cb_vs_vertexshader->data.wvpMatrix = worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
	//this->cb_vs_vertexshader->data.worldMatrix = worldMatrix; //Calculate World Matrix
	//this->cb_vs_vertexshader->ApplyChanges();
	//this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());

	//this->deviceContext->PSSetShaderResources(0, 1, &this->texture); //Set Texture
	
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw();
	}
}

void modelWithTexture::RenderDepth()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].RenderDepth();
	}
}

bool modelWithTexture::LoadModel(const std::string & filePath)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
		return false;

	this->ProcessNode(pScene->mRootNode, pScene);
	return true;
}

void modelWithTexture::ProcessNode(aiNode * node, const aiScene * scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

Mesh modelWithTexture::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	// Data to fill
	std::vector<Vertex_Texture> vertices;
	std::vector<DWORD> indices;

	//Get vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex_Texture vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		vertex.normals.x = mesh->mNormals[i].x;
		vertex.normals.y = mesh->mNormals[i].y;
		vertex.normals.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.tex.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.tex.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	//Get indices
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	CalcBound(vertices);
	return Mesh(this->device, this->deviceContext, vertices, indices);
}

