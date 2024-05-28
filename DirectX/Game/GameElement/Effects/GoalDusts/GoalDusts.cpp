#include "GoalDusts.h"
#include"RandomGenerator/RandomGenerator.h"
#include"TextureManager.h"
#include"calc.h"

EffectGoalDusts::EffectGoalDusts()
{

	instancingManager_ = InstancingModelManager::GetInstance();
	for (int i = 0; i < 3; i++) {
		const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("plane");
		modelData_[i] = instancingManager_->GetDrawData({modelData,TextureManager::GetInstance()->LoadTexture(dustPaths_[i]),BlendMode::kBlendModeNormal});
	}
	gvu_ = new GlobalVariableUser("Effects", "EffectGoalDust");
	gvu_->AddItem(keys[MaxPreCount], preMaxCount);
	gvu_->AddItem(keys[MaxCount], maxCount_);
	gvu_->AddItem(keys[IsRandomRotate], randomRotate_);
	gvu_->AddItem(keys[IsRandomColor], randomColor_);
	gvu_->AddItem(keys[IsRandomTexture], randomTexture_);
	gvu_->AddItem(keys[SelectTex], selectTex);
	gvu_->AddItem(keys[RandSpawnRadius], randSpawnRadius_);
	gvu_->AddItem(keys[RandSpd], randSpd_);
	gvu_->AddItem(keys[StedSize], stedSize_);
	gvu_->AddItem(keys[StedAlpha], stedAlpha_);


}

void EffectGoalDusts::GetGlobalV()
{
	preMaxCount = gvu_->GetFloatValue(keys[MaxPreCount]);
	maxCount_ = gvu_->GetFloatValue(keys[MaxCount]);
	randomRotate_ = gvu_->GetBoolValue(keys[IsRandomRotate]);
	randomColor_ = gvu_->GetBoolValue(keys[IsRandomColor]);
	randomTexture_ = gvu_->GetBoolValue(keys[IsRandomTexture]);
	selectTex = gvu_->GetIntValue(keys[SelectTex]);
	randSpawnRadius_ = gvu_->GetVector2Value(keys[RandSpawnRadius]);
	randSpd_ = gvu_->GetVector2Value(keys[RandSpd]);
	stedSize_ = gvu_->GetVector2Value(keys[StedSize]);
	stedAlpha_ = gvu_->GetVector2Value(keys[StedAlpha]);
}

void EffectGoalDusts::SetActive(bool active)
{
	isActive_ = active;
}

EffectGoalDusts::~EffectGoalDusts()
{
}

void EffectGoalDusts::Initialize(const Vector3&gpos)
{
	datas_.clear();

	goalPos_ = &gpos;

	GetGlobalV();
}

void EffectGoalDusts::Update()
{
#ifdef _DEBUG
	GetGlobalV();
#endif // _DEBUG

#pragma region 出現処理
	if (count_++ >= maxCount_) {
		count_ = 0;

		DustData newdata;
		//生成座標作成
		//ランダムな方向に作成
		Vector3 pos = {
			RandomGenerator::GetInstance()->RandFloat(-1,1),
			RandomGenerator::GetInstance()->RandFloat(-1,1),
			0
		};
		newdata.pos =*goalPos_+ pos.Normalize() * RandomGenerator::GetInstance()->RandFloat(randSpawnRadius_.x, randSpawnRadius_.y);

		//速度作成
		newdata.velo = (*goalPos_ - newdata.pos).Normalize() * RandomGenerator::GetInstance()->RandFloat(randSpd_.x, randSpd_.y);

		//回転ランダム有効の場合
		if (randomRotate_) {
			newdata.rotate = RandomGenerator::GetInstance()->RandFloat(0, 3.14f * 2.0f);
		}
		if (randomColor_) {
			newdata.color = RandomGenerator::GetInstance()->RandVector3({ 0,0,0 }, { 1,1,1 });
		}
		if (randomTexture_) {
			newdata.texType = (textureType)RandomGenerator::GetInstance()->RandInt(0, _countTexture-1);
		}

		newdata.maxDirection = (*goalPos_ - newdata.pos).Length();

		//データ追加
		datas_.emplace_back(newdata);
	}
#pragma endregion

#pragma region 更新処理
	for (auto& data : datas_) {
		//移動処理
		data.pos += data.velo;
		//距離取得でT取得
		data.direction = ((*goalPos_) - data.pos).Length();
		float t = data.direction / data.maxDirection;
		//サイズ再設定
		float scale = Calc::Lerp(stedSize_.y, stedSize_.x, t);
		data.size = { scale,scale ,scale };
		//透明度再設定
		data.alpha = Calc::Lerp(stedAlpha_.y, stedAlpha_.x, t);

		//移動方向とゴールへの向きベクトル反転で死ぬ
		if (data.direction<=0.1f) {
			data.IsDead_ = true;
		}
	}

	datas_.remove_if([](auto& data) {
		if (data.IsDead_) {
			return true;
		}
		else {
			return false;
		}
		});
#pragma endregion

}

void EffectGoalDusts::Draw()
{
	for (auto& data : datas_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(data.size, Vector3(0, 0, data.rotate), data.pos);
		instancingManager_->AddBox(modelData_[data.texType], InstancingModelData{matrix ,Matrix4x4::MakeIdentity4x4(), {data.color.x,data.color.y,data.color.z,data.alpha}});
	}
}


