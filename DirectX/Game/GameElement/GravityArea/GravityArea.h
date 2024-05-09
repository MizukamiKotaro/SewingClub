#pragma once
#include "CollisionSystem/Collider/Collider.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include <memory>
#ifdef _DEBUG
#include "InstancingModelManager.h"
#endif // _DEBUG

class GravityArea : public Collider
{
public:
	GravityArea(const ColliderShape& shape = ColliderShape::CIRCLE);

	static void StaticInitialize();

	static void StaticUpdate();

	void Update(const Vector2& pos, const Vector2& radius, bool isSame = false, const float& rotate = 0.0f, const Vector2& velcity = {});
	void Update(const Vector3& leftTop, const Vector3& rightTop, const Vector3& leftBottom, const Vector3& rightBottom, const Vector3& startPos, const Vector3& endPos);

#ifdef _DEBUG
	void Draw(const Vector2& pos, const Vector2& radius, bool isSame = false, const float& rotate = 0.0f);
#endif // _DEBUG

private:
	void OnCollision(const Collider& collider) override;
	static void SetGlobalVariable();
	static void ApplyGlobalVariable();

private:
	static std::unique_ptr<GlobalVariableUser> globalVariable_;
	static float scale_;

#ifdef _DEBUG
	static InstancingModelManager* instancingManager_;
	static const InstancingMeshTexData* modelData_;
#endif // _DEBUG
};
