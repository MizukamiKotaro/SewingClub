#include "TestModel.h"
#include "Camera.h"
#include "ImGuiManager/ImGuiManager.h"

TestModel::TestModel() {
	model_ = std::make_unique<Model>("plane");
	model_->transform_.scale_ = Vector3(5.0f, 5.0f, 1.0f);
	animation_ = std::make_unique<Animation2D>();
	//animation_->Initialize(model_.get(),8, 8);
}

void TestModel::Update() {
#ifdef _DEBUG
	ImGui::Begin("TestModel");
	ImGui::DragFloat3("translate", &model_->transform_.translate_.x, 0.1f);
	ImGui::DragFloat3("scale", &model_->transform_.scale_.x, 0.1f);
	static Vector3 uvpos, uvscale = Vector3(1.0f, 1.0f, 1.0f);
	ImGui::DragFloat3("uvTranslate", &uvpos.x, 0.1f);
	ImGui::DragFloat3("uvScale", &uvscale.x, 0.1f);
	ImGui::DragFloat4("Color", &model_->color_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG
	animation_->Update();
	//model_->SetUVParam(uvscale, Vector3(0.0f, 0.0f, 0.0f), uvpos);
	model_->Update();
}

void TestModel::Draw(const Camera* camera) {
	model_->Draw(*camera);
}
