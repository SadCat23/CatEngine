#pragma once
#include <DirectXMath.h>

struct CB_VS_vertexshader
{
	DirectX::XMMATRIX wvpMatrix;
	DirectX::XMMATRIX worldMatrix;
}; 

struct CB_VS_vertexshader_2d
{
	DirectX::XMMATRIX wvpMatrix;

};


struct CB_PS_light
{
	DirectX::XMFLOAT3 ambientLoghtColor;
	float ambientStrength;
	
	DirectX::XMFLOAT3 dynamicLightColor;
	float dynamicStrength;
	DirectX::XMFLOAT3 dynamicLightPosition;
	float dynamicAttenuation_a;
	float dynamicAttenuation_b;
	float dynamicAttenuation_c;
};

struct MattrixBufferDepth
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct MattrixBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX lightView;
	DirectX::XMMATRIX lightProjection;
}; 

struct LightBufferType
{
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 diffuseColor;
	float exposure;
	float gamma;
	DirectX::XMFLOAT3 paddind;
};

struct LightBufferType2
{
	DirectX::XMFLOAT3 lightPosition;
	float padding;
};

struct CB_VS_vertexshader_Particle
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
}; 

struct CB_VS_pixelshader_Particle
{
	DirectX::XMMATRIX wvpMatrix;

};