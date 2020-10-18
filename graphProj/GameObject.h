#pragma once
#include "modelWithTexture.h"
enum typeObject { actor, light,base};

class GameObject
{
	public:
		XMVECTOR posVector;
		XMVECTOR rotVector;
		XMVECTOR rotRelativeVector;
		XMFLOAT3 pos;
		XMFLOAT3 rot;
		XMFLOAT3 rotRelative;


	virtual  typeObject GetType()
	{
		return base;
	}
	GameObject();

	virtual bool Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	virtual void Draw(const XMMATRIX & viewProjectionMatrix);
	virtual void UpdateWorldMatrix();


	XMFLOAT3 scale = XMFLOAT3(1,1,1);

	const XMVECTOR & GetPositionVector() const;
	const XMFLOAT3 & GetPositionFloat3() const;
	const XMVECTOR & GetRotationVector() const;
	const XMFLOAT3 & GetRotationRelativeFloat3() const;
	const XMFLOAT3 & GetRotationFloat3() const;

	void SetPosition(const XMVECTOR & pos);
	void SetPosition(const XMFLOAT3 & pos);
	void SetPosition(float x, float y, float z);

	void AdjustPosition(const XMVECTOR & pos);
	void AdjustPosition(const XMFLOAT3 & pos);
	void AdjustPosition(float x, float y, float z);

	void SetRotation(const XMVECTOR & rot);
	void SetRotation(const XMFLOAT3 & rot);
	void SetRotation(float x, float y, float z);

	void AdjustRotation(const XMVECTOR & rot);
	void AdjustRotation(const XMFLOAT3 & rot);
	void AdjustRotation(float x, float y, float z);

	void AdjustRelativeRotation(const XMVECTOR & rotRelative);
	void AdjustRelativeRotation(const XMFLOAT3 & rot);
	void AdjustRelativeRotation(float x, float y, float z);

	void SetColor(const XMVECTOR & color);
	void SetColor(const XMFLOAT3 & color);
	void SetColor(float r, float g, float b);
	void SetScale(float scale);
	void SetScale(float scaleX,float scaleY, float scaleZ);
	

	modelWithTexture model;

private:
	
	
	XMFLOAT3 color;
	


};