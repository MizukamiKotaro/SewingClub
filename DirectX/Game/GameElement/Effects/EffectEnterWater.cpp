#include "EffectEnterWater.h"
#include"ModelDataManager.h"
#include"Ease/Ease.h"
#include"RandomGenerator/RandomGenerator.h"

#include<numbers>

EffectEnterWater::EffectEnterWater()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
	modelData_ = instancingManager_->GetDrawData({ modelData,modelData->texture,BlendMode::kBlendModeNormal });
	gVariUser = new GlobalVariableUser("Effects","EffectEnterWater");
	
	gVariUser->AddItem(keys[diffsion], diffusion_);
	gVariUser->AddItem(keys[scale], scale_);
	gVariUser->AddItem(keys[startVelo], startVelo_);
	gVariUser->AddItem(keys[bendNum], bendNum_);
	gVariUser->AddItem(keys[maxSPD], maxSpped_);
	gVariUser->AddItem(keys[rateScaling], rateScaling_);
	gVariUser->AddItem(keys[spawnDustCount], spawnDustCount_);
	gVariUser->AddItem(keys[acceSPD], acceSpd_);
	gVariUser->AddItem(keys[alliveLeverage], alliveLeverage_);
	gVariUser->AddItem(keys[setEqualSpace], setEqualSpace_);
	gVariUser->AddItem(keys[isRandomVelo], isVeloRandom_);
	gVariUser->AddItem(keys[SpawnNum], spawnNum_);


}

EffectEnterWater::~EffectEnterWater()
{
	datas_.clear();
	dustDatas_.clear();
}

void EffectEnterWater::Initialize()
{
	datas_.clear();

	dustDatas_.clear();
}

void EffectEnterWater::Update()
{
	
	Debug();

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

			float t = (float)data->count / (float)data->maxDeadCount_;
			data->scale = Ease::UseEase(data->saveScale, 0, t);

			
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

void EffectEnterWater::Draw()
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

void EffectEnterWater::Debug()
{

	diffusion_ = gVariUser->GetFloatValue(keys[diffsion]);
	scale_ = gVariUser->GetFloatValue(keys[scale]);
	startVelo_ = gVariUser->GetVector2Value(keys[startVelo]);
	bendNum_ = gVariUser->GetFloatValue(keys[bendNum]);
	maxSpped_ = gVariUser->GetFloatValue(keys[maxSPD]);
	rateScaling_ = gVariUser->GetFloatValue(keys[rateScaling]);
	spawnDustCount_ = gVariUser->GetIntValue(keys[spawnDustCount]);
	acceSpd_ = gVariUser->GetFloatValue(keys[acceSPD]);
	alliveLeverage_ = gVariUser->GetFloatValue(keys[alliveLeverage]);
	setEqualSpace_ = gVariUser->GetBoolValue(keys[setEqualSpace]);
	isVeloRandom_ = gVariUser->GetBoolValue(keys[isRandomVelo]);
	spawnNum_ = gVariUser->GetIntValue(keys[SpawnNum]);

}

void EffectEnterWater::SpawnEffect(const Vector2& translate, const Vector2& velo, const Vector2& gpos)
{

	//水の球からプレイヤーまでの上向きのベクトル
	Vector2 effeVelo = Vector2{ translate - gpos }.Normalize() * (sqrtf(velo.x * velo.x + velo.y * velo.y));


	float rotate = diffusion_ * (float)std::numbers::pi;
	
	//等間隔の時の処理1
	rotate = -rotate / 2;

	for (int i = 0; i < spawnNum_; i++) {
		//ランダム
		if (!setEqualSpace_) {
			rotate = RandomGenerator::GetInstance()->RandFloat(-diffusion_ * (float)std::numbers::pi, diffusion_ * (float)std::numbers::pi);
		}
		//傾けたベクトル計算
		Vector2 newVelo = RotateVelo(effeVelo, rotate);

		//新しくデータを作成して移動
		std::unique_ptr<EnterWaterData> newData;
		newData = std::make_unique<EnterWaterData>();
		newData->translate = { translate.x,translate.y,-0.2f };
		newData->saveScale = scale_;

		//初速度
		float num = RandomGenerator::GetInstance()->RandFloat(startVelo_.x, startVelo_.y);

		if (!isVeloRandom_) {
			num = 1;
		}

		//湾曲度を計算
		//放物線式のｘを求める（分母は速度が０になる値
		float x = rotate / (float)std::numbers::pi;
		//計算
		float bend = (-bendNum_ * sqrtf(x * x) + 1);

		
		newData->velo = (Vector3{ newVelo.x,newVelo.y,0 }*num * rateScaling_) * bend;

		//加速度計算
		newData->acce = -Vector3{ effeVelo.x,effeVelo.y,0}.Normalize() * acceSpd_;

		//残留演出を出すまでの間隔
		newData->maxSpawnCount = spawnDustCount_;

		//生存カウント
		newData->maxDeadCount_ = (int)(sqrtf(velo.x * velo.x + velo.y * velo.y) * alliveLeverage_);

		//出現遅延
		newData->maxDelayCount = spawnDelayCount_;

		//データを群に送る
		datas_.emplace_back(std::move(newData));
		
		rotate += (diffusion_ * (float)std::numbers::pi) / spawnNum_;

	}


}

Vector2 EffectEnterWater::RotateVelo(const Vector2& velo, float theta)
{
	return {
	std::cos(theta) * velo.x + (-std::sin(theta) * velo.y),
	std::sin(theta) * velo.x + (std::cos(theta) * velo.y),
	};
}

float EffectEnterWater::Radian(float digree)
{
	return (digree * (float)std::numbers::pi) / 180.0f;
}
