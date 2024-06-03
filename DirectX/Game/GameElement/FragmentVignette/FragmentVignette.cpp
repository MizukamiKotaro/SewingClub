#include "FragmentVignette.h"
#include <algorithm>
#include "calc.h"

FragmentVignette::FragmentVignette()
{
	sprite_ = std::make_unique<Sprite>("ingame_crackFlame.png");
	vignette_ = std::make_unique<Vignette>();
	vignette_->color_ = { 1.0f,1.0f,1.0f,0.8f };
	vignette_->vignetteData_->scale = 50.0f;

	globalVariable_ = std::make_unique<GlobalVariableUser>("Effect", "FragmentEffect", "ガラス関係");
	SetGlobalVariable();

	vignette_->PreDrawScene();
	sprite_->Draw();
	vignette_->PostDrawScene();

	se_.LoadMP3("SE/Scene/ingame_goalOpen.mp3", "割れる音");
}

void FragmentVignette::Initialize()
{
	SetGlobalVariable();
	vignette_->vignetteData_->scale = 50.0f;
	isDraw_ = false;
	preHP_ = 100.0f;
	for (int32_t i = 0; i < 4; i++) {
		isSe_[i] = false;
	}
}

void FragmentVignette::Update(const float& hp)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG

	if (hp < 100.0f) {
		if (preHP_ == 100.0f || (preHP_ > 80.0f && hp <= 80.0f) || (preHP_ > 60.0f && hp <= 60.0f) || (preHP_ > 40.0f && hp <= 40.0f) || (preHP_ > 20.0f && hp <= 20.0f) || hp <= 0.0f) {
			if (preHP_ != hp) {
				se_.Play();
			}
		}
		preHP_ = hp;

		isDraw_ = true;
		float t = hp * 0.01f;
		float scale = (1.0f - t) * minScale_ + t * maxScale_;
		vignette_->vignetteData_->scale = Calc::Lerp(vignette_->vignetteData_->scale, scale, 0.1f);
		Vector3 color = (1.0f - t) * finalColor_ + t * Vector3{ 1.0f,1.0f,1.0f };
		vignette_->color_.x = Calc::Lerp(vignette_->color_.x, color.x, 0.01f);
		vignette_->color_.y = Calc::Lerp(vignette_->color_.y, color.y, 0.01f);
		vignette_->color_.z = Calc::Lerp(vignette_->color_.z, color.z, 0.01f);
	}
}

void FragmentVignette::Draw()
{
	if (isDraw_) {
		vignette_->Draw();
	}
}

void FragmentVignette::SetGlobalVariable()
{
	globalVariable_->AddItem("最終的な色", Vector3{ 1.0f,1.0f,1.0f });
	globalVariable_->AddItem("透明度", vignette_->color_.w);
	globalVariable_->AddItem("スケールの最大", 50.0f);
	globalVariable_->AddItem("スケールの最小", 0.0f);
	ApplyGlobalVariable();
}

void FragmentVignette::ApplyGlobalVariable()
{
	vignette_->color_.w = globalVariable_->GetFloatValue("透明度");
	finalColor_ = globalVariable_->GetVector3Value("最終的な色");
	minScale_ = globalVariable_->GetFloatValue("スケールの最小");
	maxScale_ = globalVariable_->GetFloatValue("スケールの最大");
}
