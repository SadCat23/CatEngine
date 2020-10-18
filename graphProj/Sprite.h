#pragma once
#include "GameObject2D.h"
#include "ConstantBuffer.h"
#include <string>
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Vertex_Texture.h"
#include "Texture.h"

class Sprite : public GameObject2D
{
public:
	bool Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, float width, float height, std::string spritePath, ConstantBuffer<CB_VS_vertexshader_2d> & cb_vs_vertexshader_2d, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);
	void Draw(XMMATRIX orthoMatrix); //2d camera orthogonal matrix
	void DrawTexture(XMMATRIX orthoMatrix, ID3D11ShaderResourceView* NewTexture); //2d camera orthogonal matrix
	float GetWidth();
	float GetHeight();
private:
	void UpdateWorldMatrix() override;

	ConstantBuffer<CB_VS_vertexshader_2d> * cb_vs_vertexshader_2d = nullptr;
	XMMATRIX worldMatrix = XMMatrixIdentity();

	ID3D11ShaderResourceView* textureResource;
	
	ID3D11DeviceContext * deviceContext = nullptr;

	IndexBuffer indices;
	VertexBuffer<Vertex_Texture2D> vertices;


	std::unique_ptr<Texture> texture_class;

};