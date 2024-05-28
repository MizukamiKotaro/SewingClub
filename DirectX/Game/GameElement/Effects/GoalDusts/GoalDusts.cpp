#include "GoalDusts.h"
#include"RandomGenerator/RandomGenerator.h"
#include"TextureManager.h"
#include"calc.h"
#include<numbers>

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

void EffectGoalDusts::Initialize()
{
	datas_.clear();

	

	GetGlobalV();
}

void EffectGoalDusts::Update(Vector3& gpos)
{
#ifdef _DEBUG
	GetGlobalV();
#endif // _DEBUG

	rotateR_ += 1.0f / 180.0f;
	rotateR_ = std::fmod(rotateR_, (float)std::numbers::pi*2.0f);
	
	goalPos_ = gpos;
#pragma region 出現処理
	
	float maxC;
	if (isActive_) {
		maxC = maxCount_;
	}
	else {
		maxC = preMaxCount;
	}

	if (count_++ >= maxC) {
		count_ = 0;

		DustData newdata;
		//生成座標作成
		//ランダムな方向に作成
		Vector3 pos = {
			RandomGenerator::GetInstance()->RandFloat(-1,1),
			RandomGenerator::GetInstance()->RandFloat(-1,1),
			0
		};
		newdata.pos = pos.Normalize() * RandomGenerator::GetInstance()->RandFloat(randSpawnRadius_.x, randSpawnRadius_.y);
		newdata.stPos = newdata.pos;
		//速度作成
		newdata.maxCount = RandomGenerator::GetInstance()->RandFloat(randSpd_.x,randSpd_.y);

		//回転ランダム有効の場合
		if (randomRotate_) {
			newdata.rotate = RandomGenerator::GetInstance()->RandFloat(0, 3.14f * 2.0f);
		}
		if (randomColor_) {
			newdata.color = RandomGenerator::GetInstance()->RandVector3({ 0,0,0 }, { 1,1,1 });
		}
		else {
			newdata.color = { 1,1,1 };
		}
		if (randomTexture_) {
			newdata.texType = (textureType)RandomGenerator::GetInstance()->RandInt(0, _countTexture-1);
		}
		else {
			newdata.texType = (textureType)selectTex;
		}

		
		//データ追加
		datas_.emplace_back(newdata);
	}
#pragma endregion

#pragma region 更新処理
	for (auto& data : datas_) {
		float t = data.count / data.maxCount;

		//移動処理
		data.pos = Calc::Lerp(data.stPos,{0,0,0}, t);
		//サイズ再設定
		float scale = Calc::Lerp(stedSize_.x, stedSize_.y, t);
		data.size = { scale,scale ,scale };
		//透明度再設定
		data.alpha = Calc::Lerp(stedAlpha_.x, stedAlpha_.y, t);

		//移動方向とゴールへの向きベクトル反転で死ぬ
		if (data.count++>=data.maxCount) {
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
	Matrix4x4 pmatrix = Matrix4x4::MakeAffinMatrix({1,1,1}, Vector3(0, 0, rotateR_), goalPos_);
	for (auto& data : datas_) {
		
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(data.size, Vector3(0, 0, data.rotate), data.pos);

		matrix =matrix*pmatrix;

		instancingManager_->AddBox(modelData_[data.texType], InstancingModelData{matrix ,Matrix4x4::MakeIdentity4x4(), {data.color.x,data.color.y,data.color.z,data.alpha}});
	}
}


