#pragma once
#include "Math/calc.h"
#include "Camera.h"
#include <string>
#include <memory>
#include "StageEditor/StageEditor.h"
#include "InstancingModelManager.h"
#include "CollisionSystem/Collider/Collider.h"

class Camera;
class Wave;
class ItemManager;

class MoveWaterGimmick : public Collider {
public:
	MoveWaterGimmick() = default;
	MoveWaterGimmick(int no);
	~MoveWaterGimmick() = default;

	static void StaticInitialize();

	void Update(const float& delta, Camera* camera);
	void Draw() const;

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();
	void OnCollision(const Collider& collider) override;
	void SetCollider();
	void ActiveCheck(Camera* camera);

private:
	static InstancingModelManager* instancingManager_;
	static const InstancingMeshTexData* modelData_;

	static ItemManager* itemManager_;

	static std::unique_ptr<GlobalVariableUser> staticGlobalVariable_;
	std::unique_ptr<StageEditor> stageEditor_;

private:
	Vector3 position_;
	float scale_;
	float rotate_;

	Vector2 moveSpeed_;


	Vector2 endPoint_;
	Vector4 color_;

	int no_ = 0;
	bool isActive_ = true;
};
