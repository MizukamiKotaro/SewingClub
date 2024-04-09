#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <string>
#include <memory>
#include "InstancingModelManager.h"
#include "GlobalVariables/GlobalVariableUser.h"

class Camera;

class YarnLimit
{
public:

	YarnLimit(const Vector3* parent, const Vector3& ganeratePosition);

	static void StaticInitialize();

	void Initialize();

	void Update();

	void Draw() const;

	static void StaticUpdate();

private:
	static void SetGlobalVariable();

	static void ApplyGlobalVariable();

private:
	enum FloatParamater {
		kAttenuation, // 減衰率
		kMinSpeed, // 最低速度
		kInterpolationMaxLange, // 間の長さ
		kScale, // スケール
		kFloatEnd,
	};
	static const std::string fNames[kFloatEnd];
	static float fParas_[kFloatEnd];

	//enum Vector3Paramater {
	//	kScale, // スケール
	//	kVector3End,
	//};
	//static const std::string v3Names[kVector3End];
	//static Vector3 v3Paras_[kVector3End];

	static InstancingModelManager* instancingManager_;
	static const ModelData* modelData_;

	static Vector3 scale_;

	static std::unique_ptr<GlobalVariableUser> globalVariable_;

private:
	Vector3 velocity_;

	Vector3 position_;
	Quaternion rotate_;

	Vector3 ganeratePosition_;

	const Vector3* parent_ = nullptr;
	std::unique_ptr<YarnLimit> child_;
};