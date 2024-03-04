#pragma once
#include "Model.h"
#include <memory>
#include "Sprite.h"
#include "PostEffect/PostEffect.h"
#include "Bloom/Bloom.h"

class Camera;

class Screen {
public: 

	Screen();

	void Initialize();

	void Update();

	void Draw(Camera* camera);

	void PreDrawScene();

	void PostDrawScene();

private:

	void SetGPUHandle(D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle);

private:
	
	std::unique_ptr<Model> screen_;
	std::unique_ptr<Sprite> screenSprite_;
	std::unique_ptr<PostEffect> postEffect_;
	std::unique_ptr<Bloom> bloom_;
};
