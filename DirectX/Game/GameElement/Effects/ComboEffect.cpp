#include "ComboEffect.h"
#include "TextureManager/TextureManager.h"
#include <GameElement/Enemy/IEnemy.h>
//#include <numbers>
#include <random>

ComboEffectManager::ComboEffectManager() {
	instancingManager_ = ParticleManager::GetInstance();
	uint32_t index = 0u;
	for (auto& data : modelData_) {
		data = instancingManager_->GetDrawData(
			ParticleMeshTexData{ ModelDataManager::GetInstance()->LoadObj("plane"),
			TextureManager::GetInstance()->LoadTexture(texturePath_.at(index++)),BlendMode::kBlendModeNormal });
	}

}

ComboEffectManager* ComboEffectManager::GetInstance() {
	static ComboEffectManager instance;
	return &instance;
}

void ComboEffectManager::Update(const float& delta) {
	// 更新処理
	for (auto& model : effectContiner_) {
		model.Update(delta);
	}
}

void ComboEffectManager::Create(const Vector3& playerPosition) {
	// 生成処理
	for (auto& model : effectContiner_) {
		// 今動いていなければ
		if (!model.GetActive()) {
			model.Initialize(playerPosition, RandNum(0, static_cast<int>(modelData_.size())));
			return;
		}
	}
}

void ComboEffectManager::Draw() {
	for (auto& model : effectContiner_) {
		model.Draw(instancingManager_, modelData_.at(model.GetIndex()));
	}
}

uint32_t ComboEffectManager::RandNum(int min, int max) {

	//	ハードウェア乱数をシードにして初期化
	std::random_device seedGen;
	//	メルセンヌツイスター法で疑似乱数生成器の作成
	std::mt19937_64 randNum(seedGen());
	//	(min,max)の範囲で等間隔に乱数を生成 一様実数分布
	std::uniform_real_distribution<> dist(min, max);

	return static_cast<int>(dist(randNum));
}

void ComboEffect::Initialize(const Vector3& playerpos, const uint32_t& index) {
	meshIndex_ = index;
	position_ = playerpos;
	position_.z -= 2.0f;
	isActive_ = true;
	nowframe_ = 0.0f;
	scale_ = Vector2(3.0f, 30.0f);
	color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

void ComboEffect::Update(const float& delta) {
	if (!isActive_) { return; }

	// とりあえず一定時間たったら消すにする
	nowframe_ += delta;
	if (nowframe_ >= 10.0f) {
		isActive_ = false;
	}

}

void ComboEffect::Draw(ParticleManager* instancingManager, const ParticleMeshTexData* data) const {
	if (!isActive_) { return; }
	instancingManager;
	ParticleManager* instance = ParticleManager::GetInstance();
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_.x,scale_.y,1.0f }, Vector3{ 0.0f,0.0f,0.0f }, position_);
	instance->AddParticle(ParticleData{ matrix,Matrix4x4::MakeIdentity4x4(), color_ }, data);
}
