#pragma once
#include <memory>
#include <string>
#include "GlobalVariables/GlobalVariableUser.h"
#include "Model.h"
#include "CollisionSystem/Collider/Collider.h"

class Camera;

class Charactor : public Collider
{
public:
	virtual ~Charactor() override = default;

	virtual void OnCollision(const Collider& collider) override = 0;

	virtual void Initialize() = 0;

	virtual void Update(float deltaTime) = 0;

	virtual void Draw(const Camera* camera) = 0;

protected:
	void CreateGlobalVariable(const std::string& groupName);

	virtual void SetGlobalVariable();

	virtual void ApplyGlobalVariable();

protected:
	std::unique_ptr<Model> model_;

	std::unique_ptr<GlobalVariableUser> globalVariable_;
};