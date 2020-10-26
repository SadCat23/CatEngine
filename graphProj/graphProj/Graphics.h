#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Camera.h"
#include "model.h"
#include "SphereKatamaria.h"
#include "ObjectKatamaria.h"
#include "KatamariaScene.h"
#include "Timer.h"
#include <vector>
#include <WICTextureLoader.h>
#include "Light.h"
#include "Sprite.h"
#include "Camera2D.h"
#include "GameObject3D.h"
#include "RenderTexture.h"

class Graphics
{
public: 


	float exposure = 10.0f;
	float gamma = 2.0f;

	Sprite sprite;
	bool Init(HWND hwnd, int width, int height);
	void RenderFrame();
	void RenderTexture();
	void RenderScene();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	XMMATRIX  GetProjectionMatrix();
	XMMATRIX* GetWorldMatrix();
	XMMATRIX* GetOrthoMatrix();

	void EnableDepthShader();
	void EnableShadowShader();
	void EnableNormalShader();



	void InitShadowParametrs();
	Camera myCamera;
	Camera2D myCamera2D;
	std::vector <Light*>  light;
	std::vector <Model*>  models;
	std::vector <GameObject3D*>  GameObjects3D;
	Model* AddModel(XMFLOAT3 pos, Model* parent, XMFLOAT3 color);
	GameObject3D* AddGameObject3D(XMFLOAT3 pos, GameObject3D* parent, const std::string & filePath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);
	GameObject3D* AddLight3D(XMFLOAT3 pos, GameObject3D* parent);
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	
	RenderTextureClass* m_RenderClass;
	
	
	ID3D11SamplerState* m_sampleStateWrap;
	ID3D11SamplerState* m_sampleStateClamp;
	
	bool SetShaderDepthParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

	void SetShadowParametrs(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix,
		ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMapTexture, XMFLOAT3 lightPosition,
		XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, float exposure,float gamma);
	

	ConstantBuffer<MattrixBufferDepth> m_matrixBufferDetph;

	ConstantBuffer<MattrixBuffer> m_matrixBuffer;
	ConstantBuffer<LightBufferType> m_lightBuffer;
	ConstantBuffer<LightBufferType2> m_lightBuffer2;

	ConstantBuffer <CB_VS_pixelshader_Particle> m_particlePixelBuffer;
	ConstantBuffer <CB_VS_vertexshader_Particle> m_particleVertexBuffer;


	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> StarDeath;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Ogon;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Mettal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Grass;
	
	
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView()
	{
		return depthStencilView;
	}

	void SetBackBufferRenderTarget()
	{
		// Bind the render target view and depth stencil buffer to the output render pipeline.
		context->OMSetRenderTargets(1, rtv.GetAddressOf(), depthStencilView.Get());
		return;
	}

private:
	bool InitDirecrX(HWND hwnd);
	bool InitShaders();
	bool InitScene();

	void TurnZBufferOn();

	void TurnZBufferOff();
	
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr <IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> rtv;
	Microsoft::WRL::ComPtr <ID3D11Device> device;


	ConstantBuffer<CB_VS_vertexshader_2d> constBuffer_vertex2d;
	ConstantBuffer<CB_VS_vertexshader> constBuffer_vertex;
	ConstantBuffer<CB_PS_light> constBuffer_light;
	
	ConstantBuffer<CB_PS_light> constBuffer_light;

	
	VertexShader vertexSh;
	PixelShader pixelSh;
	
	PixelShader pixelSh_2d;
	VertexShader vertexSh_2d;

	VertexShader vertexShaderShadow;
	PixelShader pixelShaderShadow;

	PixelShader pixelSh_depth;
	VertexShader vertexSh_depth;

	PixelShader pixelSh_Particle;
	VertexShader vertexSh_Particle;



	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>  depthDisabledStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;


	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;

	int windowWidth = 0;
	int windowHeight = 0;


	Timer fpsTimer;



};