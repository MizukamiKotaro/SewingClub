#pragma once
#include <string>
#include <memory>
#include "StageEditor/StageEditor.h"
#include "InstancingModelManager.h"
#include "CollisionSystem/Collider/Collider.h"
#include "Audio.h"

class Camera;
class Wave;
class ItemManager;

struct QuotaSendData {
	const Vector3* pos;
	const float* size;
	const bool* isHit;
};

class RequiredObject : public Collider {
public:
	RequiredObject() = default;
	RequiredObject(const int& no, const float& scale, const float& scaleDia);
	~RequiredObject() = default;

	static void StaticInitialize();
	// true:取得済み/false:未取得
	bool Update(float deltaTime, Camera* camera);

	void Draw() const;

	QuotaSendData GetQuotaData() {
		return{
			&position_,
			&scale_,
			&isHit_
		};
	}

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

private:
	void OnCollision(const Collider& collider) override;
	void SetCollider();

private:
	static InstancingModelManager* instancingManager_;
	static const InstancingMeshTexData* modelData_;

	static ItemManager* itemManager_;
	static Vector4 staticColor_;

	std::unique_ptr<StageEditor> stageEditor_;

	void ActiveCheck(Camera* camera);

private:
	bool isHit_;
	Vector3 position_;
	float scale_;
	float rotate_;
	Vector4 color_;
	float scaleDiameter_;

	int no_;
	bool isActive_;


	//コイン取得音
	Audio seGetCoin_;

	
};
