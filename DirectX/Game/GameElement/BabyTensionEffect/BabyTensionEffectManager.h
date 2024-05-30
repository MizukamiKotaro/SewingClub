#pragma once
#include "BabyTensionEffect.h"

class BabyTensionEffectManager
{
public:
	static BabyTensionEffectManager* GetInstance();

	void Clear() { effects_.clear(); }

	void FirstInitialize(const Vector3* babyPos);

	void Initialize();

	void Update(const float& deltaTime);

	void Draw() const;

	void CreateEffect(const float& tension);

	const float& GetTension() const { return tension_; }

private:
	BabyTensionEffectManager() = default;
	~BabyTensionEffectManager() = default;
	BabyTensionEffectManager(const BabyTensionEffectManager&) = delete;
	BabyTensionEffectManager& operator=(const BabyTensionEffectManager&) = delete;

private:
	std::list<std::unique_ptr<BabyTensionEffect>> effects_;
	float tension_;
};