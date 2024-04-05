#pragma once
#include <string>
#include <memory>
#include "WaterChunkChip.h"
#include "GameElement/GravityArea/GravityArea.h"

class Camera;
class Wave;

class WaterChunk : public Collider
{
public:

	WaterChunk();
	WaterChunk(int no);
	WaterChunk(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate, bool isSmall);

	static void StaticInitialize();

	void Initialize();

	void Update(float deltaTime);

	void Draw() const;

	static void StaticUpdate();

	const bool IsDelete() const { return deleteTime_ <= time_; }

	void HitTest(const Wave& wave);

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

	static void StaticSetGlobalVariable();
	static void StaticApplyGlobalVariable();

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
	static std::unique_ptr<GlobalVariableUser> staticGlobalVariable_;
	std::unique_ptr<GlobalVariableUser> globalVariable_;

public:
	
	//std::list<std::unique_ptr<WaterChunkChip>> chips_;

private:
	static float deleteTime_;

	Vector3 position_;
	float maxScale_;
	float scale_;
	float rotate_;
	bool isSmaeGravitySize_;
	bool isSmall_;
	float time_;
	bool isTree_;
	Vector4 color_;

	std::unique_ptr<GravityArea> gravityArea_;
	std::string groupName_ = "_";
	int no_;
};