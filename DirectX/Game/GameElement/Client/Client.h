#pragma once

#include <string>
#include <memory>
#include "InstancingModelManager.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include "GameElement/Planet/PlanetType.h"
#include "Sprite.h"

class Client 
{
public:
	Client(PlanetType type, const Vector3& pos, float scale);

	static void StaticInitialize();

	void Initialize();

	void Update(float deltaTime);

	void Draw() const;
	void Draw(const Vector2& pos) const;

	static void StaticUpdate();

	const PlanetType GetType() const { return type_; }

private:
	static void SetGlobalVariable();

	static void ApplyGlobalVariable();

private:
	static InstancingModelManager* instancingManager_;
	static const ModelData* modelData_;

	static std::unique_ptr<GlobalVariableUser> globalVariable_;

	PlanetType type_;
	std::unique_ptr<Sprite> sprite_;

	Vector3 position_;
	float scale_;
	float rotate_;
};