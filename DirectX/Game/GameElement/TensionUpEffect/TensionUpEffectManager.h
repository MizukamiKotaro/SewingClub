#pragma once
#include "TensionUpEffect.h"

class TensionUpEffectManager
{
public:
	static TensionUpEffectManager* GetInstance();

	void Clear() { effects_.clear(); }

	void FirstInitialize(const Vector3* babyPos, const Vector3* cameraPos);

	void Initialize();

	void Update(const float& deltaTime);

	void Draw() const;

	void CreateEffect(const float& tension);

private:
	TensionUpEffectManager() = default;
	~TensionUpEffectManager() = default;
	TensionUpEffectManager(const TensionUpEffectManager&) = delete;
	TensionUpEffectManager& operator=(const TensionUpEffectManager&) = delete;

private:
	std::list<std::unique_ptr<TensionUpEffect>> effects_;
};