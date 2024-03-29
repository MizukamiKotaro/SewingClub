#include "Client.h"
#include "ModelDataManager.h"
#include "GameElement/Planet/PlanetTypeColor.h"

InstancingModelManager* Client::instancingManager_ = nullptr;
const ModelData* Client::modelData_ = nullptr;
std::unique_ptr<GlobalVariableUser> Client::globalVariable_ = nullptr;

Client::Client(PlanetType type, const Vector3& pos, float scale)
{
	type_ = type;
	position_ = pos;
	scale_ = scale;
	rotate_ = 0.0f;

	sprite_ = std::make_unique<Sprite>("white.png");
	sprite_->size_ = { 40.0f,40.0f };
}

void Client::StaticInitialize()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	modelData_ = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
	globalVariable_ = std::make_unique<GlobalVariableUser>("Charactor", "Client");
	globalVariable_->CreateGroup();

	SetGlobalVariable();
}

void Client::Initialize()
{
}

void Client::Update(float deltaTime)
{
	deltaTime = deltaTime;
}

void Client::Draw() const
{
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
	instancingManager_->AddBox(modelData_, InstancingModel{ matrix,PlanetTypeColor::GetColor(type_) });
}

void Client::Draw(const Vector2& pos) const
{
	sprite_->pos_ = pos;
	sprite_->SetColor(PlanetTypeColor::GetColor(type_));
	sprite_->Update();
	sprite_->Draw();
}

void Client::StaticUpdate()
{
	ApplyGlobalVariable();
}

void Client::SetGlobalVariable()
{
}

void Client::ApplyGlobalVariable()
{
}
