#pragma once
#include "WindowContainer.h"
#include "Timer.h"

class Engine : WindowContainer
{

public:
	float deltaTime;
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height, float posX, float posY);
	bool ProcessMessage();
	Graphics* GetGraphicsEngine();
	void Update();
	void RenderFrame();
	Model* AddModel(XMFLOAT3 pos, Model* parent, XMFLOAT3 color);
	GameObject3D* AddLight3D(XMFLOAT3 pos, GameObject3D* parent);
	GameObject3D* AddGameObject3D(XMFLOAT3 pos, GameObject3D* parent, const std::string & filePath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);
private:
	Timer timer;
};
