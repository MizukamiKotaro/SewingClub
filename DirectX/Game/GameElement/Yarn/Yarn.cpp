#include "Yarn.h"
#include "Camera.h"
#include "ModelDataManager.h"
#include <algorithm>

InstancingModelManager* Yarn::instancingManager_ = nullptr;
const InstancingMeshTexData* Yarn::modelData_ = nullptr;

std::unique_ptr<GlobalVariableUser> Yarn::globalVariable_ = nullptr;

const std::string Yarn::fNames[kFloatEnd] = {
	"減速率",
	"最低速度",
	"間の最大の長さ",
	"スケール"
};

float Yarn::fParas_[kFloatEnd] = { 1.0f };

Vector3 Yarn::scale_ = { Yarn::fParas_[Yarn::kScale],Yarn::fParas_[Yarn::kScale] ,Yarn::fParas_[Yarn::kScale] };

Yarn::Yarn(const Vector3* parent, const Vector3& ganeratePosition)
{
	parent_ = parent;
	position_ = ganeratePosition;
	ganeratePosition_ = ganeratePosition;
	velocity_ = {};
	rotate_ = Quaternion::Identity();
	child_ = nullptr;
}

void Yarn::StaticInitialize()
{
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("Cube");
	modelData_ = instancingManager_->GetDrawData({ modelData,modelData->texture,BlendMode::kBlendModeNormal });

	globalVariable_ = std::make_unique<GlobalVariableUser>("Yarn", "StaticYarn");
	globalVariable_->CreateGroup();

	SetGlobalVariable();
}

void Yarn::Initialize()
{

}

void Yarn::Update()
{
	float length = ((*parent_) - position_).Length();

	if (length > fParas_[kInterpolationMaxLange]) {
		velocity_ = ((*parent_) - position_) * (length - fParas_[kInterpolationMaxLange]);
	}
	else {
		velocity_ *= fParas_[kAttenuation];
		if (velocity_.Length() <= fParas_[kMinSpeed]) {
			velocity_ = {};
		}
	}

	position_ += velocity_;

	if (position_ != ganeratePosition_ && !child_) {
		child_ = std::make_unique<Yarn>(&position_, ganeratePosition_);
	}

	if (child_) {
		child_->Update();
	}
}

void Yarn::Draw() const 
{
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(scale_, rotate_, position_);
	instancingManager_->AddBox(modelData_, InstancingModelData{ matrix,Matrix4x4::MakeIdentity4x4() ,{1.0f,1.0f,1.0f,1.0f} });

	if (child_) {
		child_->Draw();
	}
}

void Yarn::StaticUpdate()
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG
}

void Yarn::SetGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		globalVariable_->AddItem(fNames[i], fParas_[i]);
	}

	ApplyGlobalVariable();
}

void Yarn::ApplyGlobalVariable()
{
	for (int i = 0; i < kFloatEnd; i++) {
		fParas_[i] = globalVariable_->GetFloatValue(fNames[i]);

		if (i == FloatParamater::kAttenuation) {
			fParas_[i] = std::clamp(fParas_[i], 0.0001f, 1.0f);
		}
		else {
			if (fParas_[i] < 0.0001f) {
				fParas_[i] = 0.0001f;
			}
		}
	}

	scale_ = { fParas_[kScale],fParas_[kScale] ,fParas_[kScale] };
}