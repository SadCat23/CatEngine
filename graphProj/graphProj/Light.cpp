#include "Light.h"

bool Light::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture,ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader, ConstantBuffer<CB_PS_light> & cb_ps_lightBuffer)
{
	D3D11_TEXTURE2D_DESC depthDescription = {};
	depthDescription.Width = ShadowMapSize;
	depthDescription.Height = ShadowMapSize;
	depthDescription.ArraySize = 1;
	depthDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	depthDescription.CPUAccessFlags = 0;
	depthDescription.Format = DXGI_FORMAT_R32_TYPELESS;
	depthDescription.MipLevels = 1;
	depthDescription.MiscFlags = 0;
	depthDescription.SampleDesc.Count = 1;
	depthDescription.SampleDesc.Quality = 0;
	depthDescription.Usage = D3D11_USAGE_DEFAULT;	

	
	device->CreateTexture2D(&depthDescription, nullptr, &resource);
	
	D3D11_DEPTH_STENCIL_VIEW_DESC dViewDesc = {};
	dViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	
	device->CreateDepthStencilView(resource, &dViewDesc, &DepthStancilVievwLight);
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	device->CreateShaderResourceView(resource, &srvDesc, &DepthShaderViewLight);


	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.MultisampleEnable = true;
	rastDesc.DepthBias = 1;
	rastDesc.SlopeScaledDepthBias = 1.5f;
	rastDesc.DepthBiasClamp = 4;

	device->CreateRasterizerState(&rastDesc, &rastState);

	if (!model.Initialize("Data\\Objects\\light.fbx", device, deviceContext, texture ,cb_vs_vertexshader))
		return false;
	this->deviceContext = deviceContext;
	this->constBuffer_light = &cb_ps_lightBuffer;
	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->UpdateWorldMatrix();
	return true;
}

void Light::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = this->viewMatrix;
	return;
}

void Light::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

void Light::GenerateViewMatrix()
{

	SimpleMath::Vector3 up;
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	SimpleMath::Vector3 m_position;

	m_position.x = pos.x;
	m_position.y = pos.y;
	m_position.z = pos.z;



	SimpleMath::Vector3 m_lookAt;

	//const XMVECTOR UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//XMVECTOR position = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
	// position = XMVectorSet(-4.94999981f, 1.0f,-2.0, 0.0f);

	viewMatrix = SimpleMath::Matrix::CreateLookAt(m_position, m_lookAt, up);
	// Create the view matrix from the three vectors.
	//this->m_viewMatrix=XMMatrixLookAtLH(position, m_lookAt, UP_VECTOR);

	return;
}

void Light::GenerateProjectionMatrix(float screenDepth, float screenNear)
{


	float fieldOfView, screenAspect;


	// Setup field of view and screen aspect for a square light source.
	fieldOfView = 3.141592654f / 2.0f;
	screenAspect = 1.0f;
	
	// Create the projection matrix for the light.
	this->m_projectionMatrix = SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, screenAspect, screenNear, screenDepth);

	return;


}

void Light::RenderLight()
{
	this->constBuffer_light->data.dynamicLightColor = this->lightColor;
	this->constBuffer_light->data.dynamicStrength = this->lightStrength;
	this->constBuffer_light->data.dynamicLightPosition = this->GetPositionFloat3();
	this->constBuffer_light->data.dynamicAttenuation_a = this->attenuation_a;
	this->constBuffer_light->data.dynamicAttenuation_b = this->attenuation_b;
	this->constBuffer_light->data.dynamicAttenuation_c = this->attenuation_c;
	this->constBuffer_light->ApplyChanges();
	
	this->deviceContext->PSSetConstantBuffers(0, 1, this->constBuffer_light->GetAddressOf());
}


