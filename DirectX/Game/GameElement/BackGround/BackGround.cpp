#include "BackGround.h"
#include "TextureManager.h"
#include "InstancingModelManager.h"
#include "RandomGenerator/RandomGenerator.h"

InstancingModelManager* BackGround::instancingManager_ = nullptr;
const InstancingMeshTexData* BackGround::modelData_ = nullptr;

BackGround::BackGround() {
	back_ = std::make_unique<Model>("plane");
	back_->SetTexture(TextureManager::GetInstance()->LoadTexture("white.png"));
	back_->SetColor(Vector4(0.09f,0.09f,0.13f,1.0f));
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	back_->transform_.scale_ = Vector3(22.5f, 15.0f, 0.1f);
	back_->transform_.translate_.z = 30.0f;
	for (auto& i : starlist_) {
		i = std::make_unique<Star>();
	}
	StaticInitialize();
}

void BackGround::StaticInitialize() {
	instancingManager_ = InstancingModelManager::GetInstance();
	const ModelData* modelData = ModelDataManager::GetInstance()->LoadObj("plane");
	const Texture* tex_ptr = TextureManager::GetInstance()->LoadTexture("star1.png");
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
	back_->Draw(*camera_);
	Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	for (auto& i : starlist_) {
		instancingManager_->AddBox(modelData_, InstancingModelData{ i->GetTrans().worldMat_, Matrix4x4::MakeIdentity4x4(), color});
	}
}
