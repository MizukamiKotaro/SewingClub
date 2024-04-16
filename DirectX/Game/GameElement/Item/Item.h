#pragma once
#include <string>
#include <memory>
#include "StageEditor/StageEditor.h"
#include "InstancingModelManager.h"
#include "CollisionSystem/Collider/Collider.h"
#include"Audio.h"

class Camera;
class Wave;

class Item : public Collider
{
public:
	Item(int no, const float* scale);
	
	static void StaticInitialize();

	void Initialize();

	void Update(float deltaTime, Camera* camera);

	void Draw() const;

	static void StaticUpdate();

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

	static void StaticSetGlobalVariable();
	static void StaticApplyGlobalVariable();

private:
	void OnCollision(const Collider& collider) override;
	void SetCollider();

private:
	static InstancingModelManager* instancingManager_;
	static const InstancingMeshTexData* modelData_;

	static std::unique_ptr<GlobalVariableUser> staticGlobalVariable_;
	std::unique_ptr<StageEditor> stageEditor_;

	void ActiveCheck(Camera* camera);

private:
	static float deleteTime_;

	bool isHit_;
	const float* maxScale_;

	Vector3 position_;
	float scale_;
	float rotate_;
	bool isSmall_;
	float time_;
	Vector4 color_;

	int no_;
	bool isActive_;

	//コイン取得音
	Audio seGetCoin_;
};