#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include <string>
#include <memory>
#include "InstancingModelManager.h"
#include "GlobalVariables/GlobalVariableUser.h"

#include "WaterChunkChip.h"

class Camera;
class Wave;

class WaterChunk
{
public:

	WaterChunk();

	static void StaticInitialize();

	void Initialize();

	void Update();

	void Draw() const;

	static void StaticUpdate();

	void HitTest(const Wave& wave);

public:
	/*static void SetGlobalVariable();

	static void ApplyGlobalVariable();*/

	static bool IsHitCircle(const Vector3& pos0, const Vector3& pos1, float radius);

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

	static InstancingModelManager* instancingManager_;
	static const ModelData* modelData_;

	//static Vector3 scale_;

	static std::unique_ptr<GlobalVariableUser> globalVariable_;

public:
	
	//std::list<std::unique_ptr<WaterChunkChip>> chips_;

	Vector3 position_;
	float scale_;
	float rotate_;

};