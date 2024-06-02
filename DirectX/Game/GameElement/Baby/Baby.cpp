#include "Baby.h"
#include "GameElement/Player/Player.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "calc.h"
#include "GameElement/Animation/AnimationManager.h"
#include "TextureManager.h"
#include "Input.h"
#include "Ease/Ease.h"
#include "GameElement/BabyTensionEffect/BabyTensionEffectManager.h"
#include "GameElement/Effects/ComboEffect.h"
#include "GameElement/HitStop/HitStop.h"

Input* input = nullptr;

Baby::Baby(Player* player)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::BABY);
	Collider::AddTargetMask(ColliderMask::WATER);

	CreateGlobalVariable("Baby");

	player_ = player;

	gravityAreaSearch_ = std::make_unique<GravityAreaSearch>();
	model_ = std::make_unique<Model>("plane");
	model_->transform_.translate_ = player_->GetPosition();

	baby_ = std::make_unique<Model>("plane");
	baby_->transform_.parent_ = &model_->transform_;

	velocity_ = {};
	isInWater_ = true;
	isFollowWater_ = false;
	speed_ = 0.0f;

	waterPos_ = { model_->transform_.translate_.x,model_->transform_.translate_.y };
	waterGravityPos_ = waterPos_;
	waterRadius_ = 0.0f;

	isCircleWater_ = true;
	yarn_ = std::make_unique<Model>("Cube");
	yarn_->transform_.scale_ = { 0.1f,0.1f,0.1f };
	combo_.num = 0;
	combo_.maxNum = 1;
	TensionInitialize();
	RideInWaterInitialize();
	SetGlobalVariable();

	// アニメーションの初期化とモデルのセット
	animation_ = std::make_unique<Animation2D>(AnimationManager::GetInstance()->AddAnimation("babynormal"));
	// UV座標のセット
	Transform handle = animation_->GetSceneUV(0u);
	baby_->SetUVParam(handle);
	baby_->SetTexture(TextureManager::GetInstance()->LoadTexture("baby_normal.png"));
	animation_->Play(true);

	effeEnterW_ = std::make_unique<EffectEnterWater>("BabyEnterWater");

	input = Input::GetInstance();
	tensionEffectManager_ = BabyTensionEffectManager::GetInstance();

	for (int i = 0; i < SEType::kMaxNumber; i++) {
		se_[i].LoadMP3(sePath_[i], seText_[i]);
	}
}

Baby::~Baby() {
	for (int i = 0; i < SEType::kMaxNumber; i++) {
		se_[i].Stop();
	}
}

void Baby::Initialize()
{
	velocity_ = {};
	preIsInWater_ = true;
	isInWater_ = true;
	isFollowWater_ = false;
	speed_ = 0.0f;
	isClear_ = false;
	clearTime_ = 0.0f;

	waterRadius_ = 0.0f;
	model_->transform_.translate_ = player_->GetPosition();
	model_->transform_.rotate_ = {};
	waterPos_ = { model_->transform_.translate_.x,model_->transform_.translate_.y };
	waterGravityPos_ = waterPos_;
	prePosition_ = model_->transform_.translate_;
	yarn_->transform_.translate_ = { model_->transform_.translate_.x, model_->transform_.translate_.y, model_->transform_.translate_.z + 0.06f };
	yarn_->transform_.rotate_ = {};
	yarn_->transform_.scale_ = { 0.05f,0.05f,0.05f };
	SetGlobalVariable();
	isCircleWater_ = true;
	combo_.num = 0;
	effeEnterW_->Initialize();
	TensionInitialize();
	RideInWaterInitialize();
	preIsInWaterPlayer_ = true;
	prePreIsInWaterPlayer_ = preIsInWaterPlayer_;
	playerOutTime_ = 0.0f;
	isRide_ = false;
	//音も再生
	se_[SEType::sNormal].Play();
	yarn_->Update();
	model_->Update();
	baby_->Update();
}

void Baby::Update(float deltaTime)
{
	if (deltaTime == 0.0f) {
		return;
	}
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG
	prePosition_ = model_->transform_.translate_;
	preIsInWater_ = isInWater_;

	bool isRide = false;
	if (!player_->GetPreInWater()) {
		Vector3 pos = player_->GetPosition() - model_->transform_.translate_;
		pos.z = 0.0f;
		float length = pos.Length();
		if (length <= fParas_[kNearPlayerLength]) {
			isRide = true;
			isRide_ = true;
			playerOutTime_ = 0.0f;
		}
		else if (length <= fParas_[kNearPulledPlayerLength]) {
			isRide = true;
			playerOutTime_ = 0.0f;
		}
	}
	preIsInWaterPlayer_ = player_->GetPreInWater();

	if (isRide || isRide_) {
		RideUpdate(deltaTime);
	}
	else if (isInWater_) {
		InWaterUpdate(deltaTime);
		isInWater_ = false;
		waterGravityPos_ = {};
	}
	else {
		OutWaterUpdate(deltaTime);
	}

	Vector2 vect;
	if (!preIsInWater_ && !isFollowWater_) {
		Vector3 pos = player_->GetPosition() - model_->transform_.translate_;
		vect = { pos.x,pos.y };
		vect = vect.Normalize();
		float rotate = std::acosf(vect.x);
		if (vect.y < 0) {
			rotate = 6.28f - rotate;
		}
		rotate = std::fmod(rotate + 1.57f, 6.28f);
		if (model_->transform_.rotate_.z >= 4.71f && rotate <= 1.57f) {
			model_->transform_.rotate_.z = Calc::Lerp(model_->transform_.rotate_.z, rotate + 6.28f, 0.05f);
		}
		else if (model_->transform_.rotate_.z <= 1.57f && rotate >= 4.71f){
			model_->transform_.rotate_.z = Calc::Lerp(model_->transform_.rotate_.z, rotate - 6.28f, 0.05f);
		}
		else {
			model_->transform_.rotate_.z = Calc::Lerp(model_->transform_.rotate_.z, rotate, 0.05f);
		}
	}
	else {
		vect = Vector2{ model_->transform_.translate_.x,model_->transform_.translate_.y } - waterPos_;
		vect = vect.Normalize();
		float rotate = std::acosf(vect.x);
		if (vect.y < 0) {
			rotate = 6.28f - rotate;
		}
		rotate -= 1.57f;
		if (rotate < 0.0f) {
			rotate = 6.28f + rotate;
		}

		if (model_->transform_.rotate_.z >= 4.71f && rotate <= 1.57f) {
			model_->transform_.rotate_.z = Calc::Lerp(model_->transform_.rotate_.z, rotate + 6.28f, 0.05f);
		}
		else if (model_->transform_.rotate_.z <= 1.57f && rotate >= 4.71f) {
			model_->transform_.rotate_.z = Calc::Lerp(model_->transform_.rotate_.z, rotate - 6.28f, 0.05f);
		}
		else {
			model_->transform_.rotate_.z = Calc::Lerp(model_->transform_.rotate_.z, rotate, 0.05f);
		}
	}
	model_->transform_.rotate_.z = std::fmod(model_->transform_.rotate_.z, 6.28f);
	if (model_->transform_.rotate_.z < 0.0f) {
		model_->transform_.rotate_.z += 6.28f;
	}
	model_->Update();
	baby_->Update();
	SetCollider();
	TensionUpdate(deltaTime);
	rideInWater_.isRideInWater = false;
	gravityAreaSearch_->Update(model_->transform_.translate_, velocity_);
	prePreIsInWaterPlayer_ = preIsInWaterPlayer_;
	if (animation_->Update("babynormal",deltaTime)) {
		// modelにuvのセット
		baby_->SetUVParam(animation_->GetUVTrans());
	}
	TextureUpdate();

	if (spawnWaitCount_-- <= 0) {
		isSpawnEffect_ = true;
	}
	effeEnterW_->Update();
}

void Baby::Draw(const Camera* camera)
{
	baby_->Draw(*camera);
	YarnUpdate();
	yarn_->Draw(*camera);

}

void Baby::EffectDraw()
{
	effeEnterW_->Draw();
}

const bool Baby::GetIsCry() const
{
	if (tension_.face == Face::kCry) {
		return true;
	}
	return false;
}

const bool Baby::GetIsSuperSuperSmile() const
{
	return false;
}

void Baby::ClearUpdate(const float& deltaTime)
{
	if (isClear_ || model_->transform_.scale_.x <= 0.0f) {
		return;
	}
	float clearTime = player_->GetClearTime();
	clearTime_ = std::clamp(clearTime_ + deltaTime, 0.0f, clearTime);

	Vector2 goalPos = player_->GetGoalPos();

	Vector2 pos = Vector2{ model_->transform_.translate_.x,model_->transform_.translate_.y } - goalPos;
	float len = pos.Length() - player_->GetClearSpeed() * deltaTime;
	float r = player_->GetClearRotate() * deltaTime;
	if (len <= 0.0f) {
		pos = {};
	}
	else {
		pos = pos.Normalize() * len;
		model_->transform_.translate_.x = pos.x * std::cosf(r) - pos.y * std::sinf(r) + goalPos.x;
		model_->transform_.translate_.y = pos.x * std::sinf(r) + pos.y * std::cosf(r) + goalPos.y;
	}
	Vector2 s = globalVariable_->GetVector2Value("スケール");
	Vector3 scale = { s.x,s.y,1.0f };

	model_->transform_.scale_ = Ease::UseEase(scale, Vector3{}, clearTime_, clearTime, Ease::EaseType::Constant);

	model_->transform_.rotate_.z += r;

	if (clearTime_ >= clearTime) {
		isClear_ = true;
	}
	model_->Update();
	baby_->Update();
	YarnUpdate();
}


void Baby::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::WATER) {
		if (!isRide_ || (isRide_ && (player_->GetSpeed() < rideInWater_.rideFinishSpeed || rideInWater_.rideFinishTime > fParas_[kInWaterTime]))) {
			isRide_ = false;
			combo_.num = 0;
			if (!preIsInWater_ && isFollowWater_) {
				model_->transform_.translate_ = prePosition_;
				Vector3 pos = player_->GetPosition() - model_->transform_.translate_;
				Vector2 vect = { pos.x,pos.y };
				vect = vect.Normalize();
				float rotate = std::acosf(vect.x);
				if (vect.y < 0) {
					rotate = 6.28f - rotate;
				}
				rotate = std::fmod(rotate + 1.57f, 6.28f);
				if (model_->transform_.rotate_.z >= 4.71f && rotate <= 1.57f) {
					model_->transform_.rotate_.z = Calc::Lerp(model_->transform_.rotate_.z, rotate + 6.28f, 0.01f);
				}
				else if (model_->transform_.rotate_.z <= 1.57f && rotate >= 4.71f) {
					model_->transform_.rotate_.z = Calc::Lerp(model_->transform_.rotate_.z, rotate - 6.28f, 0.01f);
				}
				else {
					model_->transform_.rotate_.z = Calc::Lerp(model_->transform_.rotate_.z, rotate, 0.01f);
				}
				model_->transform_.rotate_.z = std::fmod(model_->transform_.rotate_.z, 6.28f);
				if (model_->transform_.rotate_.z < 0.0f) {
					model_->transform_.rotate_.z += 6.28f;
				}
				model_->Update();
				baby_->Update();
			}

			if (collider.GetShape() == ColliderShape::CIRCLE) {
				if (!preIsInWater_ && isFollowWater_ && (player_->GetPosition() - model_->transform_.translate_).Length() <= fParas_[FloatParamater::kLimitePlayerLength]) {
					isInWater_ = false;
					isFollowWater_ = true;
				}
				else {
					isInWater_ = true;
					isFollowWater_ = false;
				}
				isCircleWater_ = true;
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
				if (!preIsInWater_ && isFollowWater_ && (player_->GetPosition() - model_->transform_.translate_).Length() <= fParas_[FloatParamater::kLimitePlayerLength]) {
					isInWater_ = false;
					isFollowWater_ = true;
				}
				else {
					isInWater_ = true;
					isFollowWater_ = false;
				}
				isCircleWater_ = false;
				ShapeQuadrangle* quadrangle = collider.GetQuadrangle();
				Vector2 startPos = (quadrangle->leftTop_ + quadrangle->leftBottom_) / 2;
				Vector2 endPos = (quadrangle->rightTop_ + quadrangle->rightBottom_) / 2;
				Vector3 vect = { endPos.x - startPos.x, endPos.y - startPos.y, 0.0f };
				Vector3 point = Calc::ClosestPoint({ model_->transform_.translate_.x,model_->transform_.translate_.y,0.0f }, Segment{ {startPos.x,startPos.y,0.0f},vect });
				Vector2 position = { point.x,point.y };
				waterPos_ = position;

				startPos_ = startPos;
				endPos_ = endPos;
				startScale_ = (quadrangle->leftTop_ - quadrangle->leftBottom_).Length() / 2;
				endScale_ = (quadrangle->rightTop_ - quadrangle->rightBottom_).Length() / 2;

				float t = (position - startPos_).Length() / (endPos_ - startPos_).Length();
				waterRadius_ = (1.0f - t) * startScale_ + t * endScale_;

				Vector3 v = model_->transform_.translate_ - point;
				Vector2 v2 = { v.x,v.y };
				v2 = v2.Normalize();
				waterRotate_ = std::acosf(v2.x);
				if (v2.y < 0) {
					waterRotate_ = 6.28f - waterRotate_;
				}

				if (waterGravityPos_.x == 0.0f && waterGravityPos_.y == 0.0f) {
					waterGravityPos_ = position;
				}
				else {
					waterGravityPos_ = (position + waterGravityPos_) * 0.5f;

				}
			}
		}
		else {
			rideInWater_.isRideInWater = true;
		}

		//水にはいいた瞬間の処理
		if (!preIsInWater_&& isInWater_ && !isFollowWater_) {

			if (isSpawnEffect_) {
				isSpawnEffect_ = false;
				spawnWaitCount_ = (float)maxSpawnWaitCount_;

				Vector3 pos = player_->GetPosition() - model_->transform_.translate_;

				Vector2 playerV2 = { model_->transform_.translate_.x,model_->transform_.translate_.y };
				effeEnterW_->SpawnEffect(playerV2, Vector2{ pos.x, pos.y } *spawnEffectVelo_, { player_->GetPosition().x,player_->GetPosition().y });
			}
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
	globalVariable_->AddItem("コンボの最大数", 3, "コンボ関係");

	for (int i = 0; i < kFloatEnd; i++) {
		if (i >= kFlyTime) {
			globalVariable_->AddItem(fNames[i], fParas_[i], "テンション関係");
		}
		else {
			globalVariable_->AddItem(fNames[i], fParas_[i]);
		}
	}
	globalVariable_->AddItem("浮き具合", float(0.0f));

	ApplyGlobalVariable();
}

void Baby::ApplyGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		if (i >= kFlyTime) {
			fParas_[i] = globalVariable_->GetFloatValue(fNames[i], "テンション関係");
		}
		else {
			fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);
		}
	}

	if (fParas_[FloatParamater::kMaxPlayerLength] == 0.0f) {
		fParas_[FloatParamater::kMaxPlayerLength] = 0.01f;
	}
	combo_.maxNum = globalVariable_->GetIntValue("コンボの最大数", "コンボ関係");

	Vector2 scale = globalVariable_->GetVector2Value("スケール");
	model_->transform_.scale_ = { scale.x,scale.y,1.0f };
	model_->transform_.UpdateMatrix();

	baby_->transform_.translate_.y = globalVariable_->GetFloatValue("浮き具合");
	baby_->Update();
	//spawnEffectVelo_ = fParas_[kEffectEnterWaterVelo];
	//maxSpawnWaitCount_ = fParas_[kEffectSpawwnInterval];
}

void Baby::OutWaterUpdate(const float& deltaTime)
{
	Vector3 vect = player_->GetPosition() - model_->transform_.translate_;
	vect.z = 0.0f;

	float length = vect.Length();

	if (length >= fParas_[FloatParamater::kLimitePlayerLength]) {
		// 無理やり引っ張る処理
		PulledUpdate(vect, length);
	}
	else {
		length = std::clamp(length, 0.0f, fParas_[FloatParamater::kMaxPlayerLength]);
		float t = length / fParas_[FloatParamater::kMaxPlayerLength];

		if (isFollowWater_) {
			Vector2 vect1 = { vect.x,vect.y };
			vect1 = vect1.Normalize();

			Vector2 vect2 = waterPos_ - Vector2{ model_->transform_.translate_.x,model_->transform_.translate_.y };
			vect2 = vect2.Normalize();

			float outer = Calc::Outer(vect2, vect1);

			if (isCircleWater_) {
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

				float dot = Calc::Dot(vect2, vect1);
				if (dot <= 0) {
					isFollowWater_ = false;
					float speed = t * fParas_[FloatParamater::kMaxAcceleration] * deltaTime;
					velocity_ += vect.Normalize() * speed;
					speed_ = std::clamp(velocity_.Length(), 0.0f, fParas_[FloatParamater::kMaxSpeed] * deltaTime);
					velocity_ = velocity_.Normalize() * speed_;
					model_->transform_.translate_ += velocity_;
				}
				else {
					float speed = t * fParas_[FloatParamater::kMaxSlideLength] * deltaTime;
					vect = velocity_ + vect.Normalize() * speed;
					speed_ = std::clamp(vect.Length(), 0.0f, fParas_[FloatParamater::kMaxSpeed] * deltaTime);
					vect = vect.Normalize() * speed_;

					Vector3 po = Calc::ClosestPoint(vect, Line{ Vector3{},Vector3{endPos_.x - startPos_.x,endPos_.y - startPos_.y,0.0f} });
					po.x += waterPos_.x;
					po.y += waterPos_.y;

					if ((po.x > startPos_.x && po.x > endPos_.x) || (po.x < startPos_.x && po.x < endPos_.x)) {
						isFollowWater_ = false;
						velocity_ = vect;
						model_->transform_.translate_ += velocity_;
					}
					else {
						Vector3 ppo = Calc::ClosestPoint(Vector3{ player_->GetPosition().x - startPos_.x,player_->GetPosition().y - startPos_.y,0.0f }, Line{ Vector3{startPos_.x,startPos_.y,0.0f},Vector3{endPos_.x - startPos_.x,endPos_.y - startPos_.y,0.0f} });
						ppo.x += startPos_.x;
						ppo.y += startPos_.y;
						if (((ppo.x > startPos_.x && waterPos_.x > ppo.x && po.x < ppo.x) || (ppo.x < startPos_.x && waterPos_.x < ppo.x && po.x > ppo.x) ||
							(ppo.x > startPos_.x && waterPos_.x < ppo.x && po.x > ppo.x) || (ppo.x < startPos_.x && waterPos_.x > ppo.x && po.x < ppo.x)) &&
							speed_ <= 0.05f) {

							waterPos_ = Vector2{ ppo.x,ppo.y };
							waterGravityPos_ = waterPos_;
							float l1 = (waterPos_ - startPos_).Length();
							float t2 = l1 / (endPos_ - startPos_).Length();
							waterRadius_ = (1.0f - t2) * startScale_ + t2 * endScale_;

							Vector3 v = { std::cosf(waterRotate_),std::sinf(waterRotate_), 0.0f };
							po = Vector3{ ppo.x,ppo.y,model_->transform_.translate_.z } + v * (waterRadius_ + 0.01f);
							velocity_ = {};
							speed_ = 0.0f;
							model_->transform_.translate_ = po;
						}
						else {
							waterPos_ = Vector2{ po.x,po.y };
							waterGravityPos_ = waterPos_;
							float l1 = (waterPos_ - startPos_).Length();
							float t2 = l1 / (endPos_ - startPos_).Length();
							waterRadius_ = (1.0f - t2) * startScale_ + t2 * endScale_;

							Vector3 v = { std::cosf(waterRotate_),std::sinf(waterRotate_), 0.0f };
							po = Vector3{ po.x,po.y,model_->transform_.translate_.z } + v * (waterRadius_ + 0.01f);
							velocity_ = po - model_->transform_.translate_;

							Vector2 in = input->GetGamePadLStick();
							if (in.x == 0.0f && in.y == 0.0f) {
								velocity_ *= 0.8f;
							}
							speed_ = velocity_.Length();
							model_->transform_.translate_ = po;
						}
					}
				}
			}
		}
		else {
			float speed = t * fParas_[FloatParamater::kMaxAcceleration] * deltaTime;
			vect = vect.Normalize();
			velocity_ += vect * speed;
			speed_ = std::clamp(velocity_.Length(), 0.0f, fParas_[FloatParamater::kMaxSpeed] * deltaTime);
			velocity_ = velocity_.Normalize() * speed_;
			Vector2 vector = gravityAreaSearch_->GetNearPos() - Vector2{ model_->transform_.translate_.x,model_->transform_.translate_.y };
			vector = vector.Normalize() * fParas_[kGravityWater] * deltaTime + Vector2{ vect.x,vect.y } *fParas_[kGravityPlayer] * deltaTime;
			velocity_.x += vector.x;
			velocity_.y += vector.y;
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
		PulledUpdate(vect, length);
	}
	else {
		if (isCircleWater_) {
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
		else {

			length = std::clamp(length, 0.0f, fParas_[FloatParamater::kMaxPlayerLength]);
			float t = length / fParas_[FloatParamater::kMaxPlayerLength];

			Vector3 vect2 = { std::cosf(waterRotate_),std::sinf(waterRotate_) };

			velocity_ += vect.Normalize() * t * fParas_[FloatParamater::kMaxAcceleration] * deltaTime;
			velocity_ += vect2 * fParas_[kBuoyancy] * deltaTime;

			speed_ = std::clamp(velocity_.Length(), 0.0f, fParas_[FloatParamater::kMaxSpeed] * deltaTime);
			velocity_ = velocity_.Normalize() * speed_;
			model_->transform_.translate_ += velocity_;

			vect = { endPos_.x - startPos_.x, endPos_.y - startPos_.y, 0.0f };
			Vector3 point = Calc::ClosestPoint({ model_->transform_.translate_.x,model_->transform_.translate_.y,0.0f }, Segment{ {startPos_.x,startPos_.y,0.0f},vect });
			Vector2 position = { point.x,point.y };
			waterPos_ = position;
			waterGravityPos_ = position;

			t = (position - startPos_).Length() / (endPos_ - startPos_).Length();
			waterRadius_ = (1.0f - t) * startScale_ + t * endScale_;

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
}

void Baby::InitializeGlobalVariable()
{
	fParas_.resize(kFloatEnd);

	fNames.resize(kFloatEnd);
	fNames = {
		"加速度が最大になるときのプレイヤーとの距離",
		"プレイヤーとの限界距離",
		"プレイヤーのジャンプに引き寄せられる距離",
		"プレイヤーのジャンプに引き寄せられる最大の時間",
		"プレイヤーが水中にいる時の引っぱりの倍率",
		"プレイヤーに乗って水中にいられる時間",
		"プレイヤーに乗って水中に潜れるスピード",
		"加速度の最大",
		"最大速度",
		"最低速度",
		"水の浮力",
		"プレイヤーに対しての重力加速度",
		"プレイヤーのジャンプに引っ張られる距離",
		"プレイヤーのジャンプに引っ張られる加速度",
		"重力加速度",
		"加速度が最大の時の移動角度",
		"加速度が最大の時の水の移動距離",
		"エフェクトの初速度への微調整用乗算",
		"エフェクトの発生間隔",

		"空中のテンションアップするまで時間",
		"空中のテンションアップの数値",
		"プレイヤーが水中にいる時にテンションダウンの数値",
		"プレイヤーが水中にいる時にテンションが下がる間隔の時間",
		"泣き止むまでの時間",
		"泣き止んだ時のテンション",
		"水から出たときのテンションアップの数値",
		"水から出たときのテンションアップするまでの水中の時間",
		"テンションマックス維持の時間",
		"テンションマックスが終了したときのテンション",
		"プレイヤーに乗ったときのテンションアップの数値",
	};
}

void Baby::YarnUpdate()
{
	Vector3 vect = player_->GetPosition() - model_->transform_.translate_;
	vect.z = 0.0f;

	float length = vect.Length();
	vect = vect.Normalize();
	yarn_->transform_.scale_.x = length / 2;
	yarn_->transform_.rotate_.z = std::acos(vect.x);
	if (vect.y < 0.0f) {
		yarn_->transform_.rotate_.z = 6.28f - yarn_->transform_.rotate_.z;
	}
	vect = (player_->GetPosition() + model_->transform_.translate_) / 2;
	yarn_->transform_.translate_.x = vect.x;
	yarn_->transform_.translate_.y = vect.y;
	yarn_->Update();
}

void Baby::PulledUpdate(const Vector3& vect, const float& length)
{
	velocity_ = vect.Normalize() * (length - fParas_[FloatParamater::kLimitePlayerLength] + 0.01f);
	model_->transform_.translate_ += velocity_;
}

void Baby::TensionInitialize()
{
	tension_.cryTime = 0.0f;
	tension_.face = Face::kNormal;
	tension_.oldFace = Face::kNormal;
	tension_.flyTime = 0.0f;
	tension_.playerInWaterTime = 0.0f;
	tension_.inWaterTime = 0.0f;
	tension_.tension = 50.0f;
	tension_.superTime = 0.0f;
}

void Baby::TensionUpdate(const float& deltaTime)
{
	float tension = 0.0f;
	if (preIsInWater_) {
		tension_.inWaterTime += deltaTime;
		tension_.flyTime = 0.0f;
	}
	else {
		if (tension_.tension > 0.0f && tension_.inWaterTime >= fParas_[FloatParamater::kInWaterTime]) {
			tension += fParas_[FloatParamater::kUpTensionOutWater];
		}
		tension_.inWaterTime = 0.0f;
	}

	if (player_->GetPreInWater() && !rideInWater_.isRideInWater) {
		tension_.playerInWaterTime += deltaTime;
		if (tension_.playerInWaterTime >= fParas_[FloatParamater::kPlayerInWaterTime]) {
			tension_.tension -= fParas_[FloatParamater::kDownTensionBePlayerInWater];
			tension_.playerInWaterTime = std::fmodf(tension_.playerInWaterTime, fParas_[FloatParamater::kPlayerInWaterTime]);
		}
	}
	else {
		tension_.playerInWaterTime = 0.0f;
	}

	if (isRide_ && !tension_.isRideUp_) {
		tension_.isRideUp_ = true;
		tension += fParas_[FloatParamater::kUpTentionRide];
		HitStop::SetHitStop(HitStopType::kBabyCatch);
	}
	else if (!isRide_) {
		tension_.isRideUp_ = false;
	}
	else if (isRide_ && !preIsInWaterPlayer_ && prePreIsInWaterPlayer_) {
		combo_.num++;
		if (combo_.num > combo_.maxNum) {
			combo_.num = combo_.maxNum;
		}
		tension += fParas_[FloatParamater::kUpTensionOutWater];
		if (combo_.num == combo_.maxNum) {
			HitStop::SetHitStop(HitStopType::kBabyJump);
		}
		else {
			HitStop::SetHitStop(HitStopType::kBabyComb);
		}
		// comboUIの生成
		ComboEffectManager::GetInstance()->Create(baby_->transform_.GetWorldPosition());
	}

	if (tension_.tension <= 0.0f) {
		tension_.cryTime += deltaTime;
		if (tension_.cryTime >= fParas_[FloatParamater::kCryTime]) {
			tension_.cryTime = 0.0f;
			tension_.tension = fParas_[FloatParamater::kResetTensionFromCry];
		}
	}
	else {
		tension_.tension += tension;
		if (tension != 0.0f) {
			tensionEffectManager_->CreateEffect(tension);
		}
		tension_.cryTime = 0.0f;
	}

	TensionFaceUpdate();

	tension_.tension = std::clamp(tension_.tension, 0.0f, 100.0f);
	
}

void Baby::TensionFaceUpdate() {

	if (tension_.tension <= 0.0f) {
		tension_.face = Face::kCry;
	}
	else if ((player_->GetPreInWater() && !rideInWater_.isRideInWater) || tension_.tension < 30.0f) {
		tension_.face = Face::kAnxiety;
	}
	else if (tension_.tension >= 90.0f) {
		tension_.face = Face::kSuperSmile;
	}
	else if (tension_.tension >= 80.0f) {
		tension_.face = Face::kSmile;
	}
	else {
		tension_.face = Face::kNormal;
	}

	if (tension_.face != tension_.oldFace) {
		// 前の顔と違ったらse流す
		se_[tension_.oldFace].Stop();
		se_[tension_.face].Play();
	}
	tension_.oldFace = tension_.face;
}

void Baby::RideInWaterInitialize()
{
	rideInWater_.isRideInWater = false;
	rideInWater_.rideFinishSpeed = 0.0f;
	rideInWater_.rideFinishTime = 0.0f;
}

void Baby::RideUpdate(const float& deltaTime)
{
	if (isRide_) {
		RideUpdate2(deltaTime);
	}
	else {
		Vector3 pos = player_->GetPosition();
		Vector3 v = pos - model_->transform_.translate_;
		v.z = 0.0f;
		v = v.Normalize();
		float s = velocity_.Length();
		velocity_ = v * (s + fParas_[kNearPulledPlayerGravity] * deltaTime);
		model_->transform_.translate_ += velocity_;
		Vector3 v2 = pos - model_->transform_.translate_;
		v2.z = 0.0f;
		float l = v2.Length();
		v2 = v2.Normalize();

		if (Calc::Dot(v, v2) < 0.0f || l <= fParas_[kNearPlayerLength]) {
			isRide_ = true;
			playerOutTime_ = 0.0f;
			RideUpdate2(deltaTime);
		}
	}
	model_->Update();
}

void Baby::RideUpdate2(const float& deltaTime)
{
	model_->transform_.translate_ = player_->GetPosition();
	model_->transform_.rotate_.z = player_->GetRotate().z;
	rideInWater_.rideFinishSpeed = fParas_[kRideInWaterSpeed] * deltaTime;
	if (rideInWater_.isRideInWater) {
		rideInWater_.rideFinishTime += deltaTime;
	}
	else {
		rideInWater_.rideFinishTime = 0.0f;
	}
	playerOutTime_ = std::clamp(playerOutTime_ + deltaTime, 0.0f, fParas_[kNearPlayerTime]);
}

void Baby::TextureUpdate() {
	uint32_t faceIndex = static_cast<uint32_t>(tension_.face);
	if (!isInWater_ && !isFollowWater_) {
		faceIndex += kMaxFacePattern;
	}
	if (faceIndex == texturePath.size()) {
		faceIndex -= kMaxFacePattern;
	}
	baby_->SetTexture(TextureManager::GetInstance()->LoadTexture(directryPath + texturePath.at(faceIndex)));
}
