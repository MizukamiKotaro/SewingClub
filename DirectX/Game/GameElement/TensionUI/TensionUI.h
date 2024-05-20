#pragma once
#include "Sprite.h"
#include <memory>
#include <array>
#include "GlobalVariables/GlobalVariableUser.h"
#include "Transform/Transform.h"

class TensionUI {
public:
	TensionUI();
	~TensionUI() = default;

	void Initialize();
	void Update(const float& tension);
	void Draw();

private:
	void SetGlobalVariable();
	void ApplyGlobalVariable();

private:

	enum class Type {
		Frame,
		Gauge,
		kMaxNum
	};

	std::array<std::string, static_cast<uint32_t>(Type::kMaxNum)> tree;


	std::array<std::unique_ptr<Sprite>, static_cast<uint32_t>(Type::kMaxNum)> sprites_;
	std::array<Transform, static_cast<uint32_t>(Type::kMaxNum)> trans_;

	float fixedPosition_ = 1060.f; // 定位置
	float kMaxSize_ = 350.0f; // 最大サイズx
	float tensionPercent_ = 0.2f; // テンション率。1を最大とする


	std::unique_ptr<GlobalVariableUser> global_;
};
