#include "SceneAcquisition.h"



SceneAcquisition* SceneAcquisition::GetInstance()
{
	static SceneAcquisition ins;
	return &ins;
}

void SceneAcquisition::Initialize()
{
	pe_ = std::make_unique<PostEffect>();
	pe_->Initialize();
	pe_->color_ = { 1,1,1,1 };
}

void SceneAcquisition::DrawScene()
{
	pe_->Draw();
}

void SceneAcquisition::PreDraw()
{
	pe_->PreDrawScene();
}

void SceneAcquisition::PostDraw()
{
	pe_->PostDrawScene();
}

void SceneAcquisition::Finalize()
{
	pe_.reset();
	pe_ = nullptr;
}

