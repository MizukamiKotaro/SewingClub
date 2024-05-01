#include "EffectExtraJump.h"
#include"ModelDataManager.h"
#include"RandomGenerator/RandomGenerator.h"
#include"TextureManager/TextureManager.h"
#include"Ease/Ease.h"
#include"ImGuiManager/ImGuiManager.h"

EffectExtraJump::EffectExtraJump()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("plane");
	modelData_ = instancingManager_->GetDrawData({ modelData, TextureManager::GetInstance()->LoadTexture("whiteStar.png"), BlendMode::kBlendModeNormal });

	gVU_= new GlobalVariableUser("Effects", "EffectExtraJump");

	gVU_->AddItem(keys[SpawnAreaSize], spawnAreaSize_);
	gVU_->AddItem(keys[DustMaxScale], maxScale);
	gVU_->AddItem(keys[RandVelo], randVelo_);
	gVU_->AddItem(keys[SpawnInterval], spawnInterval_);
	gVU_->AddItem(keys[DustDead], dustDeadCount_);
	gVU_->AddItem(keys[Blinking], maxblinkingCount_);

}

EffectExtraJump::~EffectExtraJump()
{
}

void EffectExtraJump::Initialize(const Vector3* playerPos)
{
	playerPos_ = playerPos;

	datas_.clear();
	isActive_ = false;
	count_ = 0;
}

void EffectExtraJump::Update()
{
	//生きているときの処理
	if (isActive_) {

		//生存カウントがある場合
		if (count_-- > 0) {

#pragma region 塵生成処理
			if (spawnIntervalCount_++ >= spawnInterval_) {
				spawnIntervalCount_ = 0;

				std::unique_ptr<Dust> newData = std::make_unique<Dust>();
				//座標設定
				newData->translate = {
					playerPos_->x + RandomGenerator::GetInstance()->RandFloat(-spawnAreaSize_.x,spawnAreaSize_.x),
					playerPos_->y + RandomGenerator::GetInstance()->RandFloat(-spawnAreaSize_.y,spawnAreaSize_.y),
					1
				};

				//newData->translate = *playerPos_;
				newData->velo = RandomGenerator::GetInstance()->RandVector3(-1, 1).Normalize() * RandomGenerator::GetInstance()->RandFloat(randVelo_, randVelo_);
				newData->velo.z = 0;
				newData->maxCount_ = dustDeadCount_ / 2;

				newData->maxScale = maxScale;

				newData->maxTenmetu = maxblinkingCount_;

				//データ送信
				datas_.emplace_back(std::move(newData));
			}
#pragma endregion


		}//無い場合
		else {
			//count_ = 0;
			//isActive_ = false;
		}

	}
	else
	{
	}


#pragma region 塵のアニメーション処理
	for (auto& data : datas_) {
		data->translate += data->velo;

		if (data->isSizeUp) {
			if (data->count++ >= data->maxCount_) {
				data->isSizeUp = false;
				data->count = 0;
				data->scale = data->maxScale;
			}
			float t = float(data->count) / float(data->maxCount_);

			data->scale = Ease::UseEase(0, data->maxScale, t);
		}
		else
		{
			if (data->count++ >= data->maxCount_) {
				data->scale = 0;

				if (data->tenmetuCount++ >= data->maxTenmetu) {
					data->isDead_ = true;
				}
				else {
					data->count = 0;
					data->isSizeUp = true;
				}
			}
			float t = float(data->count) / float(data->maxCount_);

			data->scale = Ease::UseEase(data->maxScale, 0, t);
		}



	}

	//削除処理
	datas_.remove_if([](auto& data) {
		if (data->isDead_) {
			return true;
		}
		else
		{
			return false;
		}
		});
#pragma endregion

}

void EffectExtraJump::Draw()
{
	for (auto& data : datas_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ data->scale,data->scale ,1 }, Vector3{ 0,0,0 }, data->translate);
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix ,Matrix4x4::MakeIdentity4x4(), {1,1,1,1} });
	}

}

void EffectExtraJump::Debug()
{

	spawnAreaSize_ = gVU_->GetVector2Value(keys[SpawnAreaSize]);
	maxScale = gVU_->GetFloatValue(keys[DustMaxScale]);
	randVelo_ = gVU_->GetFloatValue(keys[RandVelo]);
	spawnInterval_ = gVU_->GetIntValue(keys[SpawnInterval]);
	dustDeadCount_ = gVU_->GetIntValue(keys[DustDead]);
	maxblinkingCount_ = gVU_->GetIntValue(keys[Blinking]);


}

void EffectExtraJump::SpawnEffect(bool isAllive)
{

	//カウントをリセット
	count_ = 10000;
	isActive_ = isAllive;

}
