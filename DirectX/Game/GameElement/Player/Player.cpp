#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "calc.h"
#include <algorithm>
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "GameElement/WaterManager/WaterManager.h"
#include "GameElement/Client/ClientManager.h"
#include"GameElement/Effects/EffectOutWater.h"
#include "GameElement/Animation/AnimationManager.h"
#include "TextureManager/TextureManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include <numbers>

const float kRotate = 90.0f * (std::numbers::pi_v<float> / 180.0f);

Player::Player()
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::RIGID_BODY, ColliderMask::PLAYER);
	Collider::AddTargetMask(ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::GRAVITY_AREA);
	Collider::AddTargetMask(ColliderMask::ENEMY);
	//Collider::AddTargetMask(ColliderMask::ITEM);

	CreateGlobalVariable("Player");

	input_ = Input::GetInstance();
	waterManager_ = WaterManager::GetInstance();

	InitializeGlobalVariable();

	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = 0.5f;
	}
	for (int i = 0; i < kBoolEnd; i++) {
		bParas_[i] = false;
	}

	velocity_ = {};
	vector_ = { 0.0f,1.0f };
	speed_ = 0.0f;
	addAcceleration_ = 0.0f;
	timeCount_ = 0.0f;
	gravityVelocity_ = {};
	gravityPos_ = {};
	isGravity_ = false;
	coolTimeCount_ = 0.0f;
	isUseInputAcceleration_ = true;
	accelerationTimeCount_ = 0.0f;
	isInputAcceleration_ = false;
	waterRecoveryTimeCount_ = 0.0f;

	memoOutWaterSpeed_ = 0.0f;
	isHitEnemy_ = false;

	waterGravityPos_ = {};

	model_ = std::make_unique<Model>("plane");
	model_->transform_.scale_ = { 0.4f,0.2f,0.2f };

	stageEditor_ = std::make_unique<StageEditor>("プレイヤーの設定");
	SetGlobalVariable();
	model_->Update();

	floatingParameter_ = 0.0f;
	isInWater_ = true;
	preIsInWater_ = true;

	gravityAreaSearch_ = std::make_unique<GravityAreaSearch>();

	seIn2Water_.LoadWave("SE/inToWater.wav","水に入る音");
	seOutWater_.LoadWave("SE/outWater.wav","水から出る音");
	seStayWater_.LoadWave("SE/inWater.wav","水の中にいる音");

	effeExtraJump_ = std::make_unique<EffectExtraJump>();
	effectOutWater_ = std::make_unique<EffectOutWater>();
	effeEnterWater_ = std::make_unique<EffectEnterWater>();
	effeUIEnterWater_ = std::make_unique<EffectUIEnterWater>("IngameUIEnterWater");
	particleScceleration_ = std::make_unique<ParticleAcceleration>();
	
	// アニメーションの初期化とモデルのセット
	animation_ = std::make_unique<Animation2D>(AnimationManager::GetInstance()->AddAnimation("playermove"));
	// UV座標のセット
	Transform handle = animation_->GetSceneUV(0u);
	model_->SetUVParam(handle);
	model_->SetTexture(TextureManager::GetInstance()->LoadTexture("player_anime.png"));
	animation_->Play(true);
}

void Player::Initialize()
{

	stageEditor_->Initialize();
	Reset();
	effeExtraJump_->Initialize(&model_->transform_.GetWorldPosition());
	effectOutWater_->Initialize();
	effeEnterWater_->Initialize();
	effeUIEnterWater_->Initialize();
	particleScceleration_->Initialze(&model_->transform_.translate_);
	//debugyou
	particleScceleration_->IsActive(true,10.0f);

	//水の中のUI演出を実行
	effeUIEnterWater_->IsEffectActive(true);
	//音も再生
	seStayWater_.Play();
}

void Player::Update(float deltaTime)
{
#ifdef _DEBUG
	if (stageEditor_->IsChangedStage()) {
		Initialize();
	}
	ApplyGlobalVariable();
	effeExtraJump_->Debug();
	effectOutWater_->Debug();
#endif // _DEBUG
	if (isInWater_ && preIsInWater_) {
		// 水や惑星内での更新処理
		Move(deltaTime);
	}
	else if (!isInWater_ && preIsInWater_) {
		// 水や惑星から飛び出たときの処理
		PopUpFromWater();
	}
	else if (isInWater_ && !preIsInWater_) {
		// 水や惑星に入ったときの処理
		ComeToWater();
	}
	else {
		// 水や惑星の外での処理
		OutWater(deltaTime);
	}
	// 入力による加速の更新処理
	UpdateInputAcceleration(deltaTime);

	preIsInWater_ = isInWater_;

	std::string animationHandle;
	if (velocity_.y != 0.0f) {
		animationHandle = "playermove";
	}
	else {
		animationHandle = "playeridle";
	}
	// アニメーションがされていたら
	if (animation_->Update(animationHandle)) {
		// modelにuvのセット
		model_->SetUVParam(animation_->GetUVTrans());
	}


#pragma region 加速エフェクトの表示
	bool effectActive = false;
	float leng = velocity_.Length();
	if (leng >= 0.4f) {
		effectActive = true;
	}

	particleScceleration_->IsActive(effectActive, velocity_.Length());
#pragma endregion



	model_->Update();

	isInWater_ = false;
	isGravity_ = false;
	gravityVelocity_ = {};
	waterGravityPos_ = {};
	SetCollider();

	effeExtraJump_->Update();
	effectOutWater_->Update();
	effeEnterWater_->Update();
	effeUIEnterWater_->Update();
	particleScceleration_->Update(vector_);
}

void Player::Draw(const Camera* camera)
{
	effeExtraJump_->Draw();
	particleScceleration_->Draw();
	model_->Draw(*camera);
	//yarn_->Draw();
}

void Player::EffectDraw()
{
	
	effectOutWater_->Draw();
	effeEnterWater_->Draw();
}

void Player::DrawUI()
{
	effeUIEnterWater_->Draw();
}

void Player::Finalize()
{
	seIn2Water_.Stop();
	seOutWater_.Stop();
	seStayWater_.Stop();
}

const Vector3& Player::GetPosition() const
{
	return model_->transform_.GetWorldPosition();
}

const Vector3* Player::GetPositionPtr() const
{
	return &model_->transform_.GetWorldPosition();
}

void Player::Move(float deltaTime)
{
	timeCount_ += deltaTime;

	Vector2 vector = input_->GetGamePadLStick();

	if (vector.x != 0.0f || vector.y != 0.0f) {
		// 入力があったときの処理
		vector = vector.Normalize();

		if (vector_.x != -vector.x) {
			vector_ = Calc::Lerp(vector_, vector, fParas_[kInterpolationRateInWater]).Normalize();
		}
		else {
			// 真逆の時の細かい修正保留
			vector_ = Calc::Lerp(vector_, vector, fParas_[kInterpolationRateInWater]).Normalize();
		}

		speed_ = std::clamp(speed_ + fParas_[kAcceleration] * deltaTime, fParas_[kMinSpeed] * deltaTime, fParas_[kMaxSpeed] * deltaTime + addAcceleration_);

		velocity_ = { vector_.x * speed_,vector_.y * speed_,0.0f };

		if (vector_.y >= 0.0f) {
			model_->transform_.rotate_.z = std::acosf(vector_.x) - 1.57f + kRotate;
		}
		else {
			model_->transform_.rotate_.z = -std::acosf(vector_.x) - 1.57f + kRotate;
		}

		
	}
	else {
		// 入力がなかった時の処理
		if (fParas_[kAttenuation] != 0.0f) {
			speed_ = speed_ * fParas_[kAttenuation];
		}

		if (speed_ >= fParas_[kMinSpeed] * deltaTime) {
			velocity_ = { vector_.x * speed_,vector_.y * speed_,0.0f };
		}
		else {
			velocity_ = {};
			speed_ = 0.0f;
		}

	}

	if (timeCount_ >= fParas_[kKeepSpeedTime]) {
		addAcceleration_ *= 0.92f;
		if (addAcceleration_ <= 0.3f) {
			addAcceleration_ = 0.0f;
		}
	}

	if (bParas_[kIsBuoyancy]) {
		Vector3 vect = model_->transform_.translate_ - Vector3{ waterGravityPos_.x,waterGravityPos_.y,0.0f };
		velocity_ += vect * fParas_[kBuoyancy] * deltaTime;
	}

	model_->transform_.translate_ += velocity_;

	
}

void Player::PopUpFromWater()
{
	seOutWater_.Play();
	seStayWater_.Stop();
	timeCount_ = 0.0f;
	memoOutWaterSpeed_ = speed_;
	model_->transform_.translate_ += velocity_;

	Vector2 gvelo =  Vector2(model_->transform_.translate_.x, model_->transform_.translate_.y)- gravityPos_;

	//各エフェクト発生処理
	//みずしぶき
	effectOutWater_->SpawnEffect(Vector2(model_->transform_.translate_.x, model_->transform_.translate_.y), Vector2{ velocity_.x,velocity_.y },normalJumpEffectNum_);
	//軌道エフェクト
	effeExtraJump_->SpawnEffect(true);


	effeUIEnterWater_->IsEffectActive(false);
}

void Player::ComeToWater()
{
	seIn2Water_.Play();
	seStayWater_.Play(true);
	timeCount_ = 0.0f;
	model_->transform_.translate_ += velocity_;

	effeEnterWater_->SpawnEffect(Vector2{ model_->transform_.translate_.x,model_->transform_.translate_.y }, Vector2{ velocity_.x,velocity_.y }, gravityPos_);

	effeExtraJump_->SpawnEffect(false);

	effeUIEnterWater_->IsEffectActive(true);
}

void Player::OutWater(float deltaTime)
{
	timeCount_ += deltaTime;

	if (timeCount_ <= fParas_[kOutWaterTime]) {
		if (addAcceleration_ >= fParas_[FloatParamater::kMaxAddAcceleration] * deltaTime) {
			addAcceleration_ = fParas_[FloatParamater::kMaxAddAcceleration] * deltaTime;
		}
		else {
			float addSpeed = fParas_[kOutWaterAcceleration] * deltaTime;
			speed_ += addSpeed;
			addAcceleration_ += addSpeed;
		}
	}
	speed_ = std::clamp(speed_, fParas_[kMinSpeed] * deltaTime, fParas_[kMaxSpeed] * deltaTime + addAcceleration_);
	velocity_ = { vector_.x * speed_,vector_.y * speed_ ,0.0f };
	if (bParas_[BoolParamater::kGravityArea]) {

		if (isGravity_) {
			gravityVelocity_ = gravityVelocity_.Normalize();
			velocity_.x += gravityVelocity_.x * deltaTime;
			velocity_.y += gravityVelocity_.y * deltaTime;
		}
		else {
			if (bParas_[BoolParamater::kGravityAreaSearch]) {
				Vector2 vector = gravityAreaSearch_->GetNearPos() - Vector2{ model_->transform_.translate_.x,model_->transform_.translate_.y };
				gravityVelocity_ = vector.Normalize() * fParas_[kGravityWater];
				velocity_.x += gravityVelocity_.x * deltaTime;
				velocity_.y += gravityVelocity_.y * deltaTime;
			}
			else {
				Vector2 vector = gravityPos_ - Vector2{ model_->transform_.translate_.x,model_->transform_.translate_.y };
				gravityVelocity_ = vector.Normalize() * fParas_[kGravityWater];
				velocity_.x += gravityVelocity_.x * deltaTime;
				velocity_.y += gravityVelocity_.y * deltaTime;
			}
		}
	}
	else {
		if (velocity_.y <= 0.0f) {
			velocity_.y -= fParas_[kGravityDown] * deltaTime;
		}
		else {
			velocity_.y -= fParas_[kGravity] * deltaTime;
		}
	}

	if (bParas_[BoolParamater::kJumpInput]) {
		Vector2 vector = input_->GetGamePadLStick();

		if (bParas_[BoolParamater::kGravityArea]) {
			if (vector.x > 0.0f) {
				velocity_.x += fParas_[FloatParamater::kJumpInputAcceleration] * deltaTime;
			}
			else if (vector.x < 0.0f) {
				velocity_.x -= fParas_[FloatParamater::kJumpInputAcceleration] * deltaTime;
			}
		}
		else {
			velocity_.x += vector.x * fParas_[FloatParamater::kJumpInputAcceleration] * deltaTime;
			velocity_.y += vector.y * fParas_[FloatParamater::kJumpInputAcceleration] * deltaTime;
		}
	}

	Vector3 normal = velocity_.Normalize();
	vector_ = { normal.x,normal.y };

		if (std::abs(normal.x) > std::abs(normal.y)) {
			speed_ = velocity_.x / normal.x;
		}
		else {
			speed_ = velocity_.y / normal.y;
		}
		if (vector_.y >= 0.0f) {
			model_->transform_.rotate_.z = std::acosf(vector_.x) - 1.57f + kRotate;
		}
		else {
			model_->transform_.rotate_.z = -std::acosf(vector_.x) - 1.57f + kRotate;
		}

	model_->transform_.translate_ += velocity_;
}

void Player::UpdateInputAcceleration(float deltaTime)
{
	if (bParas_[BoolParamater::kAccelerationInput] || bParas_[BoolParamater::kAccelerationInJump]) {
		coolTimeCount_ += deltaTime;
		if (isInputAcceleration_) {
			accelerationTimeCount_ += deltaTime;
			if (accelerationTimeCount_ <= fParas_[FloatParamater::kInputAccelerationTime]) {
				float addSpeed = 0.0f;
				if (addAcceleration_ >= fParas_[FloatParamater::kMaxAddAcceleration] * deltaTime) {
					addAcceleration_ = fParas_[FloatParamater::kMaxAddAcceleration] * deltaTime;
				}
				else {
					addSpeed = fParas_[FloatParamater::kInputAcceleration] * deltaTime;
					speed_ += addSpeed;
					addAcceleration_ += addSpeed;
				}
				Vector2 vector = vector_.Normalize();
				velocity_.x += vector.x * addSpeed;
				velocity_.y += vector.y * addSpeed;
				model_->transform_.translate_.x += vector.x * addSpeed;
				model_->transform_.translate_.y += vector.y * addSpeed;
			}
			else {
				accelerationTimeCount_ = 0.0f;
				isInputAcceleration_ = false;
			}
		}

		if (isUseInputAcceleration_ && input_->PressedGamePadButton(Input::GamePadButton::B)) {
			if ((isInWater_ && bParas_[BoolParamater::kAccelerationInput]) || (!isInWater_ && bParas_[BoolParamater::kAccelerationInJump])) {
				isUseInputAcceleration_ = false;
				isInputAcceleration_ = true;
				accelerationTimeCount_ = 0.0f;
			}
		}

		if (coolTimeCount_ >= fParas_[FloatParamater::kRecoveryInputTime]) {
			if (bParas_[BoolParamater::kRecoveryInJump]) {
				coolTimeCount_ = 0.0f;
				isUseInputAcceleration_ = true;
			}
			else {
				if (isInWater_) {
					coolTimeCount_ = 0.0f;
					isUseInputAcceleration_ = true;
				}
			}
		}

		if (isInputAcceleration_) {
			model_->color_ = { 0.9f,0.1f,0.1f,1.0f };
		}
		else {
			model_->color_ = { 1.0f,1.0f,1.0f,1.0f };
		}
	}
}

void Player::Reset()
{
	velocity_ = { 0.0f };
	vector_ = { 0.0f,1.0f };
	speed_ = 0.0f;
	addAcceleration_ = 0.0f;
	timeCount_ = 0.0f;
	gravityVelocity_ = {};
	gravityPos_ = {};
	isGravity_ = false;
	coolTimeCount_ = 0.0f;
	isUseInputAcceleration_ = true;
	accelerationTimeCount_ = 0.0f;
	isInputAcceleration_ = false;
	waterRecoveryTimeCount_ = 0.0f;

	memoOutWaterSpeed_ = 0.0f;
	isHitEnemy_ = false;
	waterGravityPos_ = {};

	model_->transform_.rotate_ = { 0.0f };
	model_->transform_.translate_ = { 0.0f };
	SetGlobalVariable();

	floatingParameter_ = 0.0f;
	isInWater_ = true;
	preIsInWater_ = true;

	model_->Update();
}

void Player::InitializeFloating()
{
	floatingParameter_ = 0.0f;
}

void Player::UpdateFloating()
{

}

void Player::InitializeGlobalVariable()
{
	fParas_.resize(kFloatEnd);
	bParas_.resize(kBoolEnd);

	fNames.resize(kFloatEnd);
	fNames = {
		"加速度",
		"減速率",
		"最大速度",
		"最低速度",
		"加算される加速度の最大値",
		"水中での補間の割合",
		"補間の割合",
		"上下挙動の1往復の時間",
		"水から飛び出したときの加速度",
		"水から飛び出したときに加速させる時間",
		"プレイヤーの最低の高さ",
		"加速を維持する時間",
		"重力加速度",
		"浮力",
		"降下中の重力",
		"水の塊の重力",
		"プレイヤーが生成する水のサイズ",
		"水の回復時間",
		"ジャンプで生成する水のサイズ",
		"水の生成に遅延させる時間",
		"ジャンプ中の入力の加速度",
		"ボタン入力による加速度",
		"ボタン加速のクールタイム",
		"ボタン入力による加速させる時間",
	};

	bNames.resize(kBoolEnd);
	bNames = {
		"水ごとに重力がありか",
		"水の浮力があるか",
		"一番近くの重力場に引き寄せられるか",
		"ボタンを押したときに水を生成するか",
		"ジャンプしたときに水を生成するか",
		"ジャンプ中に入力を受け付けるか",
		"ボタン入力で水中で加速できるか",
		"ボタン入力でジャンプ中に加速できるか",
		"ボタン入力で加速後ジャンプしたときに加速ボタンが回復するか",
	};

	tree1Name_.resize(kTree1End);
	tree1Name_ = {
		"プレイヤーのステータス関係",
		"重力関係",
		"水の生成関係",
		"入力による移動関係",
	};
	fTree1.resize(kTree1End);
	fTree1 = {
		{kAcceleration,kGravity},
		{kGravity,kWaterSize},
		{kWaterSize,kJumpInputAcceleration},
		{kJumpInputAcceleration,kFloatEnd},
	};
	bTree1.resize(kTree1End);
	bTree1 = {
		{0,0},
		{kGravityArea,kAddWaterTriger},
		{kAddWaterTriger,kJumpInput},
		{kJumpInput,kBoolEnd},
	};
}

void Player::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::WATER) {
		if (collider.GetShape() == ColliderShape::CIRCLE) {
			isInWater_ = true;
			if (bParas_[kIsBuoyancy]) {
				ShapeCircle* circle = collider.GetCircle();
				gravityPos_ = circle->position_;;
				if (waterGravityPos_.x == 0.0f && waterGravityPos_.y == 0.0f) {
					waterGravityPos_ = circle->position_;
				}
				else {
					waterGravityPos_ = (circle->position_ + waterGravityPos_) * 0.5f;
				}
			}
		}
		else if (collider.GetShape() == ColliderShape::QUADRANGLE2D) {
			isInWater_ = true;
			if (bParas_[kIsBuoyancy]) {
				ShapeQuadrangle* quadrangle = collider.GetQuadrangle();
				Vector2 startPos = (quadrangle->leftTop_ + quadrangle->leftBottom_) / 2;
				Vector2 endPos = (quadrangle->rightTop_ + quadrangle->rightBottom_) / 2;
				Vector3 vect = { endPos.x - startPos.x, endPos.y - startPos.y, 0.0f };
				Vector3 project = Calc::Project({ model_->transform_.translate_.x - startPos.x,model_->transform_.translate_.y - startPos.y,0.0f }, vect);
				Vector2 position = { startPos.x + project.x,startPos.y + project.y };
				gravityPos_ = position;
				if (waterGravityPos_.x == 0.0f && waterGravityPos_.y == 0.0f) {
					waterGravityPos_ = position;
				}
				else {
					waterGravityPos_ = (position + waterGravityPos_) * 0.5f;
				}
			}
		}
	}
	else if (collider.GetMask() == ColliderMask::GRAVITY_AREA) {
		if (collider.GetShape() == ColliderShape::CIRCLE) {
			ShapeCircle* circle = collider.GetCircle();
			Vector2 pos = { model_->transform_.translate_.x,model_->transform_.translate_.y };
			if (!isGravity_) {
				gravityPos_ = circle->position_;
				isGravity_ = true;
			}
			if ((gravityPos_ - pos).Length() > (circle->position_ - pos).Length()) {
				gravityPos_ = circle->position_;
			}

			Vector2 vector = circle->position_ - pos;
			gravityVelocity_ += vector.Normalize() * fParas_[kGravityWater];
		}
		else if (collider.GetShape() == ColliderShape::QUADRANGLE2D) {
			ShapeQuadrangle* quadrangle = collider.GetQuadrangle();
			Vector2 pos = { model_->transform_.translate_.x,model_->transform_.translate_.y };
			Vector2 startPos = (quadrangle->leftTop_ + quadrangle->leftBottom_) / 2;
			Vector2 endPos = (quadrangle->rightTop_ + quadrangle->rightBottom_) / 2;
			Vector3 vect = { endPos.x - startPos.x, endPos.y - startPos.y, 0.0f };
			Vector3 point = Calc::ClosestPoint({ model_->transform_.translate_.x,model_->transform_.translate_.y,0.0f }, Segment{ {startPos.x,startPos.y,0.0f},vect });
			Vector2 position = { point.x,point.y };
			if (!isGravity_) {
				gravityPos_ = position;
				isGravity_ = true;
			}
			if ((gravityPos_ - pos).Length() > (position - pos).Length()) {
				gravityPos_ = position;
			}

			Vector2 vector = position - pos;
			gravityVelocity_ += vector.Normalize() * fParas_[kGravityWater];
		}
	}
	else if (collider.GetMask() == ColliderMask::ENEMY) {
		isHitEnemy_ = true;
	}
}

void Player::SetCollider()
{
	Collider::SetCircle({ model_->transform_.translate_.x,model_->transform_.translate_.y },
		0.0f, 0.0f, { velocity_.x,velocity_.y });
	collisionManager_->SetCollider(this);
	if (bParas_[BoolParamater::kGravityAreaSearch]) {
		gravityAreaSearch_->Update(model_->transform_.translate_, velocity_);
	}
}

void Player::SetGlobalVariable()
{
	globalVariable_->AddItem("スケール", model_->transform_.scale_, tree1Name_[kTree1Status]);

	bool isAddF[kFloatEnd] = { false };
	for (int i = 0; i < kFloatEnd; i++) {
		for (int j = 0; j < Tree1::kTree1End; j++) {
			if (i >= fTree1[j].first && i < fTree1[j].second) {
				if (!isAddF[i]) {
					globalVariable_->AddItem(fNames[i], fParas_[i], tree1Name_[j]);
					isAddF[i] = true;
				}
			}
		}
		if (!isAddF[i]) {
			globalVariable_->AddItem(fNames[i], fParas_[i]);
			isAddF[i] = true;
		}
	}
	bool isAddB[kBoolEnd] = { false };
	for (int i = 0; i < kBoolEnd; i++) {
		for (int j = 0; j < Tree1::kTree1End; j++) {
			if (i >= bTree1[j].first && i < bTree1[j].second) {
				if (!isAddB[i]) {
					globalVariable_->AddItem(bNames[i], bParas_[i], tree1Name_[j]);
					isAddB[i] = true;
				}
			}
		}
		if (!isAddB[i]) {
			globalVariable_->AddItem(bNames[i], bParas_[i]);
			isAddB[i] = true;
		}
	}

	stageEditor_->AddItem("初期座標", model_->transform_.translate_);
	model_->transform_.translate_ = stageEditor_->GetVector3Value("初期座標");
	// 重要 X座標を上に出すための処理--------------------------------------------------------------------------------------------------------------------------------------------
	model_->transform_.translate_.z = -0.1f;
	ApplyGlobalVariable();
}

void Player::ApplyGlobalVariable()
{
	model_->transform_.scale_ = globalVariable_->GetVector3Value("スケール", tree1Name_[kTree1Status]);

	bool isAddF[kFloatEnd] = { false };
	for (int i = 0; i < kFloatEnd; i++) {
		for (int j = 0; j < Tree1::kTree1End; j++) {
			if (i >= fTree1[j].first && i < fTree1[j].second) {
				if (!isAddF[i]) {
					fParas_[i] = globalVariable_->GetFloatValue(fNames[i], tree1Name_[j]);
					isAddF[i] = true;
				}
			}
		}
		if (!isAddF[i]) {
			fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);
			isAddF[i] = true;
		}
	}
	bool isAddB[kBoolEnd] = { false };
	for (int i = 0; i < kBoolEnd; i++) {
		for (int j = 0; j < Tree1::kTree1End; j++) {
			if (i >= bTree1[j].first && i < bTree1[j].second) {
				if (!isAddB[i]) {
					bParas_[i] = globalVariable_->GetBoolValue(bNames[i], tree1Name_[j]);
					isAddB[i] = true;
				}
			}
		}
		if (!isAddB[i]) {
			bParas_[i] = globalVariable_->GetBoolValue(bNames[i]);
			isAddB[i] = true;
		}
	}
}