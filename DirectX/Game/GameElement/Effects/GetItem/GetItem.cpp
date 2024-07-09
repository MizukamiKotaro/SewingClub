#include "GetItem.h"
#include"ModelDataManager.h"
#include"RandomGenerator/RandomGenerator.h"
#include"TextureManager/TextureManager.h"
#include"Ease/Ease.h"



void EffectGetItem::SetGlobalV()
{
	maxCount_ = gvu_->GetIntValue(keys[kMaxCount]);
	maxScale = gvu_->GetFloatValue(keys[kMaxScale]);
	maxFlashing = gvu_->GetIntValue(keys[kMaxFlash]);
	randSpd_ = gvu_->GetVector2Value(keys[kRandSpd]);
	maxSpd_ = gvu_->GetFloatValue(keys[kMaxSpd]);
	randSpawnNum_ = gvu_->GetVector2Value(keys[kRandSapwn]);
	gravity_ = gvu_->GetFloatValue(keys[kGravity]);

}




EffectGetItem* EffectGetItem::GetInstance()
{
	static EffectGetItem instance;
	return &instance;
}

void EffectGetItem::ModelLoad()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("plane");
	modelData_ = instancingManager_->GetDrawData({ modelData, TextureManager::GetInstance()->LoadTexture("effect_star.png"), BlendMode::kBlendModeNormal });

	gvu_ = new GlobalVariableUser("Effect", "GetItem");

	gvu_->AddItem(keys[kMaxCount], maxCount_);
	gvu_->AddItem(keys[kMaxScale], maxScale);
	gvu_->AddItem(keys[kMaxFlash], maxFlashing);
	gvu_->AddItem(keys[kRandSpd], randSpd_);
	gvu_->AddItem(keys[kMaxSpd], maxSpd_);
	gvu_->AddItem(keys[kRandSapwn], randSpawnNum_);
	gvu_->AddItem(keys[kGravity], gravity_);

}

void EffectGetItem::Initialize()
{

	datas_.clear();

	SetGlobalV();
}

void EffectGetItem::Update()
{
#ifdef _DEBUG
	SetGlobalV();
#endif // _DEBUG



#pragma region こうsんしょり

#pragma region 更新処理
	for (auto& data : datas_) {

		//サイズアップ時処理
		if (data.isSizeUp_) {
			if (data.count++ > maxCount_) {
				data.isSizeUp_ = false;
				data.count = 0;
				data.scale = maxScale;
			}

			float t = (float)data.count / (float)maxCount_;

			data.scale = Ease::UseEase(0, maxScale, t);
		}
		else {
			//サイズダウン時処理
			if (data.count++ >= maxCount_) {
				data.scale = 0;
				//既定カウント数で死亡
				if (data.flashingCount++ >= maxFlashing) {
					data.isDead = true;
					continue;
				}
				else {
					data.count = 0;
					data.isSizeUp_ = true;
				}
			}
			float t = float(data.count) / float(maxCount_);

			data.scale = Ease::UseEase(maxScale, 0, t);

		}

	

	//移動処理
	data.velo.y -= gravity_;
	//速度超過
	if (data.velo.Length() > maxSpd_) {
		data.velo = data.velo.Normalize() * maxSpd_;
	}

	data.pos += data.velo;
}
#pragma endregion


#pragma region 削除処理
datas_.remove_if([](auto& data) {
	if (data.isDead == true) {
		return true;
	}
	else {
		return false;
	}
	});
#pragma endregion

#pragma endregion

}

void EffectGetItem::Draw()
{
	for (auto& data : datas_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ data.scale,data.scale ,1 }, Vector3{ 0,0,0 }, data.pos);
		instancingManager_->AddBox(modelData_, InstancingModelData{ matrix ,Matrix4x4::MakeIdentity4x4(), {1,1,1,1} });
	}
}

void EffectGetItem::Finalize()
{
	datas_.clear();
}

void EffectGetItem::Spawn(const Vector3& cPos)
{
	int SpawnNum = RandomGenerator::GetInstance()->RandInt((int)randSpawnNum_.x, (int)randSpawnNum_.y);

	for (int i = 0; i < SpawnNum; i++) {
		DustData newData;
		newData.pos = cPos;
		newData.velo = RandomGenerator::GetInstance()->RandVector3(-1, 1);
		newData.velo.z = 0;
		newData.velo = newData.velo.Normalize() * RandomGenerator::GetInstance()->RandFloat(randSpd_.x, randSpd_.y);

		datas_.emplace_back(newData);
	}
}

