#include"Particle.h"
#include "particleAcceleration.h"
#include"ModelDataManager.h"
#include"Math/calc.h"
#include"RandomGenerator/RandomGenerator.h"
#include"TextureManager/TextureManager.h"
#include"ImGuiManager/ImGuiManager.h"

#include<numbers>

ParticleAcceleration::ParticleAcceleration()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("plane");
	modelData_ = instancingManager_->GetDrawData({ modelData,TextureManager::GetInstance()->LoadTexture("buf.png"),BlendMode::kBlendModeAdd });
	gVUser_ = new GlobalVariableUser("Effects", "ParticleAcceleration");

	gVUser_->AddItem(keys[SpawnCount], maxSpawnCount_);
	gVUser_->AddItem(keys[DirectionCenter], directionCenter_);
	gVUser_->AddItem(keys[SpawnArea], spawnAreaSize_);
	gVUser_->AddItem(keys[StEdAlpha], stedAlpha_);
	gVUser_->AddItem(keys[StScale], stScale_);
	gVUser_->AddItem(keys[EdScale], edScale_);
	gVUser_->AddItem(keys[DeadCount], maxDeadCount_);
	gVUser_->AddItem(keys[randSpd], randSpd_);
	gVUser_->AddItem(keys[color], color_);
	gVUser_->AddItem(keys[DustSpawnCount], maxDustSpawnCount_);
	gVUser_->AddItem(keys[EffectivePspd], pSpdpower_);
	gVUser_->AddItem(keys[MinSpdColor], minColor_);
	gVUser_->AddItem(keys[MaxSpdColor], maxColor_);
}

ParticleAcceleration::~ParticleAcceleration()
{
	datas_.clear();
}

void ParticleAcceleration::Initialze(const Vector3* playerP,const float normalSpd,const float maxSpd)
{
	playerPos_ = playerP;
	pNormalMaxSpd_ = normalSpd;
	pMaxSpd_ = maxSpd;
	isActive_ = false;
	spawnCount_ = 0;
	datas_.clear();

	SetGlobalV();
}

float Clamp(float min, float max, float& t) {
	if (min > t) {
		t = min;
	}
	else if (max < t) {
		t = max;
	}
	return t;
}

void ParticleAcceleration::Update(const Vector2& playerdirection,float deltaTime)
{
	SetGlobalV();



	//Onの時のパーティクル発生処理
	if (isActive_) {
		//カウント一定量で処理
		if ((spawnCount_ += valueSpawnCount_ * pSpdpower_) >= (float)maxSpawnCount_) {
			spawnCount_ = 0;
			ParticleData newData{};

			//プレイヤー座標を中心に発生位置を設定
			//差の距離算出
			Vector3 direc = { playerdirection.x,playerdirection.y,0 };
			direc = direc.Normalize() * directionCenter_;

			newData.pos = *playerPos_ + direc;
			newData.pos.x += RandomGenerator::GetInstance()->RandFloat(-spawnAreaSize_.x, spawnAreaSize_.x);
			newData.pos.y += RandomGenerator::GetInstance()->RandFloat(-spawnAreaSize_.y, spawnAreaSize_.y);

			//移動方向設定
			newData.velo = -direc.Normalize() * RandomGenerator::GetInstance()->RandFloat(randSpd_.x, randSpd_.y);

			//初期サイズ設定
			newData.scale = { stScale_.x,stScale_.y,1 };

			//初期色設定
			//色の処理
			float colorT = (valueSpawnCount_ - pNormalMaxSpd_ * deltaTime) / (pMaxSpd_ * deltaTime - pNormalMaxSpd_ * deltaTime);
			if (colorT > 0) {
				colorT = Clamp(0.0f, 1.0f, colorT);
				newData.color = Calc::Lerp(minColor_, maxColor_, colorT);
			}
			else {
				newData.color = minColor_;
			}

			Vector2 direcN = { direc.x,direc.y };
			direcN = direcN.Normalize();
			if (direcN.x == 0 && direcN.y == 0) {
				newData.rotate = 0;
			}
			else {
				float dot = std::acos(direcN.x / direcN.Length());
				if (direcN.y < 0) {
					dot = 2 * (float)std::numbers::pi - dot;
				}

				newData.rotate = dot;
			}
			//死亡までのカウント設定
			newData.maxCount_ = maxDeadCount_;

			//初期透明度設定
			newData.alpha_ = stedAlpha_.x;

			//データ追加
			datas_.emplace_back(newData);
		}
	}

	//更新処理
	for (auto& data : datas_) {
		//カウント一定量で死亡
		if (data.count++ >= data.maxCount_) {
			data.isDead = true;
			continue;
		}
		else {

			//生存時の処理

			//移動速度加算
			data.pos += data.velo;

			//T取得
			float t = (float)data.count / (float)data.maxCount_;

			//各値設定し直し
			Vector2 scale = Calc::Lerp(stScale_, edScale_, t);
			data.scale = { scale.x,scale.y,1 };
			data.alpha_ = Calc::Lerp(stedAlpha_.x, stedAlpha_.y, t);
			
			
			//塵沸き処理
			if (data.dustSpawnCount++ >= maxDustSpawnCount_) {
				data.dustSpawnCount = 0;

				Dust newDust;
				newDust.count_ = dustDeadCount_;
				newDust.pos = data.pos;
				newDust.scale = data.scale;
				newDust.rotate = data.rotate;
				newDust.alpha = data.alpha_;

				dDatas_.emplace_back(newDust);
			}
		}
	}

	//塵更新
	for (auto& data : dDatas_) {
		if (data.count_++ >= maxDeadCount_) {
			data.isDead = true;
			continue;
		}
		else {
			float t = (float)data.count_ / (float)maxDeadCount_;

			Vector2 scale = Calc::Lerp(stScale_, edScale_, t);
			data.scale = { scale.x,scale.y,1 };
			data.alpha = Calc::Lerp(stedAlpha_.x, stedAlpha_.y, t);
		}
	}

	//削除処理
	datas_.remove_if([](ParticleData& data) {
		if (data.isDead) {
			return true;
		}
		else {
			return false;
		}
		});

	//削除処理
	dDatas_.remove_if([](Dust& data) {
		if (data.isDead) {
			return true;
		}
		else {
			return false;
		}
		});

}

void ParticleAcceleration::Draw()
{
	for (auto& data : datas_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(data.scale, Vector3{ 0,0,data.rotate }, data.pos);
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix ,Matrix4x4::MakeIdentity4x4(), {data.color.x,data.color.y,data.color.z,data.alpha_} });
	}
	for (auto& data : dDatas_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(data.scale, Vector3{ 0,0,data.rotate }, data.pos);
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix ,Matrix4x4::MakeIdentity4x4(), {color_.x,color_.y,color_.z,data.alpha} });
	}

}

void ParticleAcceleration::IsActive(bool active, float spd)
{

	valueSpawnCount_ = spd;
	if (active) {
		isActive_ = true;		
	}
	else {
		isActive_ = false;
		spawnCount_ = 0;
	}
}

void ParticleAcceleration::SetGlobalV()
{
	maxSpawnCount_ = gVUser_->GetIntValue(keys[SpawnCount]);
	directionCenter_ = gVUser_->GetFloatValue(keys[DirectionCenter]);
	spawnAreaSize_ = gVUser_->GetVector2Value(keys[SpawnArea]);
	stedAlpha_ = gVUser_->GetVector2Value(keys[StEdAlpha]);
	stScale_ = gVUser_->GetVector2Value(keys[StScale]);
	edScale_ = gVUser_->GetVector2Value(keys[EdScale]);
	maxDeadCount_ = gVUser_->GetIntValue(keys[DeadCount]);
	randSpd_ = gVUser_->GetVector2Value(keys[randSpd]);
	color_ = gVUser_->GetVector3Value(keys[color]);
	maxDustSpawnCount_ = gVUser_->GetIntValue(keys[DustSpawnCount]);
	pSpdpower_ = gVUser_->GetFloatValue(keys[EffectivePspd]);
	minColor_ = gVUser_->GetVector3Value(keys[MinSpdColor]);
	maxColor_ = gVUser_->GetVector3Value(keys[MaxSpdColor]);

#ifdef _DEBUG
	ImGui::Begin("AcceSpawnCount");
	ImGui::Text("valueSpawnCount : %f", float(valueSpawnCount_));
	ImGui::End();
#endif // _DEBUG

}
