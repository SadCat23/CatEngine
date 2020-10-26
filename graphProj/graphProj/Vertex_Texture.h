#pragma once
#include <DirectXMath.h>

struct Vertex_Texture
{
	Vertex_Texture() {}
	Vertex_Texture(float x, float y, float z, float tu, float tv,float nx, float ny, float nz)
		: pos(x, y, z), tex(tu, tv), normals(nx,ny,nz)  {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 tex;
	DirectX::XMFLOAT3 normals;
};

struct Vertex_Texture2D
{
	Vertex_Texture2D() {}
	Vertex_Texture2D (float x, float y, float z, float tu, float tv)
		: pos(x, y, z), tex(tu, tv) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 tex;

};