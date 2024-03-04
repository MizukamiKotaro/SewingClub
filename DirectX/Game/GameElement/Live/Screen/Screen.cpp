#include "Screen.h"
#include "Camera.h"
#include "TextureManager.h"
#include <numbers>

Screen::Screen()
{
	screen_ = std::make_unique<Model>("plane");
	screen_->SetTexture(TextureManager::GetInstance()->LoadTexture("Resources/Texture/white.png"));
	screen_->transform_.translate_ = { 0.0f,4.0f,4.0f };
	screen_->transform_.scale_ = { 3.2f * 1.4f,1.8f * 1.4f,1.0f };
	screen_->Update();
	screen_->UnUsedLight();
	screen_->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });

	screenSprite_ = std::make_unique<Sprite>("screen.png");
	screenSprite_->pos_ = { 640.0f,360.0f };
	screenSprite_->Update();

	postEffect_ = std::make_unique<PostEffect>();
	bloom_ = std::make_unique<Bloom>();

	screen_->SetSRVGPUDescriptorHandle_(postEffect_->GetSRVGPUDescriptorHandle());
}

void Screen::Initialize() {
	
}

void Screen::Update() {

}

void Screen::Draw(Camera* camera) {
	screen_->Draw(*camera);
}

void Screen::PreDrawScene()
{
	bloom_->PreDrawScene();
}

void Screen::PostDrawScene()
{
	bloom_->PostDrawScene();

	postEffect_->PreDrawScene();

	bloom_->Draw();
	screenSprite_->Draw(BlendMode::kBlendModeMultiply);

	postEffect_->PostDrawScene();
}


