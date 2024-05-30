#pragma once
#include "Model.h"
#include <memory>
#include "ParticleManager.h"
#include <array>

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


private:
	Vector2 scale_;
	Vector3 position_;
	Vector4 color_;
	uint32_t meshIndex_ = 0u;
	bool isActive_ = false;
	float nowframe_ = 0.0f;

};

// ステージシーンでのみ生成
class ComboEffectManager {
public:
	ComboEffectManager();
	~ComboEffectManager() = default;

	static ComboEffectManager* GetInstance();

	void Update(const float& delta);
	void Create(const Vector3& playerPosition);
	void Draw();

private:
	uint32_t RandNum(int min, int max);

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
		"rainbow.png",
		"rainbow.png",
		"rainbow.png",
	};
	
	std::array<const ParticleMeshTexData*, MeshData::kMaxNum> modelData_;

	std::array<ComboEffect, 10u> effectContiner_;

};