#include "EffectOutWater.h"
#include"ModelDataManager.h"
#include<cassert>
#include<cmath>
#include<numbers>
#include"Math/Vector3.h"
#include"RandomGenerator/RandomGenerator.h"
#include"Ease/Ease.h"

EffectOutWater* EffectOutWater::GetInstance()
{
	static EffectOutWater instance;
	return &instance;
}

void EffectOutWater::SetUp()
{
	if (isSetUp_) {
		//assert(false);
	}
	isSetUp_ = true;

	instancingManager_ = InstancingModelManager::GetInstance();
	modelData_ = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
}

void EffectOutWater::Initialize()
{
	datas_.clear();

	dustDatas_.clear();
	diffusionR_ = (1.0f / 6.0f) * (float)std::numbers::pi;
}

void EffectOutWater::SetPlayerData(const Vector2& translate, const Vector2& Velo)
{
	playerVelo_ = &Velo;

	playerTranslate_ = &translate;
}

void EffectOutWater::Update()
{
	//更新処理
	for (auto& data : datas_) {

		if (data->delayCount++ >= data->maxDelayCount) {

			data->isDraw = true;

			data->translate += data->velo;
			data->velo += data->acce;

			//速度制限
			if (data->velo.Length() > maxSpped_) {
				data->velo = data->velo.Normalize() * maxSpped_;
			}

			if (data->isCountDead_) {
				if (data->count++ >= data->maxDeadCount_) {
					data->isDead_ = true;
				}
			}

			float dot = (data->velo.x * data->acce.x) + (data->velo.y * data->acce.y);
			if (data->isVeloDirectionDead_) {
				//ベクトルが同じ方向
				if (dot > 0) {
					data->isDead_ = true;
				}
			}
		}

		if (data->spawnCount++ >= data->maxSpawnCount) {
			data->spawnCount = 0;

			std::unique_ptr<DustData> newDust =  std::make_unique<DustData>();
			newDust->translate = data->translate;
			newDust->velo = { 0,0,0 };
			newDust->startScale = data->scale;
			newDust->maxCount = maxCount;

			dustDatas_.emplace_back(std::move(newDust));
		}
	}


	


	for (auto& data : dustDatas_) {
		data->translate += data->velo;

		if (data->count++ <= data->maxCount) {
			float t = (float)data->count / (float)data->maxCount;

			data->scale = Ease::UseEase(data->startScale, 0, t);
		}
		else {
			data->isDead = true;
		}
	}

	//データ削除処理
	datas_.remove_if([](auto& data) {
		if (data->isDead_) {
			return true;
		}
		else
		{
			return false;
		}
		});

	//データ削除処理
	dustDatas_.remove_if([](auto& data) {
		if (data->isDead) {
			return true;
		}
		else
		{
			return false;
		}
		});
}

void EffectOutWater::Draw()
{
	for (auto& data : datas_) {

		if (!data->isDraw)continue;
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3(data->scale, data->scale, 1), Vector3(0, 0, data->rotate), data->translate);
		instancingManager_->AddBox(modelData_, InstancingModel{ matrix ,{1,1,1,1} });
	}

	for (auto& data : dustDatas_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3(data->scale, data->scale, 1), Vector3(0, 0, 0), data->translate);
		instancingManager_->AddBox(modelData_, InstancingModel{ matrix ,{1,1,1,1} });
	}
}

void EffectOutWater::Finalize()
{
	datas_.clear();
}


void EffectOutWater::SpawnEffect(const Vector2& translate, const Vector2& velo, int32_t spawnNum)
{


#pragma region 
	float rotateNum = -(diffusionR_ / 2.0f);

	int accePower = -(spawnNum / 2);

	for (int i = 0; i < spawnNum; i++) {

		rotateNum = RandomGenerator::GetInstance()->RandFloat(-diffusionR_, diffusionR_);

		//傾けたベクトル計算
		Vector2 newVelo = RotateVelo(velo, rotateNum);

		//新しくデータを作成して移動
		std::unique_ptr<OutWaterData> newData;
		newData = std::make_unique<OutWaterData>();
		newData->translate = { translate.x,translate.y,-1 };
		
		float randVelo = RandomGenerator::GetInstance()->RandFloat(0.5f, 1.5f);

		//速度計算
		newData->velo = Vector3{ newVelo.x,newVelo.y,0 }.Normalize()*0.2f*randVelo;
		//加速度計算
		newData->acce = -Vector3{ velo.x,velo.y,0 }.Normalize()*0.01f;

		//残留演出を出すまでの間隔
		newData->maxSpawnCount = 0;

		//データを群に送る
		datas_.emplace_back(std::move(newData));

		//回転量加算
		rotateNum += diffusionR_ / (float)spawnNum;

		accePower++;
	}
#pragma endregion

	//




}

Vector2 EffectOutWater::RotateVelo(const Vector2& velo, float theta)
{
	return {
		std::cos(theta) * velo.x + (-std::sin(theta) * velo.y),
		std::sin(theta) * velo.x + (std::cos(theta) * velo.y),
	};
}
