#include"Particle.h"
#include "particleAcceleration.h"
#include"ModelDataManager.h"
#include"Math/calc.h"
#include"RandomGenerator/RandomGenerator.h"
#include"TextureManager/TextureManager.h"

ParticleAcceleration::ParticleAcceleration()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("plane");
	modelData_ = instancingManager_->GetDrawData({ modelData,TextureManager::GetInstance()->LoadTexture("buf.png"),BlendMode::kBlendModeNormal });

	gVUser_ = new GlobalVariableUser("Effects", "ParticleAcceleration");

	gVUser_->AddItem(keys[SpawnCount], maxSpawnCount_);
	gVUser_->AddItem(keys[SpawnArea], spawnAreaSize_);
	gVUser_->AddItem(keys[StEdAlpha], stedAlpha_);
	gVUser_->AddItem(keys[StEdScale], stedScale_);
	gVUser_->AddItem(keys[DeadCount], maxDeadCount_);
	gVUser_->AddItem(keys[randSpd], randSpd_);
	gVUser_->AddItem(keys[color], color_);
	gVUser_->AddItem(keys[DustSpawnCount], maxDustSpawnCount_);
	gVUser_->AddItem(keys[EffectivePspd], pSpdpower_);

}

ParticleAcceleration::~ParticleAcceleration()
{
	datas_.clear();
}

void ParticleAcceleration::Initialze(const Vector3* playerP)
{
	playerPos_ = playerP;
	isActive_ = false;
	spawnCount_ = 0;
	datas_.clear();

	SetGlobalV();
}

void ParticleAcceleration::Update()
{
	SetGlobalV();

	//Onの時のパーティクル発生処理
	if (isActive_) {
		//カウント一定量で処理
		if ((spawnCount_+=valueSpawnCount_*pSpdpower_) >= (float)maxSpawnCount_) {
			spawnCount_ = 0;
			ParticleData newData{};

			//プレイヤー座標を中心に発生位置を設定
			newData.pos = *playerPos_;
			newData.pos.x += RandomGenerator::GetInstance()->RandFloat(-spawnAreaSize_.x, spawnAreaSize_.x);
			newData.pos.y += RandomGenerator::GetInstance()->RandFloat(-spawnAreaSize_.y, spawnAreaSize_.y);

			//速度設定
			newData.velo = RandomGenerator::GetInstance()->RandVector3(minVelo_, maxVelo_) * RandomGenerator::GetInstance()->RandFloat(randSpd_.x, randSpd_.y);

			//初期サイズ設定
			newData.scale = { stedScale_.x,stedScale_.x,stedScale_.x };

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
			float scale = Calc::Lerp(stedScale_.x, stedScale_.y, t);
			data.scale = { scale,scale,scale };
			data.alpha_ = Calc::Lerp(stedAlpha_.x, stedAlpha_.y, t);

			//塵沸き処理
			if (data.dustSpawnCount++ >= maxDustSpawnCount_) {
				data.dustSpawnCount = 0;

				Dust newDust;
				newDust.count_ = data.count;
				newDust.pos = data.pos;
				newDust.scale = data.scale;
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

			float scale = Calc::Lerp(stedScale_.x, stedScale_.y, t);
			data.scale = { scale,scale,scale };
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
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(data.scale, Vector3{ 0,0,0 }, data.pos);
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix ,Matrix4x4::MakeIdentity4x4(), {color_.x,color_.y,color_.z,data.alpha_} });
	}
	for (auto& data : dDatas_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(data.scale, Vector3{ 0,0,0 }, data.pos);
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix ,Matrix4x4::MakeIdentity4x4(), {color_.x,color_.y,color_.z,data.alpha} });
	}

}

void ParticleAcceleration::IsActive(bool active,float spd)
{
	if (active) {
		isActive_ = true;
		valueSpawnCount_ = spd;
	}
	else {
		isActive_ = false;
		spawnCount_ = 0;
	}
}

void ParticleAcceleration::SetGlobalV()
{
	maxSpawnCount_ = gVUser_->GetIntValue(keys[SpawnCount]);
	spawnAreaSize_ = gVUser_->GetVector2Value(keys[SpawnArea]);
	stedAlpha_ = gVUser_->GetVector2Value(keys[StEdAlpha]);
	stedScale_ = gVUser_->GetVector2Value(keys[StEdScale]);
	maxDeadCount_ = gVUser_->GetIntValue(keys[DeadCount]);
	randSpd_ = gVUser_->GetVector2Value(keys[randSpd]);
	color_ = gVUser_->GetVector3Value(keys[color]);
	maxDustSpawnCount_ = gVUser_->GetIntValue(keys[DustSpawnCount]);
	pSpdpower_ = gVUser_->GetFloatValue(keys[EffectivePspd]);
}
