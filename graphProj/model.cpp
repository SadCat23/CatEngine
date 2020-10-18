#include "model.h"



bool Model::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader,XMFLOAT3 Color)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->texture = texture;
	this->cb_vs_vertexshader = &cb_vs_vertexshader;
	

		//Textured Square
		Vertex v[] =
		{
			Vertex(-0.5f,  -0.5f, -0.5f, Color.x, Color.y, Color.z), //FRONT Bottom Left   - [0]
			Vertex(-0.5f,   0.5f, -0.5f, Color.x, Color.y, Color.z), //FRONT Top Left      - [1]
			Vertex(0.5f,   0.5f, -0.5f, Color.x, Color.y, Color.z), //FRONT Top Right     - [2]
			Vertex(0.5f,  -0.5f, -0.5f, Color.x, Color.y, Color.z), //FRONT Bottom Right   - [3]
			Vertex(-0.5f,  -0.5f, 0.5f, Color.x, Color.y, Color.z), //BACK Bottom Left   - [4]
			Vertex(-0.5f,   0.5f, 0.5f, Color.x, Color.y, Color.z), //BACK Top Left      - [5]
			Vertex(0.5f,   0.5f, 0.5f, Color.x, Color.y, Color.z), //BACK Top Right     - [6]
			Vertex(0.5f,  -0.5f, 0.5f, Color.x, Color.y, Color.z), //BACK Bottom Right   - [7]
		};

		//Load Vertex Data
		HRESULT hr = this->vertexBuffer.Initialize(this->device, v, ARRAYSIZE(v));
		

		DWORD indices[] =
		{
			0, 1, 2, //FRONT
			0, 2, 3, //FRONT
			4, 7, 6, //BACK 
			4, 6, 5, //BACK
			3, 2, 6, //RIGHT SIDE
			3, 6, 7, //RIGHT SIDE
			4, 5, 1, //LEFT SIDE
			4, 1, 0, //LEFT SIDE
			1, 5, 6, //TOP
			1, 6, 2, //TOP
			0, 3, 7, //BOTTOM
			0, 7, 4, //BOTTOM
		};

		//Load Index Data
		hr = this->indexBuffer.Initialize(this->device, indices, ARRAYSIZE(indices));
		
	

	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->UpdateWorldMatrix();
	return true;
}

void Model::Draw(const XMMATRIX & viewProjectionMatrix)
{
	
	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());

	this->deviceContext->PSSetShaderResources(0, 1, &this->texture); //Set Texture
	this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	UINT offset = 0;
	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
	this->deviceContext->DrawIndexed(this->indexBuffer.IndexCount(), 0, 0); //Draw
}

void Model::UpdateWorldMatrix()
{
	
 
	if (this->parent != nullptr)
	{
		XMMATRIX scaleMatrix = XMMatrixScaling(this->scale, this->scale, this->scale);
		this->worldMatrix = scaleMatrix;
		this->worldMatrix *= XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
		this->worldMatrix *= XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
		this->worldMatrix *= XMMatrixRotationRollPitchYaw(this->GetRotationRelativeFloat3().x, this->GetRotationRelativeFloat3().y, this->GetRotationRelativeFloat3().z);
		this->worldMatrix *= XMMatrixTranslation(parent->GetPositionFloat3().x, parent->GetPositionFloat3().y, parent->GetPositionFloat3().z);
		this->worldMatrix *= XMMatrixRotationRollPitchYaw(parent->GetRotationRelativeFloat3().x, parent->GetRotationRelativeFloat3().y, parent->GetRotationRelativeFloat3().z);
		if (parent->parent != nullptr)
		{
			this->worldMatrix *= XMMatrixTranslation(parent->parent->GetPositionFloat3().x, parent->parent->GetPositionFloat3().y, parent->parent->GetPositionFloat3().z);
			this->worldMatrix *= XMMatrixRotationRollPitchYaw(parent->parent->GetRotationRelativeFloat3().x, parent->parent->GetRotationRelativeFloat3().y, parent->parent->GetRotationRelativeFloat3().z);
		}
		
	}
	else
	{
		
		this->worldMatrix = XMMatrixScaling(this->scale, this->scale, this->scale);
		this->worldMatrix *= XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
		this->worldMatrix *= XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
		this->worldMatrix *= XMMatrixRotationRollPitchYaw(this->GetRotationRelativeFloat3().x, this->GetRotationRelativeFloat3().y, this->GetRotationRelativeFloat3().z);
		
	}
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

}
const XMVECTOR & Model::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3 & Model::GetPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR & Model::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3 & Model::GetRotationRelativeFloat3() const
{
	return this->rotRelative;
	// TODO: вставьте здесь оператор return
}

const XMFLOAT3 & Model::GetRotationFloat3() const
{
	return this->rot;
}

void Model::SetPosition(const XMVECTOR & pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateWorldMatrix();
}

void Model::SetPosition(const XMFLOAT3 & pos)
{
	this->pos = pos;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Model::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Model::AdjustPosition(const XMVECTOR & pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateWorldMatrix();
}






void Model::AdjustPosition(const XMFLOAT3 & pos)
{
	this->pos.x += pos.y;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Model::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Model::SetRotation(const XMVECTOR & rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	this->UpdateWorldMatrix();
}

void Model::SetRotation(const XMFLOAT3 & rot)
{
	this->rot = rot;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void Model::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void Model::AdjustRotation(const XMVECTOR & rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateWorldMatrix();
}


void Model::AdjustRotation(const XMFLOAT3 & rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void Model::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}




void Model::AdjustRelativeRotation(const XMVECTOR & rotRelative)
{
	this->rotRelativeVector += rotRelative;
	XMStoreFloat3(&this->rotRelative, this->rotRelativeVector);
	this->UpdateWorldMatrix();
}

void Model::AdjustRelativeRotation(float x, float y, float z)
{
	this->rotRelative.x += x;
	this->rotRelative.y += y;
	this->rotRelative.z += z;
	this->rotRelativeVector = XMLoadFloat3(&this->rotRelative);
	this->UpdateWorldMatrix();
}

void Model::SetColor(const XMVECTOR & color)
{
	
	XMStoreFloat3(&this->color, color);
}

void Model::SetColor(const XMFLOAT3 & color)
{
	this->color = color;
}

void Model::SetColor(float r, float g, float b)
{
	this->color.x = r;
	this->color.y = g;
	this->color.z = b;
}

void Model::SetScale(float scale)
{
	this->scale = scale;
	this->UpdateWorldMatrix();
}

void Model::SetParent(Model* parent)
{
	this->parent = parent;
}


void Model::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	
	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
		return;

	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

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

const XMVECTOR & Model::GetForwardVector()
{
	return this->vec_forward;
}

const XMVECTOR & Model::GetRightVector()
{
	return this->vec_right;
}

const XMVECTOR & Model::GetBackwardVector()
{
	return this->vec_backward;
}

const XMVECTOR & Model::GetLeftVector()
{
	return this->vec_left;
}