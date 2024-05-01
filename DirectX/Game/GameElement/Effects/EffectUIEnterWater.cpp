#include "EffectUIEnterWater.h"
#include"RandomGenerator/RandomGenerator.h"
#include"Ease/Ease.h"

EffectUIEnterWater::EffectUIEnterWater()
{

	datas_.resize(dataNum_);

	
	for (auto& data : datas_) {
		data = std::make_unique<BubbleData>();
		data->sprite = new Sprite();
		data->isDead = true;
		data->sprite->LoadTexture("bubble.png");
		data->sprite->SetAnchorPoint({ 0.5f,0.0f });

	}

	 gVariUser_ = new GlobalVariableUser("Effects", "EffectUIEnterWater", "effe");

	 gVariUser_->AddItem(keys[SpawnCount], maxSpawnCount_);
	 gVariUser_->AddItem(keys[SpawnDiff], spawnXDiff_);
	 gVariUser_->AddItem(keys[Size], scale_);
	 gVariUser_->AddItem(keys[SizeRondom], scaleDiff_);
	 gVariUser_->AddItem(keys[RandomVelo], veloDiff);
	 gVariUser_->AddItem(keys[DeadCount], maxDeadCount_);

}

EffectUIEnterWater::~EffectUIEnterWater()
{
	for (auto& data : datas_) {
		delete data->sprite;
		data->sprite = nullptr;
	}
}

void EffectUIEnterWater::Initialize()
{
	for (auto& data : datas_) {
		data->isDead = true;
	}
}

void EffectUIEnterWater::Update()
{

	if (isActive_) {
		//出現処理
		if (spawnCount_++ >= maxSpawnCount_) {
			spawnCount_ = 0;

			for (auto& data : datas_) {
				if (data->isDead == true) {
					data->isDead = false;
					data->sprite->Initialize();
					
					data->deadCount = 0;
					data->alpha = 1;

					//左右の出現処理
					if (isSpawnLeft_) {
						isSpawnLeft_ = false;
						data->sprite->pos_.x = (float)RandomGenerator::GetInstance()->RandInt(0, spawnXDiff_);
					}
					else
					{
						isSpawnLeft_ = true;
						data->sprite->pos_.x = (float)RandomGenerator::GetInstance()->RandInt(1280 - spawnXDiff_, 1280);
					}
					data->sprite->pos_.y = 720;

					//サイズ指定
					float scale = scale_ + RandomGenerator::GetInstance()->RandFloat(-scaleDiff_, scaleDiff_);
					data->sprite->size_ = { scale,scale };

					//速度設定
					data->velo = Vector2{ 0,-1.0f } *RandomGenerator::GetInstance()->RandFloat(veloDiff.x, veloDiff.y);

					//最大カウント設定
					data->maxDeadCount = maxDeadCount_;

					break;
				}
			}

		}
	}

#pragma region 演出の更新処理
	for (auto& data : datas_) {
		if (!data->isDead) {
			//速度代入
			data->sprite->pos_ += data->velo;


			if (data->deadCount++ >= data->maxDeadCount) {
				data->isDead = true;
				continue;
			}

			float t = data->deadCount / data->maxDeadCount;

			data->alpha = Ease::UseEase(1, 0, t);

			//削除条件処理
			data->sprite->SetColor(Vector4{ 1,1,1,data->alpha });

			data->sprite->Update();
		}
	}

	
#pragma endregion



}

void EffectUIEnterWater::Draw()
{

	for (auto& data : datas_) {
		if (!data->isDead) {
			data->sprite->Draw();
		}
	}

}

void EffectUIEnterWater::Finalize()
{
	for (auto& data : datas_) {
		if (!data->isDead) {
			data->isDead = true;
		}
	}
}

void EffectUIEnterWater::Debug()
{

	maxSpawnCount_ = gVariUser_->GetIntValue(keys[SpawnCount]);
	spawnXDiff_ = gVariUser_->GetIntValue(keys[SpawnDiff]);
	scale_ = gVariUser_->GetFloatValue(keys[Size]);
	scaleDiff_ = gVariUser_->GetFloatValue(keys[SizeRondom]);
	veloDiff = gVariUser_->GetVector2Value(keys[RandomVelo]);
	maxDeadCount_ = gVariUser_->GetFloatValue(keys[DeadCount]);

}


