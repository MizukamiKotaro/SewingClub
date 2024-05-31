#include "BabyTensionEffectManager.h"

BabyTensionEffectManager* BabyTensionEffectManager::GetInstance()
{
	static BabyTensionEffectManager instance;
	return &instance;
}

void BabyTensionEffectManager::FirstInitialize(const Vector3* babyPos, const Vector3* cameraPos)
{
	BabyTensionEffect::StaticInitialize();
	BabyTensionEffectChip::StaticInitialize();
	BabyTensionEffectChip::SetBabyPos(babyPos);
	BabyTensionEffectChip::SetCameraPos(cameraPos);
}

void BabyTensionEffectManager::Initialize()
{
	Clear();
	tension_ = 0.0f;
}

void BabyTensionEffectManager::Update(const float& deltaTime)
{
#ifdef _DEBUG
	BabyTensionEffect::StaticUpdate();
#endif // _DEBUG
	tension_ = 0.0f;
	for (std::list<std::unique_ptr<BabyTensionEffect>>::iterator i = effects_.begin(); i != effects_.end();) {
		if (!(*i)->GetIsActive()) {
			(*i).reset(nullptr);
			i = effects_.erase(i);
			continue;
		}
		(*i)->Update(deltaTime);
		tension_ += (*i)->GetTension();
		i++;
	}
}

void BabyTensionEffectManager::Draw() const
{
	for (const std::unique_ptr<BabyTensionEffect>& e : effects_) {
		e->Draw();
	}
}

void BabyTensionEffectManager::CreateEffect(const float& tension)
{
	effects_.push_back(std::make_unique<BabyTensionEffect>(tension));
}