#include "TensionUpEffectManager.h"

TensionUpEffectManager* TensionUpEffectManager::GetInstance()
{
	static TensionUpEffectManager instance;
	return &instance;
}

void TensionUpEffectManager::FirstInitialize(const Vector3* babyPos, const Vector3* cameraPos)
{
	TensionUpEffect::StaticInitialize();
	TensionUpEffectChip::StaticInitialize();
	TensionUpEffectChip::SetBabyPos(babyPos);
	TensionUpEffectChip::SetCameraPos(cameraPos);
}

void TensionUpEffectManager::Initialize()
{
	Clear();
}

void TensionUpEffectManager::Update(const float& deltaTime)
{
#ifdef _DEBUG
	TensionUpEffect::StaticUpdate();
#endif // _DEBUG
	for (std::list<std::unique_ptr<TensionUpEffect>>::iterator i = effects_.begin(); i != effects_.end();) {
		if (!(*i)->GetIsActive()) {
			(*i).reset(nullptr);
			i = effects_.erase(i);
			continue;
		}
		(*i)->Update(deltaTime);
		i++;
	}
}

void TensionUpEffectManager::Draw() const
{
	for (const std::unique_ptr<TensionUpEffect>& e : effects_) {
		e->Draw();
	}
}

void TensionUpEffectManager::CreateEffect(const float& tension)
{
	effects_.push_back(std::make_unique<TensionUpEffect>(tension));
}
