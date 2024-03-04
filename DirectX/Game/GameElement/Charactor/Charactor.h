#pragma once
#include <memory>
#include <string>
#include "GlobalVariables/GlobalVariableUser.h"
#include "Model.h"

class Camera;

class Charactor {
public:

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw(const Camera* camera) = 0;

protected:
	void CreateGlobalVariable(const std::string& groupName);

	virtual void SetGlobalVariable();

	virtual void ApplyGlobalVariable();

protected:
	std::unique_ptr<Model> model_;

	std::unique_ptr<GlobalVariableUser> globalVariable_;
};