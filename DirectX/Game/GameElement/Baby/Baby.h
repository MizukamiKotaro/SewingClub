#pragma once

#include <string>
#include <memory>
#include "InstancingModelManager.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include "GameElement/Planet/PlanetType.h"
#include "Sprite.h"
#include "GameElement/Player/GravityAreaSearch.h"

class Baby : public Collider
{
public:
	Baby(const Vector3& pos, const Vector3& velocity = {});

	void Initialize();

	void Update(float deltaTime);

	void Draw() const;

private:
	void OnCollision(const Collider& collider) override;
	void SetCollider();
	void SetGlobalVariable();

	void ApplyGlobalVariable();

private:
	InstancingModelManager* instancingManager_;
	const ModelData* modelData_;

	std::unique_ptr<GlobalVariableUser> globalVariable_;

	Vector3 position_;
	Vector3 velocity_;
	float scale_;
	float rotate_;
};