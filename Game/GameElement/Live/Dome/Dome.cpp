#include "Dome.h"
#include "Camera.h"

Dome::Dome()
{
	model_ = std::make_unique<Model>("skydome");
	model_->Update();
	model_->UnUsedLight();
	model_->SetColor(Vector4{ 0.02f,0.02f,0.02f,1.0f });
}

void Dome::Initialize() {
	
}

void Dome::Update() {

}

void Dome::Draw(Camera* camera) { 
	model_->Draw(*camera);
}
