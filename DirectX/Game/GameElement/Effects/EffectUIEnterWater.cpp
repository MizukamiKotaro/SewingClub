#include "EffectUIEnterWater.h"
#include"RandomGenerator/RandomGenerator.h"
#include"Ease/Ease.h"

EffectUIEnterWater::EffectUIEnterWater()
{
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

}

void EffectUIEnterWater::Update()
{

	if (isActive_) {
		//出現処理
		if (spawnCount_++ >= maxSpawnCount_) {
			spawnCount_ = 0;

			std::unique_ptr<BubbleData> newData = std::make_unique<BubbleData>();
			newData->sprite = new Sprite();
			newData->sprite->LoadTexture("bubble.png");
			newData->sprite->Initialize();
			newData->sprite->SetAnchorPoint({ 0.5f,0.0f });

			//左右の出現処理
			if (isSpawnLeft_) {
				isSpawnLeft_ = false;
				newData->sprite->pos_.x = (float)RandomGenerator::GetInstance()->RandInt(0, spawnXDiff_);
			}
			else
			{
				isSpawnLeft_ = true;
				newData->sprite->pos_.x = (float)RandomGenerator::GetInstance()->RandInt(1280-spawnXDiff_, 1280);
			}
			newData->sprite->pos_.y = 720;

			//サイズ指定
			float scale = scale_+RandomGenerator::GetInstance()->RandFloat(-scaleDiff_,scaleDiff_);
			newData->sprite->size_ = { scale,scale };

			//速度設定
			newData->velo = Vector2{ 0,-1.0f } *RandomGenerator::GetInstance()->RandFloat(veloDiff.x, veloDiff.y);

			newData->maxDeadCount = maxDeadCount_;

			//データ送信
			datas_.emplace_back(std::move(newData));

		}
	}

#pragma region 演出の更新処理
	for (auto& data : datas_) {
		//速度代入
		data->sprite->pos_ +=  data->velo;


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

	//削除処理
	datas_.remove_if([](auto& data) {
		if (data->isDead) {
			delete data->sprite;
			data->sprite = nullptr;
			return true;
		}
		else
		{
			return false;
		}
		});
#pragma endregion



}

void EffectUIEnterWater::Draw()
{

	for (auto& data : datas_) {
		data->sprite->Draw();
	}

}


