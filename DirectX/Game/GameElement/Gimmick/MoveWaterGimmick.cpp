#include "MoveWaterGimmick.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "GameElement/Wave/Wave.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "SceneSystem/IScene/IScene.h"
#include "WindowsInfo/WindowsInfo.h"
#include "Game/GameElement/Item/ItemManager.h"

InstancingModelManager* MoveWaterGimmick::instancingManager_ = nullptr;
const InstancingMeshTexData* MoveWaterGimmick::modelData_ = nullptr;

ItemManager* MoveWaterGimmick::itemManager_ = nullptr;

std::unique_ptr<GlobalVariableUser> MoveWaterGimmick::staticGlobalVariable_ = nullptr;

MoveWaterGimmick::MoveWaterGimmick(int no) {
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::ITEM);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	position_ = { 0.0f,1.0f,-0.02f };
	rotate_ = 0.0f;

	no_ = no;
	stageEditor_ = std::make_unique<StageEditor>("アイテムの配置");
	SetGlobalVariable();
	waterParam_.moveVector = endPoint_ - Vector2(position_.x,position_.y);
	waterParam_.moveVector = waterParam_.moveVector.Normalize() * waterParam_.waterSpeed_;
	waterParam_.respawnPoint = Vector2(position_.x, position_.y);
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	isActive_ = true;
}

void MoveWaterGimmick::StaticInitialize() {
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
	const Texture* tex = TextureManager::GetInstance()->LoadTexture("coin_1.png");
	modelData_ = instancingManager_->GetDrawData({ modelData,tex,BlendMode::kBlendModeNormal });
	itemManager_ = ItemManager::GetInstance();

}

bool MoveWaterGimmick::Update(const float& delta, Camera* camera) {
#ifdef _DEBUG
	ApplyGlobalVariable();
	Vector4 staticColor = itemManager_->GetColor();
	if (stageEditor_) {
		std::string tree = "ギミック" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "移動する水" + std::to_string(no) + "～" + std::to_string(no + 9);
		if (stageEditor_->IsTreeOpen(tree1, tree)) {
			color_ = { 1.0f,0.3f,0.3f,1.0f };
		}
		else if (stageEditor_->IsTreeOpen(tree1)) {
			color_ = { 0.7f,0.6f,0.1f,1.0f };
		}
		else {
			color_ = staticColor;
		}
	}
#endif // _DEBUG
	bool flag = false;
	ActiveCheck(camera);
	flag = CreateCount(delta);
	if (isActive_) {
		SetCollider();
	}
	return flag;
}

void MoveWaterGimmick::Draw() const {
	if (isActive_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix,Matrix4x4::MakeIdentity4x4(), color_ });
	}
#ifdef _DEBUG
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, Vector3(endPoint_.x, endPoint_.y, 0.0f));
	instancingManager_->AddBox(modelData_, InstancingModelData{ matrix,Matrix4x4::MakeIdentity4x4(), color_ });
#endif // _DEBUG
}

void MoveWaterGimmick::SetGlobalVariable() {
	if (stageEditor_) {
		std::string tree = "水の生成ギミック" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "水の生成ギミック" + std::to_string(no) + "～" + std::to_string(no + 9);
		stageEditor_->AddItem("始点座標", position_, tree1, tree);
		stageEditor_->AddItem("終点座標", endPoint_, tree1, tree);
		stageEditor_->AddItem("スケール", scale_, tree1, tree);
		stageEditor_->AddItem("生成間隔", createInterval_, tree1, tree);
		stageEditor_->AddItem("生成する水の大きさ", waterParam_.waterScale, tree1, tree);
		stageEditor_->AddItem("水の速さ", waterParam_.waterSpeed_, tree1, tree);

	}
	ApplyGlobalVariable();
}

void MoveWaterGimmick::ApplyGlobalVariable() {
	if (stageEditor_) {
		std::string tree = "水の生成ギミック" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "水の生成ギミック" + std::to_string(no) + "～" + std::to_string(no + 9);
		position_ = stageEditor_->GetVector3Value("始点座標", tree1, tree);
		endPoint_ = stageEditor_->GetVector2Value("終点座標", tree1, tree);
		scale_ = stageEditor_->GetFloatValue("スケール", tree1, tree);
		createInterval_ = stageEditor_->GetFloatValue("生成間隔", tree1, tree);
		waterParam_.waterScale = stageEditor_->GetFloatValue("生成する水の大きさ", tree1, tree);
		waterParam_.waterSpeed_ = stageEditor_->GetFloatValue("水の速さ", tree1, tree);

		waterParam_.moveVector = endPoint_ - Vector2(position_.x, position_.y);
		waterParam_.moveVector = waterParam_.moveVector.Normalize() * waterParam_.waterSpeed_;
	}
}

void MoveWaterGimmick::OnCollision(const Collider& collider) {
	if (collider.GetMask() == ColliderMask::PLAYER) {
		
	}
}

void MoveWaterGimmick::SetCollider() {
	Collider::SetCircle({ position_.x,position_.y }, scale_);
	collisionManager_->SetCollider(this);
}

void MoveWaterGimmick::ActiveCheck(Camera* camera)
{
	Vector2 win = WindowsInfo::GetInstance()->GetWindowSize();
	float ratio = win.y / (std::tanf(0.225f) * (position_.z - camera->transform_.translate_.z) * 2.0f);

	Vector2 pos{};
	pos.x = position_.x * ratio - camera->transform_.translate_.x * ratio + camera->transform_.translate_.x;
	pos.y = position_.y * ratio - camera->transform_.translate_.y * ratio + camera->transform_.translate_.y;

	float radius = scale_ * ratio;

	if ((std::fabsf(pos.x) - radius > win.x * 0.7f) || (std::fabsf(pos.y) - radius > win.y * 0.7f)) {
		isActive_ = false;
	}
	else {
		isActive_ = true;
	}
}

bool MoveWaterGimmick::CreateCount(const float& delta) {
	if (createInterval_ < nowFrame_) {
		nowFrame_ = 0.0f;
		return true;
	}
	nowFrame_ += delta /** 60.0f*/;
	return false;
}
