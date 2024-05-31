#include "TensionUpEffect.h"
#include "RandomGenerator/RandomGenerator.h"
#include "Ease/Ease.h"
#include "calc.h"
#include "ModelDataManager.h"
#include "TextureManager.h"

const Vector3* TensionUpEffectChip::babyPos_ = nullptr;
const Vector3* TensionUpEffectChip::cameraPos_ = nullptr;
ParticleManager* TensionUpEffectChip::instancingManager_ = nullptr;
const ParticleMeshTexData* TensionUpEffectChip::modelData_ = nullptr;

RandomGenerator* TensionUpEffect::rand_ = nullptr;
std::unique_ptr<GlobalVariableUser> TensionUpEffect::globalVariable_ = nullptr;

std::vector<const char*> TensionUpEffect::fNames;
std::vector<float> TensionUpEffect::fParas_;

Vector2 TensionUpEffect::baseScale_ = {};
int32_t TensionUpEffect::maxChipNum_ = 3;
int32_t TensionUpEffect::minChipNum_ = 7;

TensionUpEffect::TensionUpEffect(const float& tensionNum, const bool& isBaby, const Vector2& pos)
{
	isActive_ = true;
	time_ = 0.0f;
	int32_t chipNum = 0;
	if (tensionNum > 10.0f && isBaby) {
		chipNum = rand_->RandInt(maxChipNum_ - 1, maxChipNum_);
	}
	else {
		chipNum = rand_->RandInt(minChipNum_ - 1, minChipNum_);
	}
	float tension = tensionNum;
	float hoge = tension;
	float tensionChip = tension / chipNum;
	float chipHalf = tensionChip * 0.1f;
	float addAngle = 6.28f / chipNum;
	Vector2 scale = {};
	chips_.resize(chipNum);
	for (int32_t i = 0; i < chipNum; i++) {
		if (i == chipNum - 1) {
			tension = hoge;
		}
		else {
			float a = tensionChip + rand_->RandFloat(-chipHalf, chipHalf);
			if (hoge <= a) {
				a *= 0.5f;
			}
			hoge -= a;
			tension = a;
		}
		scale = baseScale_ * tension;
		chips_[i] = std::make_unique<TensionUpEffectChip>(pos, fParas_[kMoveSpeed], fParas_[kEndTime], rand_->RandFloat(addAngle * i, addAngle * (i + 1)), rand_->RandFloat(0.0f, 6.28f), scale, isBaby);
	}
}

void TensionUpEffect::StaticInitialize()
{
	if (!globalVariable_) {
		rand_ = RandomGenerator::GetInstance();
		globalVariable_ = std::make_unique<GlobalVariableUser>("Effect", "BabyTensionEffect", "消えるパーティクル関係");
		SetGlobalVariable();
	}
}

void TensionUpEffect::StaticUpdate()
{
	ApplyGlobalVariable();
}

void TensionUpEffect::Update(const float& deltaTime)
{
	time_ += deltaTime;
	bool isActive = false;
	for (size_t i = 0; i < chips_.size(); i++)
	{
		if (!chips_[i]->GetIsActive()) {
			continue;
		}
		chips_[i]->Update(deltaTime);
		isActive = true;
	}
	if (!isActive) {
		isActive_ = false;
	}
}

void TensionUpEffect::Draw() const
{
	if (isActive_) {
		for (const std::unique_ptr<TensionUpEffectChip>& chip : chips_) {
			chip->Draw();
		}
	}
}

void TensionUpEffect::SetGlobalVariable()
{
	InitializeGlobalVariable();

	globalVariable_->AddItem("基準スケール", Vector2{ 1.0f,1.0f });
	globalVariable_->AddItem("テンション上昇が小さいときに出す数", 5);
	globalVariable_->AddItem("テンション上昇が大きいときに出す数", 8);

	for (int i = 0; i < kFloatEnd; i++) {
		globalVariable_->AddItem(fNames[i], fParas_[i]);
	}

	ApplyGlobalVariable();
}

void TensionUpEffect::ApplyGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);
	}
	baseScale_ = globalVariable_->GetVector2Value("基準スケール");
	maxChipNum_ = globalVariable_->GetIntValue("テンション上昇が小さいときに出す数");
	minChipNum_ = globalVariable_->GetIntValue("テンション上昇が大きいときに出す数");
}

void TensionUpEffect::InitializeGlobalVariable()
{
	fParas_.resize(kFloatEnd);

	fNames.resize(kFloatEnd);
	fNames = {
		"生成するまでの時間",
		"移動スピード",
		"消える時間",
	};
}

TensionUpEffectChip::TensionUpEffectChip(const Vector2& pos, const float& speed, const float& time, const float& angle, const float& rotate, const Vector2& scale, const bool& isBaby)
{
	velocity_ = Vector2{ std::cosf(angle),std::sinf(angle) } * speed;
	rotate_ = rotate;
	scale_ = scale;
	pos_ = pos;
	time_ = 0.0f;
	finishTime_ = time;
	isActive_ = true;
	isBaby_ = isBaby;
	a_ = 1.0f;
}

void TensionUpEffectChip::StaticInitialize()
{
	instancingManager_ = ParticleManager::GetInstance();
	modelData_ = instancingManager_->GetDrawData(
		ParticleMeshTexData{ ModelDataManager::GetInstance()->LoadObj("plane"),
		TextureManager::GetInstance()->LoadTexture("whiteStar.png"),BlendMode::kBlendModeNormal});
}

void TensionUpEffectChip::Update(const float& deltaTime)
{
	time_ = std::clamp(time_ + deltaTime, 0.0f, finishTime_);
	pos_ += velocity_ * deltaTime;
	a_ = Ease::UseEase(1.0f, 0.0f, time_ / finishTime_, Ease::EaseIn, 4);
	if (time_ == finishTime_) {
		isActive_ = false;
	}
}

void TensionUpEffectChip::Draw() const
{
	if (isActive_) {
		if (!isBaby_) {
			Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_.x,scale_.y,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, Vector3{ pos_.x + cameraPos_->x,pos_.y + cameraPos_->y,-1.0f });
			instancingManager_->AddParticle(ParticleData{ matrix,Matrix4x4::MakeIdentity4x4(), {1.0f,1.0f,1.0f,a_} }, modelData_);
		}
		else {
			Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_.x,scale_.y,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, Vector3{ pos_.x + babyPos_->x,pos_.y + babyPos_->y,-1.0f });
			instancingManager_->AddParticle(ParticleData{ matrix,Matrix4x4::MakeIdentity4x4(), {1.0f,1.0f,1.0f,a_} }, modelData_);
		}
	}
}
