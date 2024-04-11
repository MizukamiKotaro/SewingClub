#pragma once
#include "IEnemy.h"

class DoesNotWorkEnemy : public IEnemy
{
public:
	DoesNotWorkEnemy(const int& no);

	static void StaticInitialize();

	void Initialize() override;

	void Update(const float& deltaTime, Camera* camera) override;

	void Draw() const override;

private:
	void OnCollision(const Collider& collider) override;

	void SetGlobalVariable() override;

	void ApplyGlobalVariable() override;

private:
	static const ParticleMeshTexData* modelData_;
};