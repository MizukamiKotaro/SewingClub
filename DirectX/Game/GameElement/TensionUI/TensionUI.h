#pragma once
#include "Sprite.h"
#include <memory>
#include <array>
#include "GlobalVariables/GlobalVariableUser.h"
#include "Transform/Transform.h"
#include "GameElement/Animation/Animation2D.h"

class TensionUI {
public:
	TensionUI();
	~TensionUI() = default;

	void Initialize();
	void Update(const float& tension, const int& faceParam);
	void Draw();

private:
	void SetGlobalVariable();
	void ApplyGlobalVariable();
	void GaugeColor(const float& tension);

private:

	enum class Type {
		Frame,
		Gauge,
		Face,
		kMaxNum
	};

	enum Face {
		kNormal,
		kSmile,
		kSuperSmile,
		kAnxiety,
		kCry,
		kSuperSuperSmile,
		kFaceEnd,
	};

	std::array<std::string, static_cast<uint32_t>(Type::kMaxNum)> tree;
	std::array<std::unique_ptr<Sprite>, static_cast<uint32_t>(Type::kMaxNum)> sprites_;
	std::array<Vector4, static_cast<uint32_t>(Type::kMaxNum)> colors_;

	Vector2 fixedPosition_ = Vector2(1060.f, 650.0f); // 定位置
	Vector2 kMaxSize_ = Vector2(350.0f, 87.5f); // 最大サイズx
	float tensionPercent_ = 0.2f; // テンション率。1を最大とする

	std::unique_ptr<Animation2DData> animationData_;
	std::unique_ptr<Animation2D> animation_;

	std::unique_ptr<GlobalVariableUser> global_;
};
