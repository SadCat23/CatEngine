#include "GameObject2D.h"

void GameObject2D::UpdateWorldMatrix()
{
	assert("UpdateMatrix must be overridden." && 0);
}

bool GameObject2D::Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	return false;
}

void GameObject2D::Draw(const XMMATRIX & viewProjectionMatrix)
{
}
