#pragma once
#include <string>
#include <memory>
#include "WaterChunkChip.h"
#include "GameElement/GravityArea/GravityArea.h"
#include "StageEditor/StageEditor.h"
#include "WaterWave.h"


class Camera;
class Player;
class WaterManager;

class WaterChunk : public Collider
{
public:

	WaterChunk();
	WaterChunk(int no);
	WaterChunk(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate, bool isSmall);

	static void StaticInitialize();

	void Initialize();

	void Update(const float& deltaTime, Camera* camera);

	void Draw() const;

	static void StaticUpdate();

	const bool IsDelete() const { return deleteTime_ <= time_; }

	static void SetPlayer(const Player* player);

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

	static void StaticSetGlobalVariable();
	static void StaticApplyGlobalVariable();

	void ActiveCheck(Camera* camera);

	void CreateChips();

	void AddWave(const bool& isDown);

	void AddWave(const WaterChunk& water, WaterWave& wave);

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
	static const InstancingMeshTexData* modelData_;

	//static Vector3 scale_;
	static std::unique_ptr<GlobalVariableUser> staticGlobalVariable_;
	std::unique_ptr<StageEditor> stageEditor_;

	static float minScale_;

	static WaterManager* waterManager_;

public:
	
	std::list<std::unique_ptr<WaterChunkChip>> chips_;

private:
	static const Player* player_;
	static float deleteTime_;

	std::list<std::unique_ptr<WaterWave>> waves_;
	bool isWave_;

	bool isPlayer_;
	bool preIsPlayer_;

	Vector3 position_;
	float maxScale_;
	float scale_;
	float rotate_;
	bool isSmaeGravitySize_;
	bool isSmall_;
	float time_;
	bool isTree_;
	Vector4 color_;

	bool isActive_;
	std::unique_ptr<GravityArea> gravityArea_;
	int no_;
};