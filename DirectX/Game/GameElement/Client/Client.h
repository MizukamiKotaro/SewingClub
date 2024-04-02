#pragma once

#include <string>
#include <memory>
#include "InstancingModelManager.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include "GameElement/Planet/PlanetType.h"
#include "Sprite.h"
#include "GameElement/Player/GravityAreaSearch.h"

class Client : public Collider
{
public:
	Client(PlanetType type, const Vector3& pos, const Vector3& velocity = {});

	static void StaticInitialize();

	void Initialize();

	void Update(float deltaTime);

	void Draw() const;
	void Draw(const Vector2& pos) const;

	const bool IsInPlanet() const { return isInPlanet_; }

	static void StaticUpdate();
	static float GetScale() { return scale_; }

	const Vector3& GetPosition() const { return position_; }
	const PlanetType GetType() const { return type_; }

private:
	void OnCollision(const Collider& collider) override;
	void SetCollider();
	static void SetGlobalVariable();

	static void ApplyGlobalVariable();

private:
	static InstancingModelManager* instancingManager_;
	static const ModelData* modelData_;

	static std::unique_ptr<GlobalVariableUser> globalVariable_;
	std::unique_ptr<GravityAreaSearch> gravityAreaSearch_;

	PlanetType type_;
	std::unique_ptr<Sprite> sprite_;

	Vector3 position_;
	Vector3 velocity_;
	static float scale_;
	float rotate_;
	bool isInWater_;
	bool isInPlanet_;
};