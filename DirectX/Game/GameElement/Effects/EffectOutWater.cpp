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
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
	modelData_ = instancingManager_->GetDrawData({ modelData,modelData->texture,BlendMode::kBlendModeNormal });
}

void EffectOutWater::Initialize()
{
	datas_.clear();

	dustDatas_.clear();
	diffusionR_ = (1.0f / 2.0f) * (float)std::numbers::pi;
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

		}

		if (data->spawnCount++ >= data->maxSpawnCount) {
			data->spawnCount = 0;

			std::unique_ptr<DustData> newDust = std::make_unique<DustData>();
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
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix ,Matrix4x4::MakeIdentity4x4(), {1,1,1,1} });
	}

	for (auto& data : dustDatas_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3(data->scale, data->scale, 1), Vector3(0, 0, 0), data->translate);
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix ,Matrix4x4::MakeIdentity4x4(), {1,1,1,1} });
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


	for (int i = 0; i < spawnNum; i++) {

		rotateNum = RandomGenerator::GetInstance()->RandFloat(-diffusionR_ / 2.0f, diffusionR_ / 2.0f);

		//傾けたベクトル計算
		Vector2 newVelo = RotateVelo(velo, rotateNum);

		//新しくデータを作成して移動
		std::unique_ptr<OutWaterData> newData;
		newData = std::make_unique<OutWaterData>();
		newData->translate = { translate.x,translate.y,-0.1f };

		newData->scale = scale_;

		//内積から放物線を描きたかった
		//Vector2 v1 =(velo / sqrtf(velo.x * velo.x + velo.y * velo.y))*addveloNum;
		//Vector2 v2 =( newVelo / sqrtf(newVelo.x * newVelo.x + newVelo.y * newVelo.y))*addveloNum;	
		//float num = (v1.x * v2.x + v1.y * v2.y);

		float num = RandomGenerator::GetInstance()->RandFloat(startVelo.x, startVelo.y);

		//速度計算
		newData->velo = Vector3{ newVelo.x,newVelo.y,0 }*num * rateScaling_;
		//加速度計算
		newData->acce = -Vector3{ velo.x,velo.y,0 }.Normalize() * acceSpd_;

		//残留演出を出すまでの間隔
		newData->maxSpawnCount = spawnDustCount_;

		//生存カウント
		newData->maxDeadCount_ = (int)(sqrtf(velo.x * velo.x + velo.y * velo.y)*alliveLeverage_);

		//出現遅延
		newData->maxDelayCount = spawnDelayCount_;

		//データを群に送る
		datas_.emplace_back(std::move(newData));

		//回転量加算
		rotateNum += diffusionR_ / (float)spawnNum;

	}
#pragma endregion
}

Vector2 EffectOutWater::RotateVelo(const Vector2& velo, float theta)
{
	return {
		std::cos(theta) * velo.x + (-std::sin(theta) * velo.y),
		std::sin(theta) * velo.x + (std::cos(theta) * velo.y),
	};
}
