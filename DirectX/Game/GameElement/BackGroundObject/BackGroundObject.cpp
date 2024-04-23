#include "BackGroundObject.h"
#include "ModelDataManager.h"
#include <algorithm>
#include "TextureManager.h"
#include "BackGroundObjectConfig.h"

InstancingModelManager* BackGroundObject::instancingManager_ = nullptr;

BackGroundObject::BackGroundObject(const int& no, const BackGroundObjectType& type)
{
	position_ = { 0.0f,1.0f,-0.02f };
	scale_ = { 1.0f,1.0f,1.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	no_ = no;

	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("plane");
	const Texture* tex = nullptr;
	
	switch (type)
	{
	case BackGroundObjectType::CLOUD:
		CreateStageEditor("雲");
		tex = TextureManager::GetInstance()->LoadTexture("back_obj.png");
		break;
	default:
		break;
	}
	modelData_ = instancingManager_->GetDrawData({ modelData,tex,BlendMode::kBlendModeNormal });
	SetGlobalVariable();
}

void BackGroundObject::StaticInitialize()
{
	instancingManager_ = InstancingModelManager::GetInstance();
}

void BackGroundObject::Initialize()
{

}

void BackGroundObject::Update(float deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
	if (stageEditor_->IsTreeOpen(3)) {
		color_ = { 1.0f,0.3f,0.3f,1.0f };
	}
	else if (stageEditor_->IsTreeOpen(4)) {
		color_ = { 0.7f,0.6f,0.1f,1.0f };
	}
	else {
		color_ = { 1.0f,1.0f,0.3f,1.0f };
	}
#endif // _DEBUG

	deltaTime = deltaTime;
}

void BackGroundObject::Draw() const
{
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(scale_, rotate_, position_);
	instancingManager_->AddBox(modelData_, InstancingModelData{ matrix,Matrix4x4::MakeIdentity4x4(), color_ });
}

void BackGroundObject::SetGlobalVariable()
{
	stageEditor_->AddItem("ポジション", position_);
	stageEditor_->AddItem("スケール", scale_);
	stageEditor_->AddItem("角度", rotate_);
	stageEditor_->AddItem("カラー", Vector3{ 1.0f,1.0f,1.0f });
	ApplyGlobalVariable();
}

void BackGroundObject::ApplyGlobalVariable()
{
	position_ = stageEditor_->GetVector3Value("ポジション");
	scale_ = stageEditor_->GetVector3Value("スケール");
	rotate_ = stageEditor_->GetVector3Value("角度");
	Vector3 color = stageEditor_->GetVector3Value("カラー");
	color_ = { color.x,color.y,color.z,1.0f };
}

void BackGroundObject::CreateStageEditor(const std::string& name)
{
	std::string tree = name + std::to_string(no_);
	int no = no_ / 10;
	no = no * 10;
	std::string tree1 = name + std::to_string(no) + "～" + std::to_string(no + 9);

	stageEditor_ = std::make_unique<StageEditor>("背景オブジェクトの配置", "雲の配置", tree1, tree);
}
