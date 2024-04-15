#include "EffectExtraJump.h"
#include"ModelDataManager.h"
#include"RandomGenerator/RandomGenerator.h"

EffectExtraJump::EffectExtraJump()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	modelData_ = ModelDataManager::GetInstance()->LoadObj("WaterCircle");
}

EffectExtraJump::~EffectExtraJump()
{
}

void EffectExtraJump::Initialize(const Vector3& playerPos)
{
	playerPos_ = &playerPos;

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
					playerPos_->x + RandomGenerator::GetInstance()->RandFloat(-spawnAreaSize_.y,spawnAreaSize_.y),
					1
				};
				newData->velo = { 0,0,0 };
				newData->maxCount_ = dustDeadCount_;

				//データ送信
				datas_.emplace_back(std::move(newData));
			}
#pragma endregion
#pragma region 塵のアニメーション処理
			for (auto& data : datas_) {
				data->translate += data->velo;

				if (data->count++ >= data->maxCount_) {
					data->isDead_ = true;
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


		}//無い場合
		else {
			count_ = 0;
			isActive_ = false;
		}

	}

}

void EffectExtraJump::Draw()
{
	for (auto& data : datas_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(scale, Vector3{0,0,0}, data->translate);
		instancingManager_->AddBox(modelData_, InstancingModel{ matrix ,{1,1,1,1} });
	}

}

void EffectExtraJump::SpawnEffect(int count)
{

	//カウントをリセット
	count_ = count;
	isActive_ = true;

}
