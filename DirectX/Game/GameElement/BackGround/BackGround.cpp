#include "BackGround.h"
#include "TextureManager.h"
#include "InstancingModelManager.h"
#include "ModelDataManager.h"
#include "RandomGenerator/RandomGenerator.h"

InstancingModelManager* BackGround::instancingManager_ = nullptr;
const InstancingMeshTexData* BackGround::modelData_ = nullptr;

BackGround::BackGround() {
	back_ = std::make_unique<Sprite>("white.png", Vector2(640.0f, 360.0f), Vector2(0.0f, 0.0f), Vector2(2.2f, 1.2f), Vector4(0.09f, 0.09f, 0.13f, 1.0f));
	back_->size_ = Vector2(2.2f, 1.2f);
	for (auto& i : starlist_) {
		i = std::make_unique<Star>();
	}
	StaticInitialize();
}

void BackGround::StaticInitialize() {
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("plane");
	const Texture* tex_ptr = TextureManager::GetInstance()->LoadTexture("whiteStar.png");
	modelData_ = instancingManager_->GetDrawData({ modelData,tex_ptr,BlendMode::kBlendModeScreen });
}

void BackGround::Update(Camera* camera) {
	static Vector2 size;
	back_->Update();
	Vector3 randnumber = camera->transform_.worldPos_;
	const Vector3 cameraScale = Vector3(25.0f, 15.0f, 0.0f);
	randnumber += RandomGenerator::GetInstance()->RandVector3(-cameraScale, cameraScale);
	randnumber.z = 0.0f;
	for (auto& i : starlist_) {
		if (i->Update()) {
			i->Initialize(randnumber);
			break;
		}
	}
}

void BackGround::Draw() {
	back_->Draw(BlendMode::kBlendModeScreen);
	Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	for (auto& i : starlist_) {
		instancingManager_->AddBox(modelData_, InstancingModelData{ i->GetTrans().worldMat_, Matrix4x4::MakeIdentity4x4(), color});
	}
}
