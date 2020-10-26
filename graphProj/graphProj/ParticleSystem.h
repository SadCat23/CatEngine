#pragma once
///////////////////////////////////////////////////////////////////////////////
// Filename: particlesystemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PARTICLESYSTEMCLASS_H_
#define _PARTICLESYSTEMCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include "SimpleMath.h"


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Texture.h"


using namespace DirectX;
////////////////////////////////////////////////////////////////////////////////
// Class name: ParticleSystemClass
////////////////////////////////////////////////////////////////////////////////
class ParticleSystem
{
private:
	struct ParticleType
	{
		float positionX, positionY, positionZ;
		float red, green, blue;
		float velocity;
		bool active;
	};
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT4 color;
	};
public:
	ParticleSystem();
	ParticleSystem(const ParticleSystem&);
	~ParticleSystem();

	bool Initialize(ID3D11Device* device, Texture* texture, ConstantBuffer<CB_VS_vertexshader_Particle>& _cb_vs_vertexshader, ConstantBuffer<CB_VS_pixelshader_Particle>& _cb_vs_pixelshader);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*);
	void Draw(const XMMATRIX& viewProjectionMatrix);
	
		
		ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();
	ConstantBuffer<CB_VS_vertexshader_Particle>* cb_vs_vertexshader= nullptr;
	ConstantBuffer<CB_VS_pixelshader_Particle>* cb_vs_pixelshader = nullptr;

private:
	void SetTexture(Texture* texture);
	void ReleaseTexture();

	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext*);

	void RenderBuffers(ID3D11DeviceContext*);

private:
	float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	float m_particleVelocity, m_particleVelocityVariation;
	float m_particleSize, m_particlesPerSecond;
	int m_maxParticles;

	int m_currentParticleCount;
	float m_accumulatedTime;

	Texture* m_Texture;

	ParticleType* m_particleList;

	int m_vertexCount, m_indexCount;
	VertexType* m_vertices;
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;


};
#endif