#include "Model.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "ModelDataManager.h"
#include "Camera.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "calc.h"

Model::Model(const std::string& fileName)
{
	modelData_ = modelDataManager_->LoadObj(fileName);

	texture_ = modelData_->texture;

	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;

	CreateResources();

	InitVariables();
}

Model::Model(const ModelData* modelData)
{
	modelData_ = modelData;

	texture_ = modelData_->texture;

	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;

	CreateResources();

	InitVariables();
}

Model::~Model()
{
	transformationResource_->Release();
	materialResource_->Release();
}

void Model::StaticInitialize()
{
	modelDataManager_ = ModelDataManager::GetInstance();
	commandList_ = DirectXBase::GetInstance()->GetCommandList();
	psoManager_ = GraphicsPipelineManager::GetInstance();
}

void Model::Initialize()
{
	
}

void Model::Update()
{
	transform_.UpdateMatrix();

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

void Model::PreDraw() 
{
	psoManager_->PreDraw(pipelineType_);
}

void Model::Draw(const Camera& camera, BlendMode blendMode)
{
	PreDraw();

	materialData_->color = color_;

	if (animation_) {
		NodeAnimation& rootNodeAnimation = animation_->nodeAnimations[modelData_->rootNode.name];
		Vector3 translate = CalculateValue(rootNodeAnimation.translate, animationTime_);
		Quaternion rotate = CalculateValue(rootNodeAnimation.rotate, animationTime_);
		Vector3 scale = CalculateValue(rootNodeAnimation.scale, animationTime_);
		Matrix4x4 localMatrix = Matrix4x4::MakeAffinMatrix(scale, rotate, translate);

		transformationData_->World = localMatrix * transform_.worldMat_;
		transformationData_->WVP = localMatrix * transform_.worldMat_ * camera.GetViewProjection();
		transformationData_->WorldInverse = Matrix4x4::Inverse(Matrix4x4::MakeScaleMatrix(transform_.scale_)) *
			Matrix4x4::MakeRotateXYZMatrix(transform_.rotate_) * Matrix4x4::MakeTranslateMatrix(transform_.translate_);
	}
	else {
		transformationData_->World = transform_.worldMat_;
		transformationData_->WVP = transform_.worldMat_ * camera.GetViewProjection();
		transformationData_->WorldInverse = Matrix4x4::Inverse(Matrix4x4::MakeScaleMatrix(transform_.scale_)) *
			Matrix4x4::MakeRotateXYZMatrix(transform_.rotate_) * Matrix4x4::MakeTranslateMatrix(transform_.translate_);
	}

	materialData_->uvTransform = uvMatrix_;

	psoManager_->SetBlendMode(pipelineType_, blendMode);

	//Spriteの描画。変更に必要なものだけ変更する
	commandList_->IASetVertexBuffers(0, 1, &modelData_->mesh.vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformationResource_->GetGPUVirtualAddress());

	//平行光源CBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(3, light_.GetDirectionalLightGPUVirtualAddress());
	// カメラの設定
	commandList_->SetGraphicsRootConstantBufferView(4, camera.GetGPUVirtualAddress());
	// pointLight の設定
	commandList_->SetGraphicsRootConstantBufferView(5, light_.GetPointLightGPUVirtualAddress());
	// spotLight の設定
	commandList_->SetGraphicsRootConstantBufferView(6, light_.GetSpotLightGPUVirtualAddress());

	commandList_->SetGraphicsRootDescriptorTable(2, srvGPUDescriptorHandle_);
	//描画!!!!（DrawCall/ドローコール）
	commandList_->DrawInstanced(UINT(modelData_->mesh.verteces.size()), 1, 0, 0);

}

void Model::AnimationUpdate(float time)
{
	if (animation_) {
		animationTime_ += time;
		animationTime_ = std::fmod(animationTime_, animation_->duration);
	}
}

void Model::LoadGLTF(const std::string& fileName)
{
	modelData_ = modelDataManager_->LoadGLTF(fileName);

	texture_ = modelData_->texture;

	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;

	LoadAnimation(fileName);
}

void Model::LoadAnimation(const std::string& fileName)
{
	animationTime_ = 0.0f;
	animation_ = std::make_unique<Animation>(modelDataManager_->LoadAnimation(fileName));
}

void Model::SetTexture(const Texture* texture)
{
	texture_ = texture;

	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;
}

void Model::SetModelData(const ModelData* modelData)
{
	modelData_ = modelData;

	texture_ = modelData_->texture;

	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;
}

void Model::SetLight(const ILight* light)
{
	light_.SetLight(light);
}

Vector3 Model::CalculateValue(const AnimationCurve<Vector3>& keyframes, const float& time)
{
	assert(!keyframes.keyframes.empty());
	if (keyframes.keyframes.size() == 1 || time <= keyframes.keyframes[0].time) {
		return keyframes.keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.keyframes.size() - 1; index++) {
		size_t nextIndex = index + 1;
		if (keyframes.keyframes[index].time <= time && time <= keyframes.keyframes[nextIndex].time) {
			float t = (time - keyframes.keyframes[index].time) / (keyframes.keyframes[nextIndex].time - keyframes.keyframes[index].time);
			return Calc::Lerp(keyframes.keyframes[index].value, keyframes.keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.keyframes.rbegin()).value;
}

Quaternion Model::CalculateValue(const AnimationCurve<Quaternion>& keyframes, const float& time) {
	assert(!keyframes.keyframes.empty());
	if (keyframes.keyframes.size() == 1 || time <= keyframes.keyframes[0].time) {
		return keyframes.keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.keyframes.size() - 1; index++) {
		size_t nextIndex = index + 1;
		if (keyframes.keyframes[index].time <= time && time <= keyframes.keyframes[nextIndex].time) {
			float t = (time - keyframes.keyframes[index].time) / (keyframes.keyframes[nextIndex].time - keyframes.keyframes[index].time);
			return Quaternion::Slerp(keyframes.keyframes[index].value, keyframes.keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.keyframes.rbegin()).value;
}

void Model::CreateResources()
{
	CreateMaterialResource();

	CreateTransformationResource();
}

void Model::CreateMaterialResource()
{
	materialResource_ = DirectXBase::CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = 3;
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();
	materialData_->shininess = 40.0f;
	materialData_->supeqularColor = { 1.0f, 1.0f, 1.0f };
}

void Model::CreateTransformationResource()
{
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformationResource_ = DirectXBase::CreateBufferResource(sizeof(TransformationMatrix));
	transformationData_ = nullptr;
	transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData_));
	*transformationData_ = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4(), Matrix4x4::Inverse(Matrix4x4::MakeIdentity4x4()) };
}

void Model::InitVariables()
{
	light_.Initialize();

	transform_ = Transform();

	uvScale_ = { 1.0f,1.0f,1.0f };
	uvRotate_ = { 0.0f,0.0f,0.0f };
	uvPos_ = { 0.0f,0.0f,0.0f };

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}



