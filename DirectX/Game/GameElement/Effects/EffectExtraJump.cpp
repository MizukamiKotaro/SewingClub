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
				newData->velo = { 0,0,0 };
				newData->maxCount_ = dustDeadCount_;

				newData->maxScale = maxScale;

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
				data->isDead_ = true;
				data->scale = 0;
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
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{data->scale,data->scale ,1}, Vector3{ 0,0,0 }, data->translate);
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix ,Matrix4x4::MakeIdentity4x4(), {1,1,1,1} });
	}

}

void EffectExtraJump::Debug()
{
#ifdef _DEBUG
	ImGui::Begin("ジャンプ時の演出");
	ImGui::DragFloat2("エフェクト発生範囲", &spawnAreaSize_.x,0.1f);
	ImGui::DragFloat("エフェクトの最大サイズ", &maxScale,0.01f);
	ImGui::DragInt("エフェクト発生間隔", &spawnInterval_);
	ImGui::DragInt("エフェクト拡縮ごとのカウント", &dustDeadCount_);
	ImGui::End();
#endif // _DEBUG

}

void EffectExtraJump::SpawnEffect(bool isAllive)
{

	//カウントをリセット
	count_ = 10000;
	isActive_ = isAllive;

}
