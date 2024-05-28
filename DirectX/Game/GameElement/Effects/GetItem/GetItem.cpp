#include "GetItem.h"
#include"ModelDataManager.h"
#include"RandomGenerator/RandomGenerator.h"
#include"TextureManager/TextureManager.h"
#include"Ease/Ease.h"

EffectGetItem::EffectGetItem()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("plane");
	modelData_ = instancingManager_->GetDrawData({ modelData, TextureManager::GetInstance()->LoadTexture("whiteStar.png"), BlendMode::kBlendModeNormal });

}

void EffectGetItem::SetGlobalV()
{
}


EffectGetItem::~EffectGetItem()
{
}

void EffectGetItem::Initialize()
{
	datas_.clear();
}

void EffectGetItem::Update()
{
#pragma region こうsんしょり
	
#pragma region 更新処理
	for(auto & data:datas_) {
		
	}
#pragma endregion


#pragma region 削除処理
	datas_.remove_if([](auto& data) {
		if (data.isDead = true) {
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

void EffectGetItem::Spawn(const Vector3& cPos)
{
	int SpawnNum = RandomGenerator::GetInstance()->RandInt(randSpawnNum_.x, randSpawnNum_.y);

	for (int i = 0; i < SpawnNum; i++) {
		DustData newData;
		newData.pos = cPos;
		newData.velo = RandomGenerator::GetInstance()->RandVector3(-1, 1).Normalize() * RandomGenerator::GetInstance()->RandFloat(randSpd_.x, randSpd_.y);
		newData.velo.z = 0;

		datas_.emplace_back(newData);
	}
}

