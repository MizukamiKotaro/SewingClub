#include "DeadLine.h"
#include "Camera.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "Camera.h"
#include "Ease/Ease.h"

DeadLine::DeadLine(Camera* camera, const Vector3* playerPos)
{
	camera_ = camera;
	modelData_ = ModelDataManager::GetInstance()->LoadObj("plane");
	instancingManager_ = InstancingModelManager::GetInstance();
	rand_ = RandomGenerator::GetInstance();

	playerPos_ = playerPos;

	globalVariable_ = std::make_unique<GlobalVariableUser>("DeadLine");

	lifeTime_ = 0.0f;
	firstScale_ = 0.0f;
	screenDivision_ = 8;
	generateTime_ = 2.0f;
	speed_ = 1.0f;
	firstPosition_ = -10.0f;

	timeCount_ = 0.0f;

	SetGlobalVariable();
	position_ = firstPosition_;
}

void DeadLine::Initialize()
{
	position_ = firstPosition_;
	timeCount_ = 0.0f;
	chips_.clear();
	GenerateChips(100.0f);
}

void DeadLine::Update(const float& deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG

	position_ += speed_ * deltaTime;

	for (std::list<std::unique_ptr<Chip>>::iterator it = chips_.begin(); it != chips_.end();) {
		(*it)->countTime_ = std::clamp((*it)->countTime_ + deltaTime, 0.0f, lifeTime_);
		if ((*it)->countTime_ >= lifeTime_) {
			it = chips_.erase(it);
		}
		else {
			float scale = Ease::UseEase(firstScale_, 0.0f, (*it)->countTime_, lifeTime_, Ease::Constant);
			(*it)->scale_ = Vector3{ scale,scale,scale };
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
			instancingManager_->AddBox(modelData_, { mat,1.0f,1.0f,1.0f,1.0f });
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
	ApplyGlobalVariable();
}

void DeadLine::ApplyGlobalVariable()
{
	lifeTime_ = globalVariable_->GetFloatValue("生存時間");
	firstScale_ = globalVariable_->GetFloatValue("スケール");
	screenDivision_ = globalVariable_->GetIntValue("縦に配置する数");
	generateTime_ = globalVariable_->GetFloatValue("生成時間");
	speed_ = globalVariable_->GetFloatValue("移動速度");
	firstPosition_ = globalVariable_->GetFloatValue("初期のx座標");
}

void DeadLine::GenerateChips(const float& deltaTime)
{
	timeCount_ += deltaTime;
	if (timeCount_ >= generateTime_) {
		timeCount_ = 0.0f;
		float center = camera_->transform_.translate_.y;
		int num = screenDivision_ / 2;
		for (int i = 0; i < screenDivision_; i++) {
			if (screenDivision_ % 2 == 0) {
				float y = center + firstScale_ * 0.8f * (float(i) - num - 0.5f);
				y += rand_->RandFloat(-firstScale_ * 0.1f, firstScale_ * 0.1f);
				float x = position_ + rand_->RandFloat(-firstScale_ * 0.05f, firstScale_ * 0.05f);
				chips_.push_back(std::make_unique<Chip>(Chip{ 0.0f,Vector3{x,y,-0.5f},Vector3{firstScale_,firstScale_,firstScale_},true }));
			}
			else {
				float y = center + firstScale_ * 0.8f * (float(i) - num);
				y += rand_->RandFloat(-firstScale_ * 0.1f, firstScale_ * 0.1f);
				float x = position_ + rand_->RandFloat(-firstScale_ * 0.05f, firstScale_ * 0.05f);
				chips_.push_back(std::make_unique<Chip>(Chip{ 0.0f,Vector3{x,y,-0.5f},Vector3{firstScale_,firstScale_,firstScale_},true }));
			}
		}
	}
}
