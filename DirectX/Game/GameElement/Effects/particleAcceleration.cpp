#include"Particle.h"
#include "particleAcceleration.h"
#include"ModelDataManager.h"
#include"Math/calc.h"
#include"RandomGenerator/RandomGenerator.h"

ParticleAcceleration::ParticleAcceleration()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
	modelData_ = instancingManager_->GetDrawData({ modelData,modelData->texture,BlendMode::kBlendModeNormal });
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
}

void ParticleAcceleration::Update()
{
	//Onの時のパーティクル発生処理
	if (isActive_) {
		//カウント一定量で処理
		if (spawnCount_++ >= maxSpawnCount_) {
			spawnCount_ = 0;
			ParticleData newData{};

			//プレイヤー座標を中心に発生位置を設定
			newData.pos = *playerPos_;
			newData.pos.x += RandomGenerator::GetInstance()->RandFloat(-spawnAreaSize_.x, spawnAreaSize_.x);
			newData.pos.y += RandomGenerator::GetInstance()->RandFloat(-spawnAreaSize_.y, spawnAreaSize_.y);

			//速度設定
			newData.velo = RandomGenerator::GetInstance()->RandVector3(minVelo_,maxVelo_) * RandomGenerator::GetInstance()->RandFloat(randSpd_.x,randSpd_.y);

			//初期サイズ設定
			newData.scale = { stScale_,stScale_,stScale_ };

			//死亡までのカウント設定
			newData.maxCount_ = maxDeadCount_;

			//初期透明度設定
			newData.alpha_ = stAlpha_;

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
			float scale = Calc::Lerp(stScale_, edScale_, t);
			data.scale = { scale,scale,scale };
			data.alpha_ = Calc::Lerp(stAlpha_, edAlpha_, t);
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

}

void ParticleAcceleration::Draw()
{

	for (auto& data : datas_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(data.scale, Vector3{ 0,0,0 }, data.pos);
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix ,Matrix4x4::MakeIdentity4x4(), {1,1,1,1} });
	}

}

void ParticleAcceleration::IsActive(bool active)
{
	if (active) {
		isActive_ = true;
	}
	else {
		isActive_ = false;
		spawnCount_ = 0;
	}
}
