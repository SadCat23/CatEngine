#pragma once
#include "GameObject3D.h"

GameObject3D::GameObject3D()
{
	this->oldRotate = XMMatrixIdentity();
}

bool GameObject3D::Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	if (!model.Initialize(filePath, device, deviceContext, texture, cb_vs_vertexshader))
		return false;

	this->centerBound = this->model.centerbound;
	this->radiusBound = this->model.radiusBound;
	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	UpdateWorldMatrix();
	return true;
}

void GameObject3D::SetParent(GameObject3D* parent)
{
	this->parent = parent;
}

void GameObject3D::Draw(const XMMATRIX & viewProjectionMatrix)
{
	model.Draw(this->worldMatrix, viewProjectionMatrix);
}

void GameObject3D::RenderDepth()
{
	model.RenderDepth();
}


void GameObject3D::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtPos.x == pos.x && lookAtPos.y == pos.y && lookAtPos.z == pos.z)
		return;

	lookAtPos.x = pos.x - lookAtPos.x;
	lookAtPos.y = pos.y - lookAtPos.y;
	lookAtPos.z = pos.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += XM_PI;

	this->SetRotation(pitch, yaw, 0.0f);
}

const XMVECTOR & GameObject3D::GetForwardVector(bool omitY)
{
	if (omitY)
		return vec_forward_noY;
	else
		return vec_forward;
}

const XMVECTOR & GameObject3D::GetRightVector(bool omitY)
{
	if (omitY)
		return vec_right_noY;
	else
		return vec_right;
}

const XMVECTOR & GameObject3D::GetBackwardVector(bool omitY)
{
	if (omitY)
		return vec_backward_noY;
	else
		return vec_backward;
}

const XMVECTOR & GameObject3D::GetLeftVector(bool omitY)
{
	if (omitY)
		return vec_left_noY;
	else
		return vec_left;
}

void GameObject3D::UpdateWorldMatrix()
{
	if (this->parent != nullptr)
	{

		float deltaX = parent->GetRotationFloat3().x - oldX;
		float deltaZ = parent->GetRotationFloat3().z - oldZ;

		
		oldRotate *= XMMatrixRotationX(deltaX);
		oldRotate *= XMMatrixRotationZ(-deltaZ);

		XMMATRIX scaleMatrix = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
		this->worldMatrix = scaleMatrix;
		this->worldMatrix *= XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
		this->worldMatrix *= XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
		//this->worldMatrix *= XMMatrixRotationRollPitchYaw(this->GetRotationRelativeFloat3().x, this->GetRotationRelativeFloat3().y, this->GetRotationRelativeFloat3().z);
		//this->worldMatrix *= XMMatrixRotationRollPitchYaw(parent->GetRotationFloat3().x, parent->GetRotationFloat3().y, parent->GetRotationFloat3().z);
		this->worldMatrix *= oldRotate;
		this->worldMatrix *= XMMatrixTranslation(parent->GetPositionFloat3().x, parent->GetPositionFloat3().y, parent->GetPositionFloat3().z);

		if (parent->parent != nullptr)
		{
			this->worldMatrix *= XMMatrixTranslation(parent->parent->GetPositionFloat3().x, parent->parent->GetPositionFloat3().y, parent->parent->GetPositionFloat3().z);
			this->worldMatrix *= XMMatrixRotationRollPitchYaw(parent->parent->GetRotationRelativeFloat3().x, parent->parent->GetRotationRelativeFloat3().y, parent->parent->GetRotationRelativeFloat3().z);
		}


		oldX = parent->GetRotationFloat3().x;
		oldZ = parent->GetRotationFloat3().z;



	}
	else
	{


		this->worldMatrix = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);

		//this->worldMatrix *= XMMatrixRotationRollPitchYaw(0, 0, 0);

		


		float deltaX = this->rot.x - oldX;
		float deltaZ = this->rot.z - oldZ;



		oldRotate *= XMMatrixRotationX(deltaX);
		oldRotate *= XMMatrixRotationZ(-deltaZ);
			

		this->worldMatrix *= oldRotate;

		this->worldMatrix *= XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
		//this->worldMatrix *= XMMatrixRotationRollPitchYaw(this->GetRotationRelativeFloat3().x, this->GetRotationRelativeFloat3().y, this->GetRotationRelativeFloat3().z);



		 oldX = this->rot.x;
		 oldZ = this->rot.z;
	}
	CalcCenterBound();
	UpdateDirectionVectors();
}

void GameObject3D::UpdateDirectionVectors()
{
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);
	vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

	XMMATRIX vecRotationMatrixNoY = XMMatrixRotationRollPitchYaw(0.0f, rot.y, 0.0f);
	vec_forward_noY = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrixNoY);
	vec_backward_noY = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrixNoY);
	vec_left_noY = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrixNoY);
	vec_right_noY = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrixNoY);
}

void GameObject3D::CalcCenterBound()
{
	FXMVECTOR vector = XMLoadFloat3(&this->centerBound);
	XMVECTOR NewCenter = XMVector3Transform(vector, worldMatrix);
	XMStoreFloat3(&this->centerBound, NewCenter);
}

