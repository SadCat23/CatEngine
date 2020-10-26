#pragma once
#include "GameObject.h"
#include <iostream>





GameObject::GameObject()
{
}

void GameObject::UpdateWorldMatrix()
{
	assert("UpdateMatrix must be overridden." && 0);
}
bool GameObject::Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	assert("Initialize must be overridden." && 0);
	return false;
}
void GameObject::Draw(const XMMATRIX & viewProjectionMatrix)
{
	assert("Draw must be overridden." && 0);
}


const XMVECTOR & GameObject::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3 & GameObject::GetPositionFloat3() const
{
	return this->pos;
}




const XMVECTOR & GameObject::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3 & GameObject::GetRotationRelativeFloat3() const
{
	return this->rotRelative;
	// TODO: вставьте здесь оператор return
}

const XMFLOAT3 & GameObject::GetRotationFloat3() const
{
	return this->rot;
}




void GameObject::SetPosition(const XMVECTOR & pos)
{
	
	
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;

	this->UpdateWorldMatrix();
}

void GameObject::SetPosition(const XMFLOAT3 & pos)
{
	this->pos = pos;
	this->posVector = XMLoadFloat3(&this->pos);

	this->UpdateWorldMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);

	this->UpdateWorldMatrix();
}




void GameObject::AdjustPosition(const XMVECTOR & pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);

	this->UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMFLOAT3 & pos)
{
	this->pos.x += pos.y;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->posVector = XMLoadFloat3(&this->pos);

	this->UpdateWorldMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);

	this->UpdateWorldMatrix();
}


void GameObject::SetRotation(const XMVECTOR & rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMFLOAT3 & rot)
{
	this->rot = rot;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}



void GameObject::AdjustRotation(const XMVECTOR & rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMFLOAT3 & rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}




void GameObject::AdjustRelativeRotation(const XMVECTOR & rotRelative)
{
	this->rotRelativeVector += rotRelative;
	XMStoreFloat3(&this->rotRelative, this->rotRelativeVector);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRelativeRotation(float x, float y, float z)
{
	this->rotRelative.x += x;
	this->rotRelative.y += y;
	this->rotRelative.z += z;
	this->rotRelativeVector = XMLoadFloat3(&this->rotRelative);
	this->UpdateWorldMatrix();
}



void GameObject::SetColor(const XMVECTOR & color)
{

	XMStoreFloat3(&this->color, color);
}

void GameObject::SetColor(const XMFLOAT3 & color)
{
	this->color = color;
}

void GameObject::SetColor(float r, float g, float b)
{
	this->color.x = r;
	this->color.y = g;
	this->color.z = b;
}



void GameObject::SetScale(float scaleNew)
{
	this->scale.x = scaleNew;
	this->scale.y = scaleNew;
	this->scale.z = scaleNew;
	this->UpdateWorldMatrix(); 
}

void GameObject::SetScale(float scaleNewX, float scaleNewY, float scaleNewZ=1.0f)
{
	scale.x = scaleNewX;
	scale.y = scaleNewY;
	scale.z = scaleNewZ;
	UpdateWorldMatrix();
}






