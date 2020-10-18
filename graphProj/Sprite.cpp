#include "Sprite.h"
#include <WICTextureLoader.h>

bool Sprite::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, float width, float height, std::string spritePath, ConstantBuffer<CB_VS_vertexshader_2d> & cb_vs_vertexshader_2d, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	this->deviceContext = deviceContext;
	if (deviceContext == nullptr)
		return false;

	this->cb_vs_vertexshader_2d = &cb_vs_vertexshader_2d;
	
	
	//textureResource = texture.Get();
	
	texture_class = std::make_unique<Texture>(device, spritePath, aiTextureType::aiTextureType_DIFFUSE);
	
	std::vector<Vertex_Texture2D> vertexData =
	{
		Vertex_Texture2D(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f), //TopLeft
		Vertex_Texture2D(0.5f, -0.5f, 0.0f, 1.0f, 0.0f), //TopRight
		Vertex_Texture2D(-0.5, 0.5, 0.0f, 0.0f, 1.0f), //Bottom Left
		Vertex_Texture2D(0.5, 0.5, 0.0f, 1.0f, 1.0f), //Bottom Right
	};

	std::vector<DWORD> indexData =
	{
		2, 1, 0,
		3, 1, 2
	};

	HRESULT hr = vertices.Initialize(device, vertexData.data(), vertexData.size());

	hr = indices.Initialize(device, indexData.data(), indexData.size());

	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);

	SetScale(width, height,1);

	return true;
}

void Sprite::Draw(XMMATRIX orthoMatrix)
{
	XMMATRIX wvpMatrix = worldMatrix * orthoMatrix;
	deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertexshader_2d->GetAddressOf());
	cb_vs_vertexshader_2d->data.wvpMatrix = wvpMatrix;
	cb_vs_vertexshader_2d->ApplyChanges();

	deviceContext->PSSetShaderResources(0, 1, texture_class->GetTextureResourceViewAddress());

	const UINT offsets = 0;
	deviceContext->IASetVertexBuffers(0, 1, vertices.GetAddressOf(), vertices.StridePtr(), &offsets);
	deviceContext->IASetIndexBuffer(indices.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(indices.IndexCount(), 0, 0);
}

void Sprite::DrawTexture(XMMATRIX orthoMatrix, ID3D11ShaderResourceView* NewTexture)
{
	this->textureResource = NewTexture;
	Draw(orthoMatrix);
}

float Sprite::GetWidth()
{
	return scale.x;
}

float Sprite::GetHeight()
{
	return scale.y;
}

void Sprite::UpdateWorldMatrix()
{
	worldMatrix = XMMatrixScaling(scale.x, scale.y, 1.0f) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(pos.x + scale.x / 2.0f, pos.y + scale.y / 2.0f, pos.z);
}