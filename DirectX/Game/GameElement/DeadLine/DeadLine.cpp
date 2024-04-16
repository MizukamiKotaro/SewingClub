#include "DeadLine.h"
#include "Camera.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "Camera.h"
#include "Ease/Ease.h"

DeadLine::DeadLine(Camera* camera, const Vector3* playerPos)
{
	camera_ = camera;
	instancingManager_ = ParticleManager::GetInstance();
	modelData_ = instancingManager_->GetDrawData("deadbox.png");
	rand_ = RandomGenerator::GetInstance();

	playerPos_ = playerPos;

	globalVariable_ = std::make_unique<GlobalVariableUser>("DeadLine", "DeadLine");

	lifeTime_ = 0.0f;
	firstScale_ = 0.0f;
	screenDivision_ = 8;
	generateTime_ = 2.0f;
	speed_ = 1.0f;
	firstPosition_ = -10.0f;
	backSpeed_ = 1.0f;

	SetGlobalVariable();
	position_ = firstPosition_;
	generator_.resize(screenDivision_);
}

void DeadLine::Initialize()
{
	position_ = firstPosition_;
	generator_.resize(screenDivision_);
	for (int i = 0; i < screenDivision_; i++) {
		generator_[i].generateTime = 0.0f;
		generator_[i].timeCount = 0.0f;
	}
	chips_.clear();
	GenerateChips(100.0f);
	for (int i = 0; i < screenDivision_; i++) {
		generator_[i].timeCount = rand_->RandFloat(0.0f, 0.3f);
	}
}

void DeadLine::Update(const float& deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG

	position_ += speed_ * deltaTime;
	float speed = backSpeed_ * deltaTime;

	for (std::list<std::unique_ptr<Chip>>::iterator it = chips_.begin(); it != chips_.end();) {
		(*it)->countTime_ = std::clamp((*it)->countTime_ + deltaTime, 0.0f, lifeTime_);
		if ((*it)->countTime_ >= lifeTime_) {
			it = chips_.erase(it);
		}
		else {
			float scale = Ease::UseEase(firstScale_, 0.0f, (*it)->countTime_, lifeTime_, Ease::Constant);
			(*it)->scale_ = Vector3{ scale,scale,scale };
			(*it)->position_.x -= speed;
			(*it)->isActive_ = camera_->InScreenCheck2D((*it)->position_, (*it)->scale_.x);
			it++;
		}
	}

	GenerateChips(deltaTime);
}

void DeadLine::Draw()
{
	for (std::unique_ptr<Chip>& chip : chips_) {
		if (chip->isActive_) {
			Matrix4x4 mat = Matrix4x4::MakeAffinMatrix(chip->scale_, Vector3{}, chip->position_);
			instancingManager_->AddParticle({ mat,Matrix4x4::MakeIdentity4x4(), 1.0f,1.0f,1.0f,1.0f }, modelData_);
		}
	}
}

void DeadLine::SetGlobalVariable()
{
	globalVariable_->AddItem("生存時間", lifeTime_);
	globalVariable_->AddItem("スケール", firstScale_);
	globalVariable_->AddItem("縦に配置する数", screenDivision_);
	globalVariable_->AddItem("生成時間", generateTime_);
	globalVariable_->AddItem("移動速度", speed_);
	globalVariable_->AddItem("初期のx座標", firstPosition_);
	globalVariable_->AddItem("左に流れる演出の速度", backSpeed_);
	ApplyGlobalVariable();
}

void DeadLine::ApplyGlobalVariable()
{
	lifeTime_ = globalVariable_->GetFloatValue("生存時間");
	firstScale_ = globalVariable_->GetFloatValue("スケール");
	generateTime_ = globalVariable_->GetFloatValue("生成時間");
	speed_ = globalVariable_->GetFloatValue("移動速度");
	firstPosition_ = globalVariable_->GetFloatValue("初期のx座標");
	backSpeed_ = globalVariable_->GetFloatValue("左に流れる演出の速度");

	int n = screenDivision_;
	screenDivision_ = globalVariable_->GetIntValue("縦に配置する数");
	if (n != screenDivision_) {
		Initialize();
	}
}

void DeadLine::GenerateChips(const float& deltaTime)
{
	for (int i = 0; i < screenDivision_; i++) {
		generator_[i].timeCount += deltaTime;
		if (generator_[i].timeCount >= generator_[i].generateTime) {
			generator_[i].timeCount = 0.0f;
			generator_[i].generateTime = generateTime_ + rand_->RandFloat(-0.1f, 0.1f);
			float center = camera_->transform_.translate_.y;
			int num = screenDivision_ / 2;
			if (screenDivision_ % 2 == 0) {
				float y = center + firstScale_ * 0.8f * (float(i) - num - 0.5f);
				y += rand_->RandFloat(-firstScale_ * 0.1f, firstScale_ * 0.1f);
				float x = position_ + rand_->RandFloat(-firstScale_ * 0.08f, firstScale_ * 0.08f);
				float scale = firstScale_ + rand_->RandFloat(-0.05f, 0.05f);
				chips_.push_back(std::make_unique<Chip>(Chip{ 0.0f,Vector3{x,y,-0.5f},Vector3{scale,scale,scale},true }));
			}
			else {
				float y = center + firstScale_ * 0.8f * (float(i) - num);
				y += rand_->RandFloat(-firstScale_ * 0.15f, firstScale_ * 0.15f);
				float x = position_ + rand_->RandFloat(-firstScale_ * 0.2f, firstScale_ * 0.2f);
				float scale = firstScale_ + rand_->RandFloat(-0.05f, 0.05f);
				chips_.push_back(std::make_unique<Chip>(Chip{ 0.0f,Vector3{x,y,-0.5f},Vector3{scale,scale,scale},true }));
			}
		}
	}
}
