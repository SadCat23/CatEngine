#pragma once
#include "GameObject.h"
class GameObject3D : public GameObject
{
public:
	GameObject3D();
	void SetLookAtPos(XMFLOAT3 lookAtPos);
	const XMVECTOR & GetForwardVector(bool omitY = false);
	const XMVECTOR & GetRightVector(bool omitY = false);
	const XMVECTOR & GetBackwardVector(bool omitY = false);
	const XMVECTOR & GetLeftVector(bool omitY = false);
	void SetParent(GameObject3D* parent);
	void CalcCenterBound();
	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMFLOAT3 centerBound;
	bool isLight;
	float oldX,oldZ;

	float radiusBound;
	XMMATRIX oldRotate;
	bool setOldRotate = false;
	GameObject3D* parent;
	std::vector <GameObject3D*> childs;
	bool Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)  override;
	void UpdateWorldMatrix() override;
	void Draw(const XMMATRIX & viewProjectionMatrix) override;
	void RenderDepth();
	void UpdateDirectionVectors();
protected:
	
	

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_left;
	XMVECTOR vec_right;
	XMVECTOR vec_backward;

	XMVECTOR vec_forward_noY;
	XMVECTOR vec_left_noY;
	XMVECTOR vec_right_noY;
	XMVECTOR vec_backward_noY;
};