#pragma once
#include "Graphics.h"
#include "SimpleMath.h"

const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 0.01f;
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

bool Graphics::Init(HWND hwnd, int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	this->fpsTimer.Start();
	if (!InitDirecrX(hwnd))
		return false;

	if (!InitShaders())
		return false;

	if (!InitScene())
		return false;

	// Setup the projection matrix.
	float fieldOfView = 3.141592654f / 2.0f;
	float screenAspect = (float)windowWidth / (float)windowHeight ;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix= SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);
	
	
	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = XMMatrixIdentity();
	
	
	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix=XMMatrixOrthographicLH((float)windowWidth, (float)windowHeight, SCREEN_NEAR, SCREEN_DEPTH);

	return true;



}

void Graphics::RenderFrame()
{
	EnableNormalShader();
	static int fpsCounter = 0;
	static std::string fpsString = "FPS: 0";
	fpsCounter += 1;
	if (fpsTimer.GetMilisecondsElapsed() > 1000.0)
	{
		fpsString = "FPS: " + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	}
	//TExt

	//spriteBatch->Begin();
	//spriteFont->DrawString(spriteBatch.get(), StringConverter::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(1400, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	//spriteFont->DrawString(spriteBatch.get(), L"By SadCat", DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1, 1));
	//spriteBatch->End();




	light[0]->isLight = true;
	
	//static float lightPositionX = -5.0f;
	//lightPositionX += 0.05f;
	//if (lightPositionX > 15.0f)
	//{
	//	lightPositionX = -15.0f;
	//}
	light[0]->SetPosition(1, 3, 0);



	RenderTexture();
	
	BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	

	RenderScene();


	

	context->IASetInputLayout(vertexSh_2d.GetInputLayout());
	context->PSSetShader(pixelSh_2d.GetShader(), NULL, 0);
	context->VSSetShader(vertexSh_2d.GetShader(), NULL, 0);
	sprite.Draw(myCamera2D.GetWorldMatrix() * myCamera2D.GetOrthoMatrix());

	this->swapChain->Present(1, NULL);
}

void Graphics::RenderTexture()
{
	
	XMMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, translateMatrix;
	// Set the render target to be the render to texture.
	m_RenderClass->SetRenderTarget(context.Get());
	//Clear the render to texture background to blue so we can differentiate it from the rest of the normal scene.
	m_RenderClass->ClearRenderTarget(context.Get(), 1.0f, 1.0f, 1.0f, 1.0f);
	

	light[0]->GenerateViewMatrix();

	worldMatrix=XMMatrixIdentity();
	

	light[0]->GetViewMatrix(lightViewMatrix);

	light[0]->GetProjectionMatrix(lightProjectionMatrix);


	// Clear the render to texture.
	//m_RenderClass->ClearRenderTarget(context.Get(), this->GetDepthStencilView().Get(), 0.0f, 0.0f, 1.0f, 1.0f);
	
	

	
	// Render the scene now and it will draw to the render to texture instead of the back buffer.
		for (int i = 0; i < GameObjects3D.size(); i++)
	{
			if (!GameObjects3D[i]->isLight) {
				SetShaderDepthParameters(context.Get(), GameObjects3D[i]->worldMatrix, lightViewMatrix, lightProjectionMatrix);
				EnableDepthShader();
				GameObjects3D[i]->RenderDepth();
			}
	}
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	this->SetBackBufferRenderTarget();
	
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.Width = this->windowWidth;
	viewport.Height = this->windowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	context->RSSetViewports(1, &viewport);

	return;





}

void Graphics::RenderScene()
{

	

	light[0]->RenderLight();
	//light[1]->RenderLight();
	//this->context->PSSetConstantBuffers(0, 1, this->constBuffer_light.GetAddressOf());


	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	//this->context->ClearRenderTargetView(this->rtv.Get(), bgcolor);
	//this->context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	myCamera.UpdateViewMatrix();

	light[0]->GenerateViewMatrix();


	XMMATRIX viewMatrix = myCamera.GetViewMatrix();
	XMMATRIX projMatrix = GetProjectionMatrix();
	
	XMMATRIX lightViewMatrix;
	light[0]->GetViewMatrix(lightViewMatrix);

	XMMATRIX lightProjectionMatrix;
	light[0]->GetProjectionMatrix(lightProjectionMatrix);


	UINT offset = 0;

	//UpdateConstantBuffer
	/*
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->Draw(myCamera.GetViewMatrix()*myCamera.GetProjectionMatrix());
	}
	*/

	XMFLOAT4 ambientColor;
	ambientColor.x = 0.15f;
	ambientColor.y= 0.15f;
	ambientColor.z= 0.15f;
	ambientColor.w = 1.0f;
	
	XMFLOAT4 diffuseColor;
	diffuseColor.x = 1.0f;
	diffuseColor.y = 1.0f;
	diffuseColor.z = 1.0f;
	diffuseColor.w = 1.0f;

	for (int i = 0; i < GameObjects3D.size(); i++)
	{
		if (!GameObjects3D[i]->isLight) {
			SetShadowParametrs(context.Get(), GameObjects3D[i]->worldMatrix, viewMatrix, projMatrix, lightViewMatrix,
				lightProjectionMatrix, GameObjects3D[i]->model.GetTexture(), m_RenderClass->GetShaderResourceView(), light[0]->GetPositionFloat3(),
				ambientColor, diffuseColor, exposure, gamma);
			EnableShadowShader();
			GameObjects3D[i]->Draw(myCamera.GetViewMatrix()*myCamera.GetProjectionMatrix());
		}
	}

}

void Graphics::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	context->ClearRenderTargetView(rtv.Get(), color);

	// Clear the depth buffer.
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;

}

void Graphics::EndScene()
{
	swapChain->Present(1, 0);
}

XMMATRIX Graphics::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

XMMATRIX * Graphics::GetOrthoMatrix()
{
	return &m_orthoMatrix;
}

XMMATRIX * Graphics::GetWorldMatrix()
{
	return &m_worldMatrix;
}

void Graphics::EnableDepthShader()
{
	context->IASetInputLayout(vertexSh_depth.GetInputLayout());
	context->VSSetShader(vertexSh_depth.GetShader(), NULL, 0);
	context->PSSetShader(pixelSh_depth.GetShader(), NULL, 0);
}

void Graphics::EnableShadowShader()
{
	context->IASetInputLayout(vertexShaderShadow.GetInputLayout());
	context->VSSetShader(vertexShaderShadow.GetShader(), NULL, 0);
	context->PSSetShader(pixelShaderShadow.GetShader(), NULL, 0);
	context->PSSetSamplers(0, 1, &m_sampleStateClamp);
	context->PSSetSamplers(1, 1, &m_sampleStateWrap);
}

void Graphics::EnableNormalShader()
{
	this->context->IASetInputLayout(this->vertexSh.GetInputLayout());
	this->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->context->RSSetState(this->rasterizerState.Get());
	this->context->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	this->context->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->context->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	this->context->VSSetShader(vertexSh.GetShader(), NULL, 0);
	this->context->PSSetShader(pixelSh.GetShader(), NULL, 0);

}

void Graphics::InitShadowParametrs()
{


}

bool Graphics::SetShaderDepthParameters(ID3D11DeviceContext * deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	m_matrixBufferDetph.data.world = worldMatrix;
	m_matrixBufferDetph.data.view = viewMatrix;
	m_matrixBufferDetph.data.projection = projectionMatrix;

	m_matrixBufferDetph.ApplyChanges();

	deviceContext->VSSetConstantBuffers(0, 1, m_matrixBufferDetph.GetAddressOf());

	return true;
}

void Graphics::SetShadowParametrs(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix,
	ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMapTexture, XMFLOAT3 lightPosition,
	XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, float exposure, float gamma)
{
	
	worldMatrix=XMMatrixTranspose(worldMatrix);
	viewMatrix=XMMatrixTranspose(viewMatrix);
	projectionMatrix=XMMatrixTranspose(projectionMatrix);
	lightViewMatrix=XMMatrixTranspose(lightViewMatrix);
	lightProjectionMatrix=XMMatrixTranspose(lightProjectionMatrix);


	m_matrixBuffer.data.world = worldMatrix;
	m_matrixBuffer.data.view = viewMatrix;
	m_matrixBuffer.data.projection = projectionMatrix;
	m_matrixBuffer.data.lightView = lightViewMatrix;
	m_matrixBuffer.data.lightProjection = lightProjectionMatrix;


	m_matrixBuffer.ApplyChanges();


	deviceContext->VSSetConstantBuffers(0, 1, m_matrixBuffer.GetAddressOf());

	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &depthMapTexture);

	m_lightBuffer.data.ambientColor = ambientColor;
	m_lightBuffer.data.diffuseColor = diffuseColor;
	m_lightBuffer.data.exposure = exposure;
	m_lightBuffer.data.gamma = gamma;
	m_lightBuffer.data.paddind = SimpleMath::Vector3(0.0f);
	m_lightBuffer.ApplyChanges();

	deviceContext->PSSetConstantBuffers(0, 1, m_lightBuffer.GetAddressOf());
	
	
	m_lightBuffer2.data.lightPosition = XMFLOAT3(2,2,2);
	m_lightBuffer2.data.padding = 0.0f;
	
	m_lightBuffer2.ApplyChanges();

	deviceContext->VSSetConstantBuffers(1, 1, m_lightBuffer2.GetAddressOf());
}



Model* Graphics::AddModel(XMFLOAT3 pos, Model* parent, XMFLOAT3 color)
{
	Model* newModel = new Model();
	newModel->Initialize(this->device.Get(), this->context.Get(), this->constBuffer_vertex, color);
	newModel->SetParent (parent);
	newModel->SetPosition(pos);
	models.push_back(newModel);
	return newModel;
		
}

GameObject3D* Graphics::AddGameObject3D (XMFLOAT3 pos, GameObject3D* parent, const std::string & filePath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{
	GameObject3D* newGameObject = new GameObject3D();
	HRESULT hr = newGameObject->Initialize(filePath,this->device.Get(), this->context.Get(), texture.Get() ,this->constBuffer_vertex);
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed load model");
	
	}
	newGameObject->SetParent(parent);
	newGameObject->SetPosition(pos);
	GameObjects3D.push_back(newGameObject);
	return newGameObject;
}

GameObject3D * Graphics::AddLight3D(XMFLOAT3 pos, GameObject3D * parent)
{
	Light* newLight = new Light();
	HRESULT hr = newLight->Initialize(this->device.Get(), this->context.Get(), this->Grass.Get(),this->constBuffer_vertex, this->constBuffer_light);
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed load model");

	}
	newLight->SetParent(parent);
	newLight->SetPosition(pos);
	XMFLOAT3 lookAt;
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 0.0f;
	newLight->SetLookAtPos(lookAt);
	newLight->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);
	GameObjects3D.push_back(newLight);
	return newLight;
}

bool Graphics::InitDirecrX(HWND hwnd)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1)
	{
		ErrorLogger::Log("No IDXGI Adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;

	// Set the width and height of the back buffer.
	scd.BufferDesc.Width = windowWidth;
	scd.BufferDesc.Height = windowHeight;

	// Set regular 32-bit surface for the back buffer.
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// Set the refresh rate of the back buffer.

	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	

	// Set the usage of the back buffer.
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	scd.OutputWindow = hwnd;

	// Turn multisampling off.
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	

	scd.Windowed = true;
	

	// Set the scan line ordering and scaling to unspecified.
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	scd.Flags = 0;

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(
		adapters[0].pAdapter, //IDXGI Adapter
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL, //FOR SOFTWARE DRIVER TYPE
		NULL, //FLAGS FOR RUNTIME LAYERS
		NULL, //FEATURE LEVELS ARRAY
		0, //# OF FEATURE LEVELS IN ARRAY
		D3D11_SDK_VERSION,
		&scd, //Swapchain description
		this->swapChain.GetAddressOf(), //Swapchain Address
		this->device.GetAddressOf(), //Device Address
		NULL, //Supported feature level
		this->context.GetAddressOf()); //Device Context Address

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create device and swapchain.");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "GetBuffer Failed.");
		return false;
	}

	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->rtv.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create render target view.");
		return false;
	}




	// Describe our Depth / Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = this->windowWidth;
	depthStencilDesc.Height = this->windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil buffer.");
		return false;
	}


	/*
	hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil view.");
		return false;
	}
	*/
	   	  

	/*
	this->context->OMSetRenderTargets(1, this->rtv.GetAddressOf(), this->depthStencilView.Get());
	*/

	//Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthstencildesc;
	ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthstencildesc.StencilEnable = true;
	depthstencildesc.StencilReadMask = 0xFF;
	depthstencildesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthstencildesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthstencildesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthstencildesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthstencildesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthstencildesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil state.");
		return false;
	}

	context->OMSetDepthStencilState(depthStencilState.Get(), 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf());

	context->OMSetRenderTargets(1, rtv.GetAddressOf(), depthStencilView.Get());


	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	

	//rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	//rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	hr = this->device->CreateRasterizerState(&rasterDesc, this->rasterizerState.GetAddressOf());
	
	context->RSSetState(this->rasterizerState.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create rasterizer state.");
		return false;
	}



	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	
	viewport.Width = this->windowWidth;
	viewport.Height = this->windowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	//Set the Viewport
	this->context->RSSetViewports(1, &viewport);

	
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->context.Get());
	spriteFont  = std::make_unique<DirectX::SpriteFont>(this->device.Get(),L"Data\\Fonts\\comic_sans_ms_16.spritefont");



	
	m_RenderClass = new RenderTextureClass;
	m_RenderClass->Initialize(this->device.Get(), SHADOWMAP_HEIGHT, SHADOWMAP_WIDTH);


	D3D11_SAMPLER_DESC samplerDesc;
	// Create a wrap texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	device->CreateSamplerState(&samplerDesc, &m_sampleStateWrap);


	// Create a clamp texture sampler state description.
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	// Create the texture sampler state.
	device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);






	return true;
}

bool Graphics::InitShaders()
{

	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG //Debug Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Debug\\";
#else  //x86 (Win32)
		shaderfolder = L"..\\Debug\\";
#endif
#else //Release Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Release\\";
#else  //x86 (Win32)
		shaderfolder = L"..\\Release\\";
#endif
#endif
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  }
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!vertexSh.Init(this->device, shaderfolder + L"vertexShader_Texture.cso", layout, numElements))
		return false;

	if (!pixelSh.Init(this->device, shaderfolder + L"pixelShader_Texture.cso"))
		return false;
	
	D3D11_INPUT_ELEMENT_DESC layout2d[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  }
	};

	UINT numElements2d = ARRAYSIZE(layout2d);

	if (!vertexSh_2d.Init(this->device, shaderfolder + L"vertexShader_2d.cso", layout2d, numElements2d))
		return false;

	if (!pixelSh_2d.Init(this->device, shaderfolder + L"pixelShader2d.cso"))
		return false;

	D3D11_INPUT_ELEMENT_DESC layoutShadow[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	
	UINT numElementsShadow = ARRAYSIZE(layoutShadow);

	if (!vertexShaderShadow.Init(this->device, shaderfolder + L"vertexShaderShadow.cso", layoutShadow, numElementsShadow))
		return false;

	if (!pixelShaderShadow.Init(this->device, shaderfolder + L"pixelShaderShadow.cso"))
		return false;




	D3D11_INPUT_ELEMENT_DESC layoutDepth[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  }
	};
	UINT numElementsDepth = ARRAYSIZE(layoutDepth);


	if (!vertexSh_depth.Init(this->device, shaderfolder + L"vertexShaderDepth.cso", layoutDepth, numElementsDepth))
		return false;
	
	if (!pixelSh_depth.Init(this->device, shaderfolder + L"pixelShaderDepth.cso"))
	{
		return false;

	}






	return true;
}

bool Graphics::InitScene()
{
	//Initialize Constant Buffer(s)
	HRESULT hr = this->constBuffer_vertex.Initialize(this->device.Get(), this->context.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create constBuffer_vertex");
		return false;
	}
	 hr = this->constBuffer_vertex2d.Initialize(this->device.Get(), this->context.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create constBuffer_vertex2d");
		return false;
	}

	hr = this->constBuffer_light.Initialize(this->device.Get(), this->context.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create constBuffer_light");
		return false;
	}

	this->constBuffer_light.data.ambientLoghtColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	this->constBuffer_light.data.ambientStrength = 0.5f;

	hr = this->m_matrixBuffer.Initialize(this->device.Get(), this->context.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create constBuffer_light");
		return false;
	}

	hr = this->m_lightBuffer.Initialize(this->device.Get(), this->context.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create constBuffer_light");
		return false;
	}

	hr = this->m_lightBuffer2.Initialize(this->device.Get(), this->context.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create constBuffer_light");
		return false;
	}

	hr = this->m_matrixBufferDetph.Initialize(this->device.Get(), this->context.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create m_matrixBufferDetph");
		return false;
	}
	
	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\starDeath.jpeg", nullptr, StarDeath.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create texture from starDeath");
		return false;
	}
	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\text1_PNG.png", nullptr, Ogon.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create texture from text1_PNG");
		return false;
	}
	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\mettal.jpg", nullptr, Mettal.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create texture from mettal");
		return false;
	}
	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\grass2.jpg", nullptr, Grass.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create texture from mettal");
		return false;
	}

	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\sprite_256x256.png", nullptr, Mettal.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create texture from mettal");
		return false;
	}

	myCamera2D.SetProjectionValues(windowWidth, windowHeight, 0.0f, 1.0f);

	sprite.Initialize(this->device.Get(), this->context.Get(), 256, 256, "Data/Textures/sprite_256x256.png", this->constBuffer_vertex2d, Mettal);

	myCamera.SetPosition(0.0f, 0.0f, 0.0f);
	myCamera.SetRotation(5.0f, 0.0f, 0.0f);

	//myCamera.SetProjectionValues(60.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 1000.0f);
	return true;
}

void Graphics::TurnZBufferOn()
{
	context->OMSetDepthStencilState(depthStencilState.Get(), 1);
	return;
}


void Graphics::TurnZBufferOff()
{
	context->OMSetDepthStencilState(depthDisabledStencilState.Get(), 1);
	return;
}