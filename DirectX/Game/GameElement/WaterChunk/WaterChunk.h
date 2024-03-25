#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include <string>
#include <memory>
#include "InstancingModelManager.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include "CollisionSystem/Collider/Collider.h"
#include "WaterChunkChip.h"
#include "GameElement/GravityArea/GravityArea.h"

class Camera;
class Wave;

class WaterChunk : public Collider
{
public:

	WaterChunk();
	WaterChunk(int no);

	static void StaticInitialize();

	void Initialize();

	void Update();

	void Draw() const;

	static void StaticUpdate();

	void HitTest(const Wave& wave);

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

private:
	void OnCollision(const Collider& collider) override;
	void SetCollider();

private:
	//enum FloatParamater {
	//	kAttenuation, // 減衰率
	//	kMinSpeed, // 最低速度
	//	kInterpolationMaxLange, // 間の長さ
	//	kScale, // スケール
	//	kFloatEnd,
	//};
	//const std::string fNames[kFloatEnd];
	//float fParas_[kFloatEnd];

	static InstancingModelManager* instancingManager_;
	static const ModelData* modelData_;

	//static Vector3 scale_;

	std::unique_ptr<GlobalVariableUser> globalVariable_;

public:
	
	//std::list<std::unique_ptr<WaterChunkChip>> chips_;

private:
	Vector3 position_;
	float scale_;
	float rotate_;

	std::unique_ptr<GravityArea> gravityArea_;
	std::string groupName_ = "_";

};