#include "SpotLightBox.h"
#include "Camera.h"
#ifdef _DEBUG
#include "ImGuiManager/ImGuiManager.h"
#endif // _DEBUG

const int kSpotNum = 2;

SpotLightBox::SpotLightBox()
{
	box_ = std::make_unique<Model>("SpotLightBox");
	box_->transform_.scale_ *= 1.01f;
	box_->color_ = { 0.0f,0.0f,0.0f,1.0f };
	point_ = std::make_unique<PointLight>();
	for (int i = 0; i < kSpotNum; i++) {
		spots_[i] = std::make_unique<SpotLight>();
		spots_[i]->light_->intensity = 1.0f + i;
	}
	box_->SetLight(point_.get());
	box_->SetLight(spots_[0].get());
	UpdatePositions();
}

void SpotLightBox::Initialize()
{
}

void SpotLightBox::Update()
{
#ifdef _DEBUG
	ImGui::Begin("SpotLightBox");
	ImGui::DragFloat3("位置", &box_->transform_.translate_.x, 0.01f);
	ImGui::SliderFloat3("角度", &spots_[0]->light_->direction.x, -1.0f, 1.0f);
	ImGui::ColorEdit3("外のスポットライトの色", &spots_[0]->light_->color.x);
	ImGui::SliderFloat("外のスポットライトの輝度", &spots_[0]->light_->intensity, 0.0f, 100.0f);
	ImGui::SliderFloat("外のスポットライトの開く減衰率", &spots_[0]->light_->decay, 0.0f, 100.0f);
	ImGui::SliderFloat("外のスポットライトの開く角度", &spots_[0]->light_->cosAngle, -1.0f, 1.0f);
	ImGui::SliderFloat("外のスポットライトの減衰し始める角度", &spots_[0]->light_->cosFalloffStart, -1.0f, 1.0f);
	ImGui::SliderFloat("外のスポットライトの距離", &spots_[0]->light_->distance, 0.0f, 100.0f);
	ImGui::ColorEdit3("内のスポットライトの色", &spots_[1]->light_->color.x);
	ImGui::SliderFloat("内のスポットライトの輝度", &spots_[1]->light_->intensity, 0.0f, 100.0f);
	ImGui::SliderFloat("内のスポットライトの開く減衰率", &spots_[1]->light_->decay, 0.0f, 100.0f);
	ImGui::SliderFloat("内のスポットライトの開く角度", &spots_[1]->light_->cosAngle, -1.0f, 1.0f);
	ImGui::SliderFloat("内のスポットライトの減衰し始める角度", &spots_[1]->light_->cosFalloffStart, -1.0f, 1.0f);
	ImGui::SliderFloat("内のスポットライトの距離", &spots_[1]->light_->distance, 0.0f, 100.0f);
	ImGui::End();
#endif // _DEBUG
	UpdatePositions();
}

void SpotLightBox::Draw(Camera* camera)
{
	//point_->Draw(*camera);
	spots_[0]->Draw(*camera);
	spots_[1]->Draw(*camera, BlendMode::kBlendModeAdd);
	box_->Draw(*camera);
}

void SpotLightBox::UpdatePositions()
{
	spots_[1]->light_->direction = spots_[0]->light_->direction;
	box_->transform_.SetQuaRot(Quaternion::DirectionToDirection(Vector3{ 1.0f,0.0f,0.0f }, spots_[0]->light_->direction));
	box_->Update();
	point_->light_->position = box_->transform_.translate_;
	point_->Update();
	for (int i = 0; i < kSpotNum; i++) {
		spots_[i]->light_->position = box_->transform_.translate_;
		spots_[i]->Update();
	}
}
