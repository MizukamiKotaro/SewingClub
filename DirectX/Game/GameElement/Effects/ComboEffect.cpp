#include "ComboEffect.h"
#include "TextureManager/TextureManager.h"
#include <GameElement/Enemy/IEnemy.h>
#include "Ease/Ease.h"
#include <numbers>
#include "Math/calc.h"
#include "RandomGenerator/RandomGenerator.h"

ComboEffectManager::ComboEffectManager() {
	instancingManager_ = ParticleManager::GetInstance();
	uint32_t index = 0u;
	for (auto& data : modelData_) {
		data = instancingManager_->GetDrawData(
			ParticleMeshTexData{ ModelDataManager::GetInstance()->LoadObj("plane"),
			TextureManager::GetInstance()->LoadTexture(texturePath_.at(index++)),BlendMode::kBlendModeNormal });
	}

	for (int i = 0; i < SEType::kMaxNumber; i++) {
		se_[i].LoadMP3(sePath_[i], seText_[i]);
	}
}

ComboEffectManager::~ComboEffectManager() {
	for (int i = 0; i < SEType::kMaxNumber; i++) {
		se_[i].Stop();
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
	catchEffect_.Update(delta);
}

int32_t ComboEffectManager::Create(const Vector3& playerPosition) {
	// 生成処理
	for (auto& model : effectContiner_) {
		// 今動いていなければ
		if (!model.GetActive()) {
			int randNum;
			// 番号が前と同じならば再抽選
			do {
				// 乱数の生成
				randNum = RandomGenerator::GetInstance()->RandInt(0, static_cast<int>(MeshData::YAY) + 1);
			}while (randNum == oldRandNumber_);

			model.Initialize(playerPosition, randNum);
			if (randNum != YAY) {
				se_[sVeryGood].Stop();
				se_[sGood].Play();
			}
			else {
				se_[sGood].Stop();
				se_[sVeryGood].Play();
			}
			oldRandNumber_ = randNum;
			return randNum;
		}
	}
	return -1;
}

void ComboEffectManager::CreateCatch(const Vector3& playerPosition) {
	catchEffect_.Initialize(playerPosition, 0u);
}

void ComboEffectManager::Draw() {
	for (auto& model : effectContiner_) {
		model.Draw(instancingManager_, modelData_.at(model.GetIndex()));
	}
	catchEffect_.Draw(instancingManager_, modelData_.at(MeshData::CATCH));
}

void ComboEffect::Initialize(const Vector3& playerpos, const uint32_t& index) {
	meshIndex_ = index;
	position_ = playerpos;
	position_.z -= 2.0f;
	isActive_ = true;
	const float rad = std::numbers::pi_v<float> *(1.0f / 3.0f);
	rotate_ = RandomGenerator::GetInstance()->RandFloat(-rad, rad);
	scale_ = 0.0f;
	color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	scaleParam_.Initialize(0.8f, Vector2(0.0f, 0.26f));
	rotateParam_.Initialize(0.4f, Vector2(rotate_ + -std::numbers::pi_v<float>, rotate_));
	fadeParam_.Initialize(0.8f, Vector2(1.0f, 0.0f));
	phase = EffectPhase::Appearance;
}

void ComboEffect::Update(const float& delta) {
	if (!isActive_) { return; }

	bool isChange = false;
	switch (phase) {
	case ComboEffect::Appearance:
		if (ScalePopup(delta)) {
			phase = EffectPhase::Lapse;
		}
		break;
	case ComboEffect::Lapse:
		isChange = Fade(delta);
		break;
	}

	if (isChange) {
		isActive_ = false;
	}
}

void ComboEffect::Draw(ParticleManager* instancingManager, const ParticleMeshTexData* data) const {
	if (!isActive_) { return; }
	static const Vector2 aspectRatio = Vector2(13.0f, 5.0f); // アスペクト比
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale_ * aspectRatio.x,scale_ * aspectRatio.y,1.0f }, Vector3{ 0.0f,0.0f,rotate_ }, position_);
	instancingManager->AddParticle(ParticleData{ matrix,Matrix4x4::MakeIdentity4x4(), color_ }, data);
}

bool ComboEffect::ScalePopup(const float& delta) {
	// 拡大して登場
	scale_ = Ease::UseEase(scaleParam_.easeMin, scaleParam_.easeMax, scaleParam_.nowframe, scaleParam_.kMaxframe, Ease::EaseType::EaseOutBounce);
	rotate_ = Ease::UseEase(rotateParam_.easeMin, rotateParam_.easeMax, rotateParam_.nowframe, rotateParam_.kMaxframe, Ease::EaseType::EaseInSine);
	
	if (scaleParam_.nowframe >= scaleParam_.kMaxframe && rotateParam_.nowframe >= rotateParam_.kMaxframe) {
		// 演出終わったらtrue返す
		return true;
	}
	
	// clamp
	scaleParam_.nowframe += delta;
	scaleParam_.nowframe = std::clamp(scaleParam_.nowframe, 0.0f, scaleParam_.kMaxframe);
	rotateParam_.nowframe += delta;
	rotateParam_.nowframe = std::clamp(rotateParam_.nowframe, 0.0f, rotateParam_.kMaxframe);
	
	return false;
}

bool ComboEffect::Fade(const float& delta) {
	float T = fadeParam_.nowframe / fadeParam_.kMaxframe;
	color_.w = Calc::Lerp(fadeParam_.easeMin, fadeParam_.easeMax, T);

	if (fadeParam_.nowframe >= fadeParam_.kMaxframe) {
		// fadeがおわったら返す
		return true;
	}
	
	fadeParam_.nowframe += delta;
	fadeParam_.nowframe = std::clamp(fadeParam_.nowframe, 0.0f, fadeParam_.kMaxframe);
	return false;
}
