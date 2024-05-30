#include "BabyTensionEffect.h"
#include "RandomGenerator/RandomGenerator.h"
#include "Ease/Ease.h"
#include "calc.h"
#include "ModelDataManager.h"
#include "TextureManager.h"

const Vector3* BabyTensionEffectChip::babyPos_ = nullptr;
ParticleManager* BabyTensionEffectChip::instancingManager_ = nullptr;
const ParticleMeshTexData* BabyTensionEffectChip::modelData_ = nullptr;

RandomGenerator* BabyTensionEffect::rand_ = nullptr;
std::unique_ptr<GlobalVariableUser> BabyTensionEffect::globalVariable_ = nullptr;

std::vector<const char*> BabyTensionEffect::fNames;
std::vector<float> BabyTensionEffect::fParas_;

Vector2 BabyTensionEffect::baseScale_ = {};
Vector2 BabyTensionEffect::localGagePos_ = {};
Vector2 BabyTensionEffect::localMinGagePos_ = {};
Vector2 BabyTensionEffect::localMaxGagePos_ = {};
int32_t BabyTensionEffect::maxChipNum_ = 3;
int32_t BabyTensionEffect::minChipNum_ = 7;

BabyTensionEffect::BabyTensionEffect(const float& tensionNum)
{
	isActive_ = true;
	time_ = 0.0f;
	int32_t chipNum = 0;
	if (tensionNum > 10.0f) {
		chipNum = rand_->RandInt(minChipNum_ - 1, minChipNum_);
	}
	else {
		chipNum = rand_->RandInt(maxChipNum_ - 1, maxChipNum_);
	}
	float hoge = tensionNum;
	float tensionChip = hoge / chipNum;
	float chipHalf = tensionChip * 0.5f;
	float angleBase = 6.28f / chipNum;
	chips_.resize(chipNum);
	float tension, angle, length, rotate;
	Vector2 scale, gagePos;
	float floatMinLength = fParas_[kFloatLength] * 0.5f;
	for (int32_t i = 0; i < chipNum; chipNum++) {
		
		length = fParas_[kGenerateLength] + rand_->RandFloat(-fParas_[kLengthWidth], fParas_[kLengthWidth]);
		rotate = rand_->RandFloat(-0.4f, 0.4f);
		angle = rand_->RandFloat(angleBase * i, angleBase * (i + 1));
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
		gagePos = localGagePos_ + rand_->RandVector2(localMinGagePos_, localMaxGagePos_);
		chips_[i] = std::make_unique<BabyTensionEffectChip>(tension, angle, length, rotate, fParas_[kMoveSpeed], scale, gagePos, fParas_[kGenerateEndTime], rand_->RandFloat(0.0f, 6.28f), fParas_[kFloatTime], rand_->RandFloat(floatMinLength, fParas_[kFloatLength]));
	}
}

void BabyTensionEffect::StaticInitialize()
{
	if (!globalVariable_) {
		rand_ = RandomGenerator::GetInstance();
		globalVariable_ = std::make_unique<GlobalVariableUser>("Effect", "BabyTensionEffect", "パーティクル関係");
		SetGlobalVariable();
	}
}

void BabyTensionEffect::StaticUpdate()
{
	ApplyGlobalVariable();
}

void BabyTensionEffect::Update(const float& deltaTime)
{
	time_ += deltaTime;
	bool isActive = false;
	for (size_t i = 0; i < chips_.size(); i++)
	{
		if (!chips_[i]->GetIsActive()) {
			continue;
		}
		if (time_ > fParas_[kMoveRagTime] * i + fParas_[kStayTime] + fParas_[kGenerateEndTime]) {
			chips_[i]->Update(deltaTime);
		}
		else if (time_ > fParas_[kGenerateTime] + fParas_[kGenerateRagTime] * i) {
			chips_[i]->GenerateUpdate(deltaTime);
		}
		isActive = true;
	}
	if (!isActive) {
		isActive_ = false;
	}
}

void BabyTensionEffect::Draw() const
{
	if (isActive_) {
		for (const std::unique_ptr<BabyTensionEffectChip>& chip : chips_) {
			chip->Draw();
		}
	}
}

const float BabyTensionEffect::GetTension()
{
	float tension = 0.0f;
	for (size_t i = 0; i < chips_.size(); i++)
	{
		tension += chips_[i]->GetTension();
	}
	return tension;
}

void BabyTensionEffect::SetGlobalVariable()
{
	InitializeGlobalVariable();

	globalVariable_->AddItem("基準スケール", Vector2{ 1.0f,1.0f });
	globalVariable_->AddItem("ゲージの位置", Vector2{ 1.0f,1.0f });
	globalVariable_->AddItem("ゲージのランダム位置の最小", Vector2{ 1.0f,1.0f });
	globalVariable_->AddItem("ゲージのランダム位置の最大", Vector2{ 1.0f,1.0f });
	globalVariable_->AddItem("テンション上昇が小さいときに出す数", 4);
	globalVariable_->AddItem("テンション上昇が大きいときに出す数", 8);

	for (int i = 0; i < kFloatEnd; i++) {
		globalVariable_->AddItem(fNames[i], fParas_[i]);
	}
	globalVariable_->AddItem("浮き具合", float(0.0f));

	ApplyGlobalVariable();
}

void BabyTensionEffect::ApplyGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);
	}
	baseScale_ = globalVariable_->GetVector2Value("基準スケール");
	localGagePos_ = globalVariable_->GetVector2Value("ゲージの位置");
	localMinGagePos_ = globalVariable_->GetVector2Value("ゲージのランダム位置の最小");
	localMaxGagePos_ = globalVariable_->GetVector2Value("ゲージのランダム位置の最大");
	maxChipNum_ = globalVariable_->GetIntValue("テンション上昇が小さいときに出す数");
	minChipNum_ = globalVariable_->GetIntValue("テンション上昇が大きいときに出す数");
}

void BabyTensionEffect::InitializeGlobalVariable()
{
	fParas_.resize(kFloatEnd);

	fNames.resize(kFloatEnd);
	fNames = {
		"生成するまでの時間",
		"生成の時間差",
		"生成にかかる時間",
		"生成が終わってからの待機時間",
		"移動スピード",
		"移動のタイムラグ",
		"生成場所の距離",
		"距離の幅",
		"ふよふよの時間",
		"ふよふよの幅",
		"スケール",
	};
}

BabyTensionEffectChip::BabyTensionEffectChip(const float& tension, const float& angle, const float& length, const float& rotate, const float& speed, const Vector2& scale, const Vector2& gagePos, const float& generateTime, const float& floatAngle, const float& floatTime, const float& floatLength)
{
	tensionNum_ = tension;
	rotate_ = rotate;
	speed_ = speed;
	maxScale_ = scale;
	gagePos_ = gagePos;
	generateTime_ = generateTime;
	floatAngle_ = floatAngle;
	floatTime_ = floatTime;
	addFloatAngle_ = floatTime / 6.28f;
	floatlength_ = floatLength;

	generatePos_ = { std::cosf(angle) * length, std::sinf(angle) * length };

	localPos_ = { 0.0f,std::sinf(floatAngle_) * floatlength_ };
	scale_ = {};
	pos_ = {};
	time_ = 0.0f;
	isActive_ = true;
	isMove_ = false;
}

void BabyTensionEffectChip::StaticInitialize()
{
	instancingManager_ = ParticleManager::GetInstance();
	modelData_ = instancingManager_->GetDrawData(
		ParticleMeshTexData{ ModelDataManager::GetInstance()->LoadObj("plane"),
		TextureManager::GetInstance()->LoadTexture("ingame_note_HUD.png"),BlendMode::kBlendModeNormal});
}

const float BabyTensionEffectChip::GetTension()
{
	if (!isActive_) {
		float tension = tensionNum_;
		tensionNum_ = 0.0f;
		return tension;
	}
	return 0.0f;
}

void BabyTensionEffectChip::GenerateUpdate(const float& deltaTime)
{
	if (time_ < generateTime_) {
		time_ = std::clamp(time_ + deltaTime, 0.0f, generateTime_);
		float t = time_ / generateTime_;
		pos_ = Ease::UseEase({}, generatePos_ + localPos_, t, Ease::EaseOutSine);
		scale_ = Ease::UseEase({}, maxScale_, t, Ease::EaseOutSine);
	}
	else {
		FloatUpdate(deltaTime);
	}
}

void BabyTensionEffectChip::Update(const float& deltaTime)
{
	isMove_ = true;
	Vector2 v = (gagePos_ - pos_).Normalize();
	pos_ += v * speed_ * deltaTime;
	if (Calc::Dot(v, (gagePos_ - pos_).Normalize()) < 0.0f) {
		isActive_ = false;
	}
}

void BabyTensionEffectChip::FloatUpdate(const float& deltaTime)
{
	floatAngle_ = std::fmodf(floatAngle_ + addFloatAngle_ * deltaTime, 6.28f);
	localPos_ = { 0.0f,std::sinf(floatAngle_) * floatlength_ };
	pos_ = generatePos_ + localPos_;
}

void BabyTensionEffectChip::Draw() const
{
	if (isActive_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_.x,scale_.y,1.0f }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ pos_.x + babyPos_->x,pos_.y + babyPos_->y,-1.0f });
		instancingManager_->AddParticle(ParticleData{ matrix,Matrix4x4::MakeIdentity4x4(), {1.0f,1.0f,1.0f,1.0f} }, modelData_);
	}
}
