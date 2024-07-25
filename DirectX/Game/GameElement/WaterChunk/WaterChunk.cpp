#include "WaterChunk.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "CollisionSystem/CollisionManager/CollisionManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "SceneSystem/IScene/IScene.h"
#include "WindowsInfo/WindowsInfo.h"
#include "Camera.h"
#include "GameElement/Player/Player.h"
#include "GameElement/WaterManager/WaterManager.h"

InstancingModelManager* WaterChunk::instancingManager_ = nullptr;
const InstancingMeshTexData* WaterChunk::modelData_ = nullptr;

std::unique_ptr<GlobalVariableUser> WaterChunk::staticGlobalVariable_ = nullptr;
float WaterChunk::deleteTime_ = 2.0f;
float WaterChunk::minScale_ = 0.7f;

const Player* WaterChunk::player_ = nullptr;
WaterManager* WaterChunk::waterManager_ = nullptr;

WaterChunk::WaterChunk()
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::WATER);
	//Collider::AddTargetMask(ColliderMask::PLAYER);
	//Collider::AddTargetMask(ColliderMask::CLIENT);

	gravityArea_ = std::make_unique<GravityArea>();

	/*float scale = WaterChunkChip::GetScale();
	int map = 100;
	for (int i = 0; i < map; i++) {
		float x = scale * i - scale * map / 2 - scale / 2;
		for (int j = 0; j < map; j++) {
			float z = scale * j - scale * map / 2 - scale / 2;
			chips_.push_back(std::make_unique<WaterChunkChip>(Vector3{ x,0.0f,z }));
		}
	}*/
	position_ = {};
	scale_ = 1.0f;
	maxScale_ = scale_;
	rotate_ = 0.0f;
	isSmaeGravitySize_ = false;
	no_ = 0;
	isSmall_ = false;
	isTree_ = false;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	isWave_ = false;
	isPlayer_ = false;
	preIsPlayer_ = false;

	isTarget_ = false;
	isQuadrangleActive_ = false;
	endNo_ = 0;
	isActive_ = true;
}

WaterChunk::WaterChunk(int no)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	gravityArea_ = std::make_unique<GravityArea>();

	position_ = {};
	scale_ = 1.0f;
	maxScale_ = scale_;
	rotate_ = 0.0f;

	no_ = no;
	isSmall_ = false;
	std::string tree = "水" + std::to_string(no_);
	int num = no_ / 10;
	num = num * 10;
	std::string tree1 = "水" + std::to_string(num) + "～" + std::to_string(num + 9);
	stageEditor_ = std::make_unique<StageEditor>("水の配置", tree1, tree);
	endNo_ = no_ - 1;
	if (endNo_ < 0) {
		endNo_ = 0;
	}
	isTarget_ = false;
	SetGlobalVariable();
	scale_ = maxScale_;
	isSmaeGravitySize_ = false;
	isTree_ = false;
	isActive_ = true;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	isPlayer_ = false;
	preIsPlayer_ = false;
	CreateChips();
	isWave_ = false;

	isQuadrangleActive_ = false;
}

WaterChunk::WaterChunk(const int& no, const MoveWaterGimmick::GimmickWaterParam& param) {
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	gravityArea_ = std::make_unique<GravityArea>();

	moveWaterGimmickParam_ = param;
	position_ = Vector3(moveWaterGimmickParam_.respawnPoint.x, moveWaterGimmickParam_.respawnPoint.y, 0.0f);
	scale_ = moveWaterGimmickParam_.waterScale;
	maxScale_ = scale_;
	rotate_ = 0.0f;

	no_ = no;
	isSmall_ = false;
	endNo_ = no_ - 1;
	if (endNo_ < 0) {
		endNo_ = 0;
	}
	isTarget_ = false;
	scale_ = maxScale_;
	isSmaeGravitySize_ = false;
	isTree_ = false;
	isActive_ = true;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	isPlayer_ = false;
	preIsPlayer_ = false;
	CreateChips();
	isWave_ = false;

	isQuadrangleActive_ = false;
}

WaterChunk::WaterChunk(const int& no, const Vector2& pos)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	gravityArea_ = std::make_unique<GravityArea>();

	position_ = { pos.x,pos.y,0.0f };
	scale_ = 1.0f;
	maxScale_ = scale_;
	rotate_ = 0.0f;

	no_ = no;
	isSmall_ = false;
	std::string tree = "水" + std::to_string(no_);
	int num = no_ / 10;
	num = num * 10;
	std::string tree1 = "水" + std::to_string(num) + "～" + std::to_string(num + 9);
	stageEditor_ = std::make_unique<StageEditor>("水の配置", tree1, tree);
	endNo_ = no_ - 1;
	if (endNo_ < 0) {
		endNo_ = 0;
	}
	isTarget_ = false;
	SetGlobalVariableAndSetPos();
	scale_ = maxScale_;
	isSmaeGravitySize_ = false;
	isTree_ = false;
	isActive_ = true;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	isPlayer_ = false;
	preIsPlayer_ = false;
	CreateChips();
	isWave_ = false;

	isQuadrangleActive_ = false;
}

WaterChunk::WaterChunk(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate, bool isSmall)
{
	Collider::CreateCollider(ColliderShape::CIRCLE, ColliderType::COLLIDER, ColliderMask::WATER);
	Collider::AddTargetMask(ColliderMask::PLAYER);

	gravityArea_ = std::make_unique<GravityArea>();

	position_ = { pos.x,pos.y,0.0f };
	scale_ = radius.x;
	maxScale_ = scale_;
	rotate_ = rotate;
	isSmall_ = isSmall;
	time_ = 0.0f;

	isSmaeGravitySize_ = isSame;
	isTree_ = false;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	isWave_ = false;
	isPlayer_ = false;
	preIsPlayer_ = false;

	isTarget_ = false;
	isQuadrangleActive_ = false;
	endNo_ = 0;
	isActive_ = true;
}

void WaterChunk::StaticInitialize()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
	modelData_ = instancingManager_->GetDrawData({ modelData,modelData->texture,BlendMode::kBlendModeNormal });

	waterManager_ = WaterManager::GetInstance();

	StaticSetGlobalVariable();
}

void WaterChunk::Initialize()
{

}

void WaterChunk::Update(const float& deltaTime, Camera* camera, const bool& globalUse)
{
#ifdef _DEBUG
	if (globalUse) {
		if (stageEditor_ && stageEditor_->IsTreeOpen()) {
			ApplyGlobalVariable();
		}
	}
	CreateQuadrangle();
	if (!isSmall_) {
		if (scale_ != maxScale_ || position_ != chips_.front()->GetCenter()) {
			CreateChips();
			scale_ = maxScale_;
		}
	}
#endif // _DEBUG

	if (!isPlayer_ && preIsPlayer_ && !player_->GetPreInWater()) {
		AddWave(false);
	}


	if (isSmall_) {
		time_ += deltaTime;

		time_ = std::clamp(time_, 0.0f, deleteTime_);
		scale_ = (1.0f - time_ / deleteTime_) * maxScale_;
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

	for (std::list<std::unique_ptr<WaterWave>>::iterator it = waves_.begin(); it != waves_.end();) {
		if ((*it)->IsFinish()) {
			it = waves_.erase(it);
		}
		else {
			/*std::unordered_map<int, std::unique_ptr<WaterChunk>>& fullWater = waterManager_->GetWater();
			for (std::pair<const int, std::unique_ptr<WaterChunk>>& water : fullWater) {
				if (water.first != no_ && water.second->isActive_) {

					const std::list<int> nums = (*it)->GetNums();
					bool same = false;
					for (const int& num : nums) {
						if (water.first == num) {
							same = true;
							break;
						}
					}
					if (!same) {
						water.second->AddWave(*this, *(*it));
					}
				}
			}*/

			it++;
		}
	}

	ActiveCheck(camera);
	preIsPlayer_ = isPlayer_;
	isPlayer_ = false;
	if (isActive_) {
		gravityArea_->Update({ position_.x,position_.y }, { scale_,scale_ }, isSmaeGravitySize_, rotate_);
		SetCollider();
	}

	if (isQuadrangleActive_) {
		quadrangle_->Update(deltaTime, camera);
	}
}

void WaterChunk::MoveUpdate() {
	position_.x += moveWaterGimmickParam_.moveVector.x;
	position_.y += moveWaterGimmickParam_.moveVector.y;
}

void WaterChunk::Draw(Camera* camera) const
{
	if (isActive_) {
		/*for (std::unique_ptr<WaterChunkChip>& chip : chips_) {
		chip->Draw();
	}*/
#ifdef _DEBUG
		//gravityArea_->Draw({ position_.x,position_.y }, { scale_,scale_ }, isSmaeGravitySize_, rotate_);
#endif // _DEBUG
		if (isWave_) {
			for (const std::unique_ptr<WaterChunkChip>& chip : chips_) {
				chip->Draw();
			}
			Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_ * minScale_,scale_ * minScale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
			instancingManager_->AddBox(modelData_, InstancingModelData{ matrix, Matrix4x4::MakeIdentity4x4(), color_ });
		}
		else {
			Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_,scale_,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
			instancingManager_->AddBox(modelData_, InstancingModelData{ matrix, Matrix4x4::MakeIdentity4x4(), color_ });
		}
	}
	if (isQuadrangleActive_) {
		quadrangle_->Draw(camera);
	}
}

void WaterChunk::StaticUpdate()
{
#ifdef _DEBUG
	StaticApplyGlobalVariable();
#endif // _DEBUG
}

void WaterChunk::SetPlayer(const Player* player)
{
	player_ = player;
}

void WaterChunk::CreateQuadrangle()
{
	bool is = false;
	if (isTarget_) {
		if (endNo_ >= 0 && endNo_ != no_) {
			if (!quadrangle_) {
				quadrangle_ = std::make_unique<WaterChunkQuadrangle>();
			}
			const WaterChunk* water = waterManager_->GetWater(endNo_);
			if (water) {
				is = true;
				quadrangle_->CreateQuadrangle(position_, scale_, water->position_, water->scale_);
			}
		}
	}
	if (is) {
		isQuadrangleActive_ = true;
	}
	else {
		isQuadrangleActive_ = false;
	}
}

bool WaterChunk::IsHitMouse(const Vector2& mousePos) const
{
	if (isActive_) {
		Vector2 pos = { position_.x - mousePos.x,position_.y - mousePos.y };
		if (pos.x * pos.x + pos.y * pos.y <= scale_ * scale_) {
			return true;
		}
	}
	return false;
}

void WaterChunk::SetPosition(const Vector3& pos)
{
	position_ = pos;
	if (stageEditor_) {
		stageEditor_->SetVariable("ポジション", position_);
	}
}

void WaterChunk::SetScale(const float& scale)
{
	maxScale_ = scale;
	if (stageEditor_) {
		stageEditor_->SetVariable("スケール", maxScale_);
	}
}

void WaterChunk::SetIsQuadrangle(const bool& is, const int& endNo)
{
	isTarget_ = is;
	endNo_ = endNo;
	if (isTarget_) {
		CreateQuadrangle();
	}
	if (stageEditor_) {
		stageEditor_->SetVariable("四角形を作るか", isTarget_);
		stageEditor_->SetVariable("四角形を生成するもう一方の水のナンバー", endNo_);
	}
}

void WaterChunk::SetGlobalVariable()
{
	if (stageEditor_) {
		stageEditor_->AddItem("ポジション", position_);
		stageEditor_->AddItem("スケール", maxScale_);
		stageEditor_->AddItem("四角形を作るか", isTarget_);
		stageEditor_->AddItem("四角形を生成するもう一方の水のナンバー", endNo_);
	}
	ApplyGlobalVariable();
}

void WaterChunk::ApplyGlobalVariable()
{
	if (stageEditor_) {
		position_ = stageEditor_->GetVector3Value("ポジション");
		maxScale_ = stageEditor_->GetFloatValue("スケール");
		isTarget_ = stageEditor_->GetBoolValue("四角形を作るか");
		endNo_ = stageEditor_->GetIntValue("四角形を生成するもう一方の水のナンバー");
	}
}

void WaterChunk::SetGlobalVariableAndSetPos()
{
	if (stageEditor_) {
		stageEditor_->AddItem("ポジション", position_);
		stageEditor_->AddItem("スケール", maxScale_);
		stageEditor_->AddItem("四角形を作るか", isTarget_);
		stageEditor_->AddItem("四角形を生成するもう一方の水のナンバー", endNo_);

		stageEditor_->SetVariable("ポジション", position_);
		stageEditor_->SetVariable("四角形を作るか", false);
	}
	ApplyGlobalVariable();
}

void WaterChunk::StaticSetGlobalVariable()
{
	staticGlobalVariable_ = std::make_unique<GlobalVariableUser>("Charactor", "Player");
	staticGlobalVariable_->AddItem("水が消えるまでの時間", deleteTime_, "水の生成関係");
	StaticApplyGlobalVariable();
}

void WaterChunk::StaticApplyGlobalVariable()
{
	deleteTime_ = staticGlobalVariable_->GetFloatValue("水が消えるまでの時間", "水の生成関係");

	if (deleteTime_ <= 0.0f) {
		0.1f;
	}
}

void WaterChunk::ActiveCheck(Camera* camera)
{
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

void WaterChunk::CreateChips()
{
	float rotateAdd = 3.1415f / 90;
	float rotate = 0.0f;

	float scale = scale_ / 2;
	float rad = scale;
	chips_.clear();
	for (int i = 0; i < 180; i++) {

		Vector3 pos{};
		pos.x = rad * std::cosf(rotate);
		pos.y = rad * std::sinf(rotate);

		pos += position_;

		chips_.push_back(std::make_unique<WaterChunkChip>(position_, pos, rotate, scale));

		rotate += rotateAdd;
	}

}

void WaterChunk::AddWave(const bool& isDown)
{
	// 波発生
	Vector3 pos = player_->GetPosition() - position_;
	Vector2 vect = { pos.x,pos.y };
	vect = vect.Normalize();
	float rotate = std::acosf(vect.x);
	if (vect.y < 0) {
		rotate = 6.28f - rotate;
	}
	waves_.push_back(std::make_unique<WaterWave>(player_->GetVelocity(), rotate, isDown, no_));
	waves_.back()->Update(0.005f);
}

void WaterChunk::AddWave(const WaterChunk& water, WaterWave& wave)
{
	if ((position_ - water.position_).Length() <= scale_ + water.scale_) {

		Vector3 pos = position_ - water.position_;
		Vector2 vect = { pos.x,pos.y };
		vect = vect.Normalize();
		float theta = std::acosf(vect.x);
		if (vect.y < 0) {
			theta = 6.28f - theta;
		}
		float length = (position_ - water.position_).Length();
		if (length != 0.0f) {
			float x = (water.scale_ * water.scale_ - scale_ * scale_ + length * length) / length;

			float s = (water.scale_ + scale_ + length) / 2;

			float y = 2 * std::sqrtf(s * (s - scale_) * (s - water.scale_) * (s - length)) / length;

			Vector2 pos1 = { x,y };
			Vector2 pos2 = { x,-y };

			// ここから角度求めて範囲内にあったら角度求めて波の生成とナンバーの追加
			Vector3 postion1 = { 0.0f,0.0f,position_.z };
			postion1.x = pos1.x * std::cosf(theta) - pos1.y * std::sinf(theta);
			postion1.y = pos1.x * std::sinf(theta) + pos1.y * std::cosf(theta);

			Vector3 postion2 = { 0.0f,0.0f,position_.z };
			postion2.x = pos2.x * std::cosf(theta) - pos2.y * std::sinf(theta);
			postion2.y = pos2.x * std::sinf(theta) + pos2.y * std::cosf(theta);


			pos = postion1 - water.position_;
			vect = { pos.x,pos.y };
			vect = vect.Normalize();
			theta = std::acosf(vect.x);
			if (vect.y < 0) {
				theta = 6.28f - theta;
			}

			float power1 = wave.GetPower(theta);

			pos = postion2 - water.position_;
			vect = { pos.x,pos.y };
			vect = vect.Normalize();
			theta = std::acosf(vect.x);
			if (vect.y < 0) {
				theta = 6.28f - theta;
			}

			float power2 = wave.GetPower(theta);


			if (power1 != 0.0f && power2 != 0.0f) {
				wave.AddNum(no_);
				float next = wave.GetNextMaxRotate();

				pos = postion1 - position_;
				vect = { pos.x,pos.y };
				vect = vect.Normalize();
				theta = std::acosf(vect.x);
				if (vect.y < 0) {
					theta = 6.28f - theta;
				}
				std::list<int> nums = wave.GetNums();
				waves_.push_back(std::make_unique<WaterWave>(std::abs(power1), theta, power1 < 0, next));
				waves_.back()->Update(0.005f);
				for (const int& no : nums) {
					waves_.back()->AddNum(no);
				}
				
				pos = postion2 - position_;
				vect = { pos.x,pos.y };
				vect = vect.Normalize();
				theta = std::acosf(vect.x);
				if (vect.y < 0) {
					theta = 6.28f - theta;
				}
				waves_.push_back(std::make_unique<WaterWave>(std::abs(power2), theta, power2 < 0, next));
				waves_.back()->Update(0.005f);
				for (const int& no : nums) {
					waves_.back()->AddNum(no);
				}
			}
			else if (power1 != 0.0f) {
				wave.AddNum(no_);
				pos = postion1 - position_;
				vect = { pos.x,pos.y };
				vect = vect.Normalize();
				theta = std::acosf(vect.x);
				if (vect.y < 0) {
					theta = 6.28f - theta;
				}
				std::list<int> nums = wave.GetNums();
				waves_.push_back(std::make_unique<WaterWave>(std::abs(power1), theta, power1 < 0, wave.GetNextMaxRotate()));
				waves_.back()->Update(0.005f);
				for (const int& no : nums) {
					waves_.back()->AddNum(no);
				}
			}
			else if (power2 != 0.0f) {
				wave.AddNum(no_);
				pos = postion2 - position_;
				vect = { pos.x,pos.y };
				vect = vect.Normalize();
				theta = std::acosf(vect.x);
				if (vect.y < 0) {
					theta = 6.28f - theta;
				}
				waves_.push_back(std::make_unique<WaterWave>(std::abs(power2), theta, power2 < 0, wave.GetNextMaxRotate()));
				waves_.back()->Update(0.005f);
				std::list<int> nums = wave.GetNums();
				for (const int& no : nums) {
					waves_.back()->AddNum(no);
				}
			}
		}
	}
}

void WaterChunk::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::PLAYER) {
		isPlayer_ = true;
		if (!player_->GetPreInWater()) {
			// 波発生
			AddWave(true);
		}
	}
}

void WaterChunk::SetCollider()
{
	Collider::SetCircle({ position_.x,position_.y }, scale_);
	collisionManager_->SetCollider(this);
}
