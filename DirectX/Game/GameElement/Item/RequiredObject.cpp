#include "RequiredObject.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "GameElement/Wave/Wave.h"
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "SceneSystem/IScene/IScene.h"
#include "WindowsInfo/WindowsInfo.h"
#include "ItemManager.h"
#include"GameElement/Effects/GetItem/GetItem.h"

InstancingModelManager* RequiredObject::instancingManager_ = nullptr;
const InstancingMeshTexData* RequiredObject::modelData_ = nullptr;

ItemManager* RequiredObject::itemManager_ = nullptr;
Vector4 RequiredObject::staticColor_ = { 1.0f,1.0f,1.0f,1.0f };


RequiredObject::RequiredObject(const int& no, const float& scale, const float& scaleDia) {
	isHit_ = false;
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::ITEM);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	position_ = { 0.0f,1.0f,-0.02f };
	scale_ = scale;
	scaleDiameter_ = scaleDia;
	rotate_ = 0.0f;

	no_ = no;
	stageEditor_ = std::make_unique<StageEditor>("アイテムの配置");
	SetGlobalVariable();
	color_ = staticColor_;
	isActive_ = true;

	seGetCoin_.LoadWave("SE/getCoin.wav");
}

void RequiredObject::StaticInitialize() {
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("plane");
	const Texture* tex = TextureManager::GetInstance()->LoadTexture("item_speaker.png");
	modelData_ = instancingManager_->GetDrawData({ modelData,tex,BlendMode::kBlendModeNormal });
	itemManager_ = ItemManager::GetInstance();
	//staticColor_ = itemManager_->GetColor();

}

bool RequiredObject::Update(float deltaTime, Camera* camera) {
	
	if (isHit_) { return true; }
#ifdef _DEBUG
	ApplyGlobalVariable();
	//staticColor_ = itemManager_->GetColor();
	if (stageEditor_) {
		std::string tree = "アイテム" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "アイテム" + std::to_string(no) + "～" + std::to_string(no + 9);
		if (stageEditor_->IsTreeOpen(tree1, tree)) {
			color_ = { 1.0f,0.3f,0.3f,1.0f };
		}
		else if (stageEditor_->IsTreeOpen(tree1)) {
			color_ = { 0.7f,0.6f,0.1f,1.0f };
		}
		else {
			color_ = staticColor_;
		}
	}
#endif // _DEBUG

	ActiveCheck(camera);
	deltaTime;
	if (isActive_) {
		SetCollider();
	}

	return isHit_;
}

void RequiredObject::Draw() const {
	if (isActive_) {
		if (!isHit_) {
			Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
			instancingManager_->AddBox(modelData_, InstancingModelData{ matrix,Matrix4x4::MakeIdentity4x4(), color_ });
		}
	}
}

void RequiredObject::SetGlobalVariable() {
	if (stageEditor_) {
		std::string tree = "必須アイテム" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "必須アイテム" + std::to_string(no) + "～" + std::to_string(no + 9);
		stageEditor_->AddItem("ポジション", position_, tree1, tree);
	}
	ApplyGlobalVariable();
}

void RequiredObject::ApplyGlobalVariable() {
	if (stageEditor_) {
		std::string tree = "必須アイテム" + std::to_string(no_);
		int no = no_ / 10;
		no = no * 10;
		std::string tree1 = "必須アイテム" + std::to_string(no) + "～" + std::to_string(no + 9);
		position_ = stageEditor_->GetVector3Value("ポジション", tree1, tree);
	}
}

void RequiredObject::OnCollision(const Collider& collider) {
	if (collider.GetMask() == ColliderMask::PLAYER) {
		isHit_ = true;
		seGetCoin_.Play();

		EffectGetItem::GetInstance()->Spawn(position_);
	}
}

void RequiredObject::SetCollider() {
	if (!isHit_) {
		Collider::SetCircle({ position_.x,position_.y }, scale_ * scaleDiameter_);
		collisionManager_->SetCollider(this);
	}
}

void RequiredObject::ActiveCheck(Camera* camera) {
	Vector2 win = WindowsInfo::GetInstance()->GetWindowSize();
	float ratio = win.y / (std::tanf(0.225f) * (position_.z - camera->transform_.translate_.z) * 2);

	Vector2 pos{};
	pos.x = position_.x * ratio - camera->transform_.translate_.x * ratio + camera->transform_.translate_.x;
	pos.y = position_.y * ratio - camera->transform_.translate_.y * ratio + camera->transform_.translate_.y;

	float radius = scale_ * ratio;

	if ((std::abs(pos.x) - radius > win.x * 0.7f) || (std::abs(pos.y) - radius > win.y * 0.7f)) {
		isActive_ = false;
	}
	else {
		isActive_ = true;
	}
}
