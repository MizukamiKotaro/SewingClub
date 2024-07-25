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

	// return true:水を生成する / false:待機中
	bool Update(const float& delta, Camera* camera);
	void Draw() const;

	struct GimmickWaterParam {
		Vector2 moveVector; // 移動ベクトル
		Vector2 respawnPoint; // 初期地点
		float waterScale = 1.0f; // 生成する水の大きさ
		float waterSpeed_ = 0.5f;
		void operator=(GimmickWaterParam param) {
			moveVector = param.moveVector;
			respawnPoint = param.respawnPoint;
			waterScale = param.waterScale;
		}
	};
	GimmickWaterParam waterParam_;

	GimmickWaterParam GetParam() const { return waterParam_; }

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();
	void OnCollision(const Collider& collider) override;
	void SetCollider();
	void ActiveCheck(Camera* camera);
	bool CreateCount(const float& delta);

private:
	static InstancingModelManager* instancingManager_;
	static const InstancingMeshTexData* modelData_;

	static ItemManager* itemManager_;

	static std::unique_ptr<GlobalVariableUser> staticGlobalVariable_;
	std::unique_ptr<StageEditor> stageEditor_;

private:
	Vector3 position_;
	float scale_ = 0.0f;
	float rotate_ = 0.0f;

	Vector2 moveSpeed_; // 移動量と向きのベクトル endPoint - position

	Vector2 endPoint_; // 終点座標
	Vector4 color_;

	int no_ = 0;
	bool isActive_ = true;

	float nowFrame_ = 0.0f;
	float createInterval_ = 10.0f; // 生成間隔second

};
