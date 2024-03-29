#include "Planet.h"
#include "ModelDataManager.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "PlanetTypeColor.h"
#include "GameElement/Player/Player.h"
#include "RandomGenerator/RandomGenerator.h"

InstancingModelManager* Planet::instancingManager_ = nullptr;
const ModelData* Planet::modelData_ = nullptr;
RandomGenerator* Planet::rand_ = nullptr;

std::unique_ptr<GlobalVariableUser> Planet::staticGlobalVariable_ = nullptr;
Vector2 Planet::minmax_ = { 2.0f,4.0f };
int Planet::MaxClientNum = 6;

Planet::Planet(PlanetType type, const Vector3& pos, Player* player, int no)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::PLANET);
	Collider::AddTargetMask(ColliderMask::PLAYER);
	Collider::AddTargetMask(ColliderMask::WATER);

	gravityArea_ = std::make_unique<GravityArea>();

	type_ = type;
	player_ = player;
	position_ = pos;
	scale_ = 2.0f;
	rotate_ = 0.0f;

	ganerateTime_ = rand_->RandFloat(0.5f, 1.5f);
	time_ = 0.0f;
	
	isPos.resize(MaxClientNum);
	for (int i = 0; i < MaxClientNum; i++) {
		isPos[i] = false;
	}

	no_ = no;
	globalVariable_ = std::make_unique<GlobalVariableUser>("Planet", "Planets");

	SetGlobalVariable();
}

void Planet::StaticInitialize()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	modelData_ = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
	rand_ = RandomGenerator::GetInstance();

	staticGlobalVariable_ = std::make_unique<GlobalVariableUser>("Planet", "StaticPlanet");
	StaticSetGlobalVariable();
}

void Planet::Initialize()
{
}

void Planet::Update(float deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG

	time_ += deltaTime;
	CreateClient();
	gravityArea_->Update({ position_.x,position_.y }, { scale_,scale_ });
	SetCollider();
}

void Planet::StaticUpdate()
{
	StaticApplyGlobalVariable();
}

void Planet::Draw() const
{
#ifdef _DEBUG
	gravityArea_->Draw({ position_.x,position_.y }, { scale_,scale_ });
#endif // _DEBUG
	for (const std::unique_ptr<Client>& client : clients_) {
		client->Draw();
	}

	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
	instancingManager_->AddBox(modelData_, InstancingModel{ matrix,PlanetTypeColor::GetColor(type_) });
}

void Planet::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::PLAYER) {
		player_->OnCollisionPlanet(type_, clients_);
		for (int i = 0; i < MaxClientNum; i++) {
			isPos[i] = false;
		}
	}
	else if (collider.GetMask() == ColliderMask::WATER) {

	}
}

void Planet::SetCollider()
{
	Collider::SetCircle({ position_.x,position_.y }, scale_);
	collisionManager_->SetCollider(this);
}

void Planet::SetGlobalVariable()
{
	globalVariable_->AddItem("ポジション", position_, "Planet" + std::to_string(no_));
	globalVariable_->AddItem("スケール", scale_, "Planet" + std::to_string(no_));
	ApplyGlobalVariable();
}

void Planet::ApplyGlobalVariable()
{
	position_ = globalVariable_->GetVector3Value("ポジション", "Planet" + std::to_string(no_));
	scale_ = globalVariable_->GetFloatValue("スケール", "Planet" + std::to_string(no_));
}

void Planet::StaticSetGlobalVariable()
{
	staticGlobalVariable_->AddItem("客の生成時間の最小と最大", minmax_);
	staticGlobalVariable_->AddItem("惑星に生成される客の最大数", MaxClientNum);
	StaticApplyGlobalVariable();
}

void Planet::StaticApplyGlobalVariable()
{
	minmax_ = staticGlobalVariable_->GetVector2Value("客の生成時間の最小と最大");
	MaxClientNum = staticGlobalVariable_->GetIntValue("惑星に生成される客の最大数");
	if (MaxClientNum <= 0) {
		MaxClientNum = 1;
	}
}

void Planet::CreateClient()
{
	if (time_ >= ganerateTime_) {
		time_ = 0.0f;
		if (minmax_.x < minmax_.y) {
			ganerateTime_ = rand_->RandFloat(minmax_.x, minmax_.y);
		}
		else if (minmax_.x > minmax_.y) {
			ganerateTime_ = rand_->RandFloat(minmax_.y, minmax_.x);
		}
		else {
			ganerateTime_ = minmax_.x;
		}

		if (MaxClientNum != static_cast<int>(isPos.size())) {
			isPos.resize(MaxClientNum);
			for (int i = 0; i < MaxClientNum; i++) {
				isPos[i] = false;
			}
		}

		while (true)
		{
			PlanetType type = static_cast<PlanetType>(rand_->RandInt(0, static_cast<int>(PlanetType::kEnd)));

			if (type != type_) {
				for (int i = 0; i < MaxClientNum; i++) {
					if (!isPos[i]) {
						float theta = 1.57f - float(i) / MaxClientNum * 6.28f;
						Vector3 pos{};
						pos.x = (scale_ + clientScale_) * std::cosf(theta);
						pos.y = (scale_ + clientScale_) * std::sinf(theta);
						pos += position_;
						clients_.push_back(std::make_unique<Client>(type, pos, clientScale_));
						isPos[i] = true;
						break;
					}
				}
				break;
			}
		}
	}
}
