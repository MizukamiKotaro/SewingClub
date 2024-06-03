#pragma once
#include "Model.h"
#include <memory>
#include "ParticleManager.h"
#include <array>
#include "Audio/Audio.h"

class ComboEffect {
public:
	ComboEffect() = default;
	~ComboEffect() = default;

	void Initialize(const Vector3& playerpos, const uint32_t& index);
	void Update(const float& delta);
	void Draw(ParticleManager* instancingManager, const ParticleMeshTexData* data) const;

	const uint32_t& GetIndex() const { return meshIndex_; }
	const bool& GetActive() const { return isActive_; }
private:
	bool ScalePopup(const float& delta); // 拡大回転して登場
	bool Fade(const float& delta);

private:
	float scale_;
	float rotate_;
	Vector3 position_;
	Vector4 color_;
	uint32_t meshIndex_ = 0u;
	bool isActive_ = false;

	struct EffectParam {
		float nowframe = 0.0f;
		float kMaxframe = 0.0f;
		float easeMin = 0.0f;
		float easeMax = 0.0f;
		void Initialize(const float& maxframe, const Vector2& minmax) {
			nowframe = 0.0f;
			kMaxframe = maxframe;
			easeMin = minmax.x;
			easeMax = minmax.y;
		}
	};
	EffectParam scaleParam_;
	EffectParam rotateParam_;
	EffectParam fadeParam_;

	enum EffectPhase {
		Appearance,
		Lapse
	};
	EffectPhase phase;

};

// ステージシーンでのみ生成
class ComboEffectManager {
public:
	ComboEffectManager();
	~ComboEffectManager();

	static ComboEffectManager* GetInstance();

	void Update(const float& delta);
	int32_t Create(const Vector3& playerPosition);
	void Draw();

private:
	//インスタンシングに必要
	ParticleManager* instancingManager_ = nullptr;
	enum MeshData {
		YEAH,
		WOW,
		YAY,
		kMaxNum
	};
	std::array<std::string, MeshData::kMaxNum> texturePath_{
		"ingame_baby_yeah1.png",
		"ingame_baby_yay1.png",
		"ingame_baby_wow1.png",
	};
	
	std::array<const ParticleMeshTexData*, MeshData::kMaxNum> modelData_;

	std::array<ComboEffect, 10u> effectContiner_;
	int oldRandNumber_ = 0;

	// SE関連
	enum SEType {
		sGood,
		sVeryGood,
		kMaxNumber
	};

	Audio se_[kMaxNumber];

	std::string sePath_[SEType::kMaxNumber] = {
		"SE/baby/baby_good.mp3",
		"SE/baby/baby_veryGood.mp3",
	};
	std::string seText_[SEType::kMaxNumber] = {
		"baby_good",
		"baby_veryGood",
	};

};