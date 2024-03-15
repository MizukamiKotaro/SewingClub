#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <string>
#include <memory>
#include "InstancingModelManager.h"
#include "GlobalVariables/GlobalVariableUser.h"

#include "WaveFloorChip.h"

class Camera;
class Wave;

class WaveFloor
{
public:

	WaveFloor();

	static void StaticInitialize();

	void Initialize();

	void Update();

	void Draw();

	static void StaticUpdate();

	void HitTest(const Wave& wave);

private:
	/*static void SetGlobalVariable();

	static void ApplyGlobalVariable();*/

	bool IsHitCircle(const Vector3& pos0, const Vector3& pos1, float radius);

private:
	//enum FloatParamater {
	//	kAttenuation, // 減衰率
	//	kMinSpeed, // 最低速度
	//	kInterpolationMaxLange, // 間の長さ
	//	kScale, // スケール
	//	kFloatEnd,
	//};
	//static const std::string fNames[kFloatEnd];
	//static float fParas_[kFloatEnd];

	//static InstancingModelManager* instancingManager_;
	//static const ModelData* modelData_;

	//static Vector3 scale_;

	static std::unique_ptr<GlobalVariableUser> globalVariable_;

private:
	
	std::list<std::unique_ptr<WaveFloorChip>> chips_;
};