#include "Baby.h"
#include "GameElement/Player/Player.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "calc.h"

Baby::Baby(Player* player)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::BABY);
	Collider::AddTargetMask(ColliderMask::WATER);
	//Collider::AddTargetMask(ColliderMask::PLAYER);

	CreateGlobalVariable("Baby");

	player_ = player;

	//gravityAreaSearch_ = std::make_unique<GravityAreaSearch>();
	model_ = std::make_unique<Model>("plane");
	model_->transform_.translate_ = player_->GetPosition();

	velocity_ = {};
	isInWater_ = true;
	isFollowWater_ = false;
	speed_ = 0.0f;

	waterPos_ = { model_->transform_.translate_.x,model_->transform_.translate_.y };
	waterGravityPos_ = waterPos_;
	waterRadius_ = 0.0f;
	
	SetGlobalVariable();
}

void Baby::Initialize()
{
	velocity_ = {};
	isInWater_ = true;
	isFollowWater_ = false;
	speed_ = 0.0f;

	waterRadius_ = 0.0f;
	model_->transform_.translate_ = player_->GetPosition();
	model_->Update();
	waterPos_ = { model_->transform_.translate_.x,model_->transform_.translate_.y };
	waterGravityPos_ = waterPos_;
}

void Baby::Update(float deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG

	if (isInWater_) {
		InWaterUpdate(deltaTime);
		isInWater_ = false;
		waterGravityPos_ = {};
	}
	else {
		OutWaterUpdate(deltaTime);
	}

	model_->Update();
	SetCollider();
	//gravityAreaSearch_->Update(model_->transform_.translate_, velocity_);
}

void Baby::Draw(const Camera* camera)
{
	model_->Draw(*camera);
}


void Baby::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::WATER) {
		if (collider.GetShape() == ColliderShape::CIRCLE) {
			isInWater_ = true;
			isFollowWater_ = false;
			ShapeCircle* circle = collider.GetCircle();
			waterPos_ = circle->position_;
			waterRadius_ = circle->radius_.x;
			if (waterGravityPos_.x == 0.0f && waterGravityPos_.y == 0.0f) {
				waterGravityPos_ = circle->position_;
			}
			else {
				waterGravityPos_ = (circle->position_ + waterGravityPos_) * 0.5f;

			}
		}
		else if (collider.GetShape() == ColliderShape::QUADRANGLE2D) {
			/*isInWater_ = true;
			isFollowWater_ = false;
			ShapeQuadrangle* quadrangle = collider.GetQuadrangle();
			Vector2 startPos = (quadrangle->leftTop_ + quadrangle->leftBottom_) / 2;
			Vector2 endPos = (quadrangle->rightTop_ + quadrangle->rightBottom_) / 2;
			Vector3 vect = { endPos.x - startPos.x, endPos.y - startPos.y, 0.0f };
			Vector3 point = Calc::ClosestPoint({ GetCircle()->position_.x,GetCircle()->position_.y,0.0f }, Segment{ {startPos.x,startPos.y,0.0f},vect });
			Vector2 position = { point.x,point.y };
			waterPos_ = position;
			waterRadius_ = circle->radius_.x;
			if (waterGravityPos_.x == 0.0f && waterGravityPos_.y == 0.0f) {
				waterGravityPos_ = position;
			}
			else {
				waterGravityPos_ = (position + waterGravityPos_) * 0.5f;

			}*/
		}
	}
}

void Baby::SetCollider()
{
	Collider::SetCircle({ model_->transform_.translate_.x,model_->transform_.translate_.y }, 0.0f);
	collisionManager_->SetCollider(this);
}

void Baby::SetGlobalVariable()
{
	InitializeGlobalVariable();

	globalVariable_->AddItem("スケール", Vector2{ 1.0f,1.0f });

	for (int i = 0; i < kFloatEnd; i++) {
		globalVariable_->AddItem(fNames[i], fParas_[i]);
	}

	ApplyGlobalVariable();
}

void Baby::ApplyGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);
	}

	if (fParas_[FloatParamater::kMaxPlayerLength] == 0.0f) {
		fParas_[FloatParamater::kMaxPlayerLength] = 0.01f;
	}

	Vector2 scale = globalVariable_->GetVector2Value("スケール");
	model_->transform_.scale_ = { scale.x,scale.y,1.0f };
	model_->transform_.UpdateMatrix();
}

void Baby::OutWaterUpdate(const float& deltaTime)
{
	Vector3 vect = player_->GetPosition() - model_->transform_.translate_;
	vect.z = 0.0f;

	float length = vect.Length();

	if (length >= fParas_[FloatParamater::kLimitePlayerLength]) {
		// 無理やり引っ張る処理

	}
	else {
		length = std::clamp(length, 0.0f, fParas_[FloatParamater::kMaxPlayerLength]);
		float t = length / fParas_[FloatParamater::kMaxPlayerLength];

		if (isFollowWater_) {
			Vector2 vect1 = { vect.x,vect.y };
			vect1 = vect1.Normalize();

			Vector2 vect2 = waterPos_ - Vector2{model_->transform_.translate_.x,model_->transform_.translate_.y};
			vect2 = vect2.Normalize();

			float outer = Calc::Outer(vect2, vect1);

			float angle = 0.85f;
			if (std::abs(outer) >= angle) {
				isFollowWater_ = false;

				velocity_ = {};
				if (outer > 0.0f) {
					outer = 1.0f - outer;
				}
				else {
					outer = 1.0f + outer;
					outer = -outer;
				}
				velocity_.x = vect1.x * std::cosf(outer) - vect1.y * std::sinf(outer);
				velocity_.y = vect1.y * std::cosf(outer) + vect1.x * std::sinf(outer);

				speed_ = t * fParas_[FloatParamater::kMaxAcceleration] * deltaTime;
				velocity_ *= speed_;
				model_->transform_.translate_ += velocity_;
			}
			else {
				outer = -outer / angle;
				outer = fParas_[FloatParamater::kMaxSlide] * outer * t * deltaTime;

				speed_ = 0.0f;
				velocity_ = {};

				vect2 *= -1;
				Vector3 pos = {};
				pos.x = vect2.x * std::cosf(outer) - vect2.y * std::sinf(outer);
				pos.y = vect2.y * std::cosf(outer) + vect2.x * std::sinf(outer);
				model_->transform_.translate_ = Vector3{ waterPos_.x,waterPos_.y,model_->transform_.translate_.z } + pos * (waterRadius_ + 0.01f);
			}
		}
		else {
			float speed = t * fParas_[FloatParamater::kMaxAcceleration] * deltaTime;
			velocity_ += vect.Normalize() * speed;
			speed_ = std::clamp(velocity_.Length(), 0.0f, fParas_[FloatParamater::kMaxSpeed] * deltaTime);
			velocity_ = velocity_.Normalize() * speed_;
			model_->transform_.translate_ += velocity_;
		}
	}
}

void Baby::InWaterUpdate(const float& deltaTime)
{
	Vector3 vect = player_->GetPosition() - model_->transform_.translate_;
	vect.z = 0.0f;

	float length = vect.Length();

	if (length >= fParas_[FloatParamater::kLimitePlayerLength]) {
		// 無理やり引っ張る処理

	}
	else {
		length = std::clamp(length, 0.0f, fParas_[FloatParamater::kMaxPlayerLength]);
		float t = length / fParas_[FloatParamater::kMaxPlayerLength];

		Vector3 vect2 = model_->transform_.translate_ - Vector3{ waterGravityPos_.x,waterGravityPos_.y,model_->transform_.translate_.z };
		vect2 = vect2.Normalize();

		velocity_ += vect.Normalize() * t * fParas_[FloatParamater::kMaxAcceleration] * deltaTime;
		velocity_ += vect2 * fParas_[kBuoyancy] * deltaTime;

		speed_ = std::clamp(velocity_.Length(), 0.0f, fParas_[FloatParamater::kMaxSpeed] * deltaTime);
		velocity_ = velocity_.Normalize() * speed_;
		model_->transform_.translate_ += velocity_;

		vect2 = Vector3{ waterGravityPos_.x,waterGravityPos_.y,model_->transform_.translate_.z } - model_->transform_.translate_;
		if (vect2.Length() >= waterRadius_) {
			vect = player_->GetPosition() - model_->transform_.translate_;
			vect.z = 0.0f;

			vect2 = vect2.Normalize();
			vect = vect.Normalize();

			if (Calc::Dot(Vector2{ vect.x,vect.y }, Vector2{ vect2.x,vect2.y }) > 0.0f) {
				isFollowWater_ = true;
				vect2 *= -1;
				model_->transform_.translate_ = Vector3{ waterGravityPos_.x,waterGravityPos_.y,model_->transform_.translate_.z } + vect2 * (waterRadius_ + 0.01f);
			}
		}
	}
}

void Baby::InitializeGlobalVariable()
{
	fParas_.resize(kFloatEnd);

	fNames.resize(kFloatEnd);
	fNames = {
		"加速度が最大になるときのプレイヤーとの距離",
		"プレイヤーとの限界距離",
		"加速度の最大",
		"最大速度",
		"最低速度",
		"水の浮力",
		"加速度が最大の時の移動角度",
	};
}
