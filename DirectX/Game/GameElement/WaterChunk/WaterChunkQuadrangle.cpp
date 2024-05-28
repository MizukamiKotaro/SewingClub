#include "WaterChunkQuadrangle.h"
#include <algorithm>
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "WindowsInfo/WindowsInfo.h"
#include "Camera.h"
#include "GameElement/Player/Player.h"
#include "Drawers/DrawQuadrangle/DrawQuadrangleManager.h"
#include "Ease/Ease.h"
#include "WaterChunk.h"

const Player* WaterChunkQuadrangle::player_ = nullptr;
DrawQuadrangleManager* WaterChunkQuadrangle::quadrangleManager_ = nullptr;

WaterChunkQuadrangle::WaterChunkQuadrangle()
{
	Collider::CreateCollider(ColliderShape::QUADRANGLE2D, ColliderType::COLLIDER, ColliderMask::WATER);

	gravityArea_ = std::make_unique<GravityArea>(ColliderShape::QUADRANGLE2D);

	rotate_ = 0.0f;
	
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	isPlayer_ = false;
	preIsPlayer_ = false;
	isWave_ = false;
	isActive_ = true;
}

void WaterChunkQuadrangle::StaticInitialize()
{
	quadrangleManager_ = DrawQuadrangleManager::GetInstance();
}

void WaterChunkQuadrangle::Initialize()
{

}

void WaterChunkQuadrangle::Update(const float& deltaTime, Camera* camera)
{
#ifdef _DEBUG
	
#endif // _DEBUG

	if (!isPlayer_ && preIsPlayer_ && !player_->GetPreInWater()) {
		AddWave(false);
	}

	for (std::list<std::unique_ptr<WaterWave>>::iterator it = waves_.begin(); it != waves_.end();) {
		(*it)->Update(deltaTime);
		it++;
	}

	isWave_ = false;
	for (std::unique_ptr<WaterChunkChip>& chip : chips_) {

		for (std::unique_ptr<WaterWave>& wave : waves_) {
			float power = wave->GetPower(chip->GetRotate());
			if (power != 0.0f) {
				chip->AddOutPower(std::abs(power), power < 0);
			}
		}

		chip->Update(deltaTime);
		if (chip->IsWave()) {
			isWave_ = true;
		}
	}
	for (std::unique_ptr<WaterChunkChip>& chip : chips2_) {

		for (std::unique_ptr<WaterWave>& wave : waves_) {
			float power = wave->GetPower(chip->GetRotate());
			if (power != 0.0f) {
				chip->AddOutPower(std::abs(power), power < 0);
			}
		}

		chip->Update(deltaTime);
		if (chip->IsWave()) {
			isWave_ = true;
		}
	}

	for (std::list<std::unique_ptr<WaterWave>>::iterator it = waves_.begin(); it != waves_.end();) {
		if ((*it)->IsFinish()) {
			it = waves_.erase(it);
		}
		else {
			it++;
		}
	}
	ActiveCheck(camera);
	preIsPlayer_ = isPlayer_;
	isPlayer_ = false;
	if (isActive_) {
		gravityArea_->Update(leftTop_, rightTop_, leftBottom_, rightBottom_, startPos_, endPos_);
		SetCollider();
	}
}

void WaterChunkQuadrangle::Draw(Camera* camera) const
{
	if (isActive_) {
#ifdef _DEBUG
		//gravityArea_->Draw({ position_.x,position_.y }, { scale_,scale_ }, isSmaeGravitySize_, rotate_);
#endif // _DEBUG
		if (isWave_) {
			/*for (const std::unique_ptr<WaterChunkChip>& chip : chips_) {
				chip->Draw();
			}
			for (const std::unique_ptr<WaterChunkChip>& chip : chips2_) {
				chip->Draw();
			}
			float scale = WaterChunk::GetMinScale();
			quadrangleManager_->Draw(*camera,
				(leftTop_ - startPos_) * scale + startPos_, (rightTop_ - endPos_) * scale + endPos_,
				(leftBottom_ - startPos_) * scale + startPos_, (rightBottom_ - endPos_) * scale + endPos_);*/
			quadrangleManager_->Draw(*camera, leftTop_, rightTop_, leftBottom_, rightBottom_);
		}
		else {
			quadrangleManager_->Draw(*camera, leftTop_, rightTop_, leftBottom_, rightBottom_);
		}
	}
}



void WaterChunkQuadrangle::SetPlayer(const Player* player)
{
	player_ = player;
}

void WaterChunkQuadrangle::CreateQuadrangle(const Vector3& pos1, const float& scale1, const Vector3& pos2, const float& scale2)
{
	if (startPos_ == pos1 && endPos_ == pos2 && scale1 == startScale_ && scale2 == endScale_) {
		return;
	}

	startPos_ = pos1;
	endPos_ = pos2;
	startScale_ = scale1;
	endScale_ = scale2;

	length_ = (endPos_ - startPos_).Length();
	position_ = (endPos_ + startPos_) / 2;
	position_.z = startPos_.z;

	Vector3 pos = endPos_ - startPos_;
	Vector2 vect = { pos.x,pos.y };
	vect = vect.Normalize();
	rotate_ = std::acosf(vect.x);
	if (vect.y < 0) {
		rotate_ = 6.28f - rotate_;
	}

	leftTop_.x = -startScale_ * std::sinf(rotate_) + startPos_.x;
	leftTop_.y = startScale_ * std::cosf(rotate_) + startPos_.y;

	leftBottom_.x = startScale_ * std::sinf(rotate_) + startPos_.x;
	leftBottom_.y = -startScale_ * std::cosf(rotate_) + startPos_.y;

	rightTop_.x = -endScale_ * std::sinf(rotate_) + endPos_.x;
	rightTop_.y = endScale_ * std::cosf(rotate_) + endPos_.y;

	rightBottom_.x = endScale_ * std::sinf(rotate_) + endPos_.x;
	rightBottom_.y = -endScale_ * std::cosf(rotate_) + endPos_.y;

	CreateChips();
}

void WaterChunkQuadrangle::ActiveCheck(Camera* camera)
{
	Vector2 win = WindowsInfo::GetInstance()->GetWindowSize();
	float ratio = win.y / (std::tanf(0.225f) * (position_.z - camera->transform_.translate_.z) * 2);

	Vector2 pos{};
	pos.x = position_.x * ratio - camera->transform_.translate_.x * ratio + camera->transform_.translate_.x;
	pos.y = position_.y * ratio - camera->transform_.translate_.y * ratio + camera->transform_.translate_.y;

	float radius = length_ * ratio;

	if ((std::abs(pos.x) - radius > win.x * 0.7f) || (std::abs(pos.y) - radius > win.y * 0.7f)) {
		isActive_ = false;
	}
	else {
		isActive_ = true;
	}
}

void WaterChunkQuadrangle::CreateChips()
{
	chips_.clear();
	chips2_.clear();

	float posAdd_ = WaterChunkChip::GetScale();
	float pos = posAdd_ / 2;

	float scale = 0.0f;

	bool isEnd = false;
	float rotate1 = rotate_;
	float rotate2 = rotate_;
	if (rotate1 >= 3.14f) {
		rotate2 -= 3.14f;
	}
	rotate1 += 1.57f;
	rotate2 += 1.57f;
	if (rotate1 >= 6.28f) {
		rotate1 -= 6.28f;
	}
	if (rotate2 >= 6.28f) {
		rotate2 -= 6.28f;
	}

	while (true) {
		if (pos >= length_ - posAdd_ / 2) {
			pos = length_ - posAdd_ / 2;
			isEnd = true;
		}

		scale = Ease::UseEase(startScale_, endScale_, pos, length_, Ease::Constant) / 2;


		Vector3 center{};
		center.x = pos * std::cosf(rotate_);
		center.y = pos * std::sinf(rotate_);
		center += startPos_;

		Vector3 position{};
		position.x = -scale * std::sinf(rotate_);
		position.y = scale * std::cosf(rotate_);

		chips_.push_back(std::make_unique<WaterChunkChip>(center, center + position, rotate1, scale, false));
		chips2_.push_back(std::make_unique<WaterChunkChip>(center, center - position, rotate2, scale, false));

		if (isEnd) {
			break;
		}
		pos += posAdd_;
	}
}

void WaterChunkQuadrangle::AddWave(const bool& isDown)
{
	// 波発生
	Vector3 pos = player_->GetPosition() - position_;
	Vector2 vect = { pos.x,pos.y };
	vect = vect.Normalize();
	float rotate = std::acosf(vect.x);
	if (vect.y < 0) {
		rotate = 6.28f - rotate;
	}
	waves_.push_back(std::make_unique<WaterWave>(player_->GetVelocity(), rotate, isDown, 0));
	waves_.back()->Update(0.005f);
}

void WaterChunkQuadrangle::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::PLAYER) {
		isPlayer_ = true;
		if (!player_->GetPreInWater()) {
			// 波発生
			//AddWave(true);
		}
	}
}

void WaterChunkQuadrangle::SetCollider()
{
	Collider::SetQuadrangle(leftTop_, rightTop_, leftBottom_, rightBottom_);
	collisionManager_->SetCollider(this);
}