#include "Planet.h"
#include "ModelDataManager.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "PlanetTypeColor.h"
#include "GameElement/Player/Player.h"
#include "RandomGenerator/RandomGenerator.h"
#include "GameElement/Client/ClientManager.h"

InstancingModelManager* Planet::instancingManager_ = nullptr;
const ModelData* Planet::modelData_ = nullptr;
RandomGenerator* Planet::rand_ = nullptr;
ClientManager* Planet::clientManager_ = nullptr;

std::unique_ptr<GlobalVariableUser> Planet::staticGlobalVariable_ = nullptr;
int Planet::MaxClientNum = 6;

Planet::Planet(PlanetType type, const Vector3& pos, Player* player, int no)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::PLANET);
	Collider::AddTargetMask(ColliderMask::PLAYER);
	Collider::AddTargetMask(ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::CLIENT);

	gravityArea_ = std::make_unique<GravityArea>();

	type_ = type;
	player_ = player;
	position_ = pos;
	scale_ = 2.0f;
	rotate_ = 0.0f;
	
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
	clientManager_ = ClientManager::GetInstance();

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
	deltaTime = deltaTime;
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
	else if (collider.GetMask() == ColliderMask::CLIENT) {

		PlanetType type = clientManager_->GetHitClientType();
		if (type_ != type) {
			Vector2 pos = collider.GetCircle()->position_;
			clients_.push_back(std::make_unique<Client>(type, Vector3{ pos.x,pos.y,0.0f }));
		}
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
	staticGlobalVariable_->AddItem("惑星に生成される客の最大数", MaxClientNum);
	StaticApplyGlobalVariable();
}

void Planet::StaticApplyGlobalVariable()
{
	MaxClientNum = staticGlobalVariable_->GetIntValue("惑星に生成される客の最大数");
	if (MaxClientNum <= 0) {
		MaxClientNum = 1;
	}
}

void Planet::CreateClient()
{
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
					float scale = Client::GetScale();
					pos.x = (scale_ + scale) * std::cosf(theta);
					pos.y = (scale_ + scale) * std::sinf(theta);
					pos += position_;
					clients_.push_back(std::make_unique<Client>(type, pos));
					isPos[i] = true;
					break;
				}
			}
			break;
		}
	}
}
