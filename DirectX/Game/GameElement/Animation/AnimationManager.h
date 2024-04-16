#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Math/Vector2.h"
#include "Animation2D.h"
#include "Model.h"
#include "GlobalVariables/GlobalVariableUser.h"


class AnimationManager {
public:
	AnimationManager();
	~AnimationManager();

	static AnimationManager* GetInstance();

	void Update();

	void Draw(const Camera* camera);

	Animation2D* AddAnimation(const std::string& groupName);

private:
	void Initialize();
	void ImGuiProcess();
	void Editor();
	// グローバル変数のセット
	void SetGlobalVariable();
	// グローバル変数の更新
	void ApplyGlobalVariable();

private:
	char nameHandle_[256]{};
	char textureName_[256]{};
	std::unique_ptr<GlobalVariableUser> global_;
	std::unordered_map<std::string, std::unique_ptr<Animation2D>> container_;

	std::unique_ptr<Animation2D> animation_;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Texture> texture_;
	// 分割数
	uint32_t divisionWidth = 1u;
	uint32_t divisionHeight = 1u;
	std::string itemName_;
	uint32_t KeyMaxNumber_ = 1u;

	const std::string chunkName = "Animation";

};
