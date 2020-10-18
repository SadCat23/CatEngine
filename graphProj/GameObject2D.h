#pragma once
#include "GameObject.h"

class GameObject2D : public GameObject
{
public:

protected:
	virtual void UpdateWorldMatrix();
	bool Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader) override;
	void Draw(const XMMATRIX & viewProjectionMatrix) override;
};