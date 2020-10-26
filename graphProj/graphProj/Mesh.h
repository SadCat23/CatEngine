#pragma once
#include "Vertex_Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Mesh
{
public:
	Mesh(ID3D11Device * device, ID3D11DeviceContext * context, std::vector<Vertex_Texture> & vertices, std::vector<DWORD> & indices);
	Mesh(const Mesh & mesh);
	void Draw();
	void RenderDepth();
	IndexBuffer indexbuffer;
private:
	VertexBuffer<Vertex_Texture> vertexbuffer;

	ID3D11DeviceContext * context;
};