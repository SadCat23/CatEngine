#include "Camera.h"
#include "SimpleMath.h"
Camera::Camera()
{
	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX & Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX & Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const XMVECTOR & Camera::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3 & Camera::GetPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR & Camera::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3 & Camera::GetRotationFloat3() const
{
	return this->rot;
}

void Camera::SetPosition(const XMVECTOR & pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateViewMatrix();
}

void Camera::SetPosition(const XMFLOAT3 & pos)
{
	this->pos = pos;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR & pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMFLOAT3 & pos)
{
	this->pos.x += pos.x;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR & rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(const XMFLOAT3 & rot)
{
	this->rot = rot;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR & rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMFLOAT3 & rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 lookAtPos)
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

void Camera::UpdateViewMatrix() //Updates view matrix and also updates the movement vectors
{

	SimpleMath::Vector3 defUp;
	// Setup the vector that points upwards.
	defUp.x = 0.0f;
	defUp.y = 1.0f;
	defUp.z = 0.0f;

	SimpleMath::Vector3 defLookAt;
	// Setup where the camera is looking by default.
	defLookAt.x = 0.0f;
	defLookAt.y = 0.0f;
	defLookAt.z = 1.0f;
	/*//Calculate camera rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	//Adjust cam target to be offset by the camera's current position
	camTarget += this->posVector;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
	//Rebuild view matrix
	this->viewMatrix = XMMatrixLookAtLH(this->posVector, camTarget, upDir);
	*/
	SimpleMath::Vector3 position, up, lookAt;
	float yaw, pitch, roll;
	SimpleMath::Matrix rotationMatrix;


	// Setup the position of the camera in the world.
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;


	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rot.x * 0.0174532925f;
	yaw = rot.y * 0.0174532925f;
	roll = rot.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = SimpleMath::Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = SimpleMath::Vector3::Transform(defLookAt, rotationMatrix);
	up = SimpleMath::Vector3::Transform(defUp, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt += position;

	// Finally create the view matrix from the three updated vectors.
	viewMatrix = SimpleMath::Matrix::CreateLookAt(position, lookAt, up);



	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, 0.0f);
	this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

	XMMATRIX vecRotationMatrixNoY = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vec_forward_noY = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrixNoY);
	this->vec_backward_noY = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrixNoY);
	this->vec_left_noY = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrixNoY);
	this->vec_right_noY = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrixNoY);


}
const XMVECTOR & Camera::GetForwardVector()
{
	return this->vec_forward;
}

const XMVECTOR & Camera::GetRightVector()
{
	return this->vec_right;
}

const XMVECTOR & Camera::GetBackwardVector()
{
	return this->vec_backward;
}

const XMVECTOR & Camera::GetLeftVector()
{
	return this->vec_left;
}