#pragma once
#include <string>
#include <memory>
#include "GameElement/GravityArea/GravityArea.h"
#include "StageEditor/StageEditor.h"
#include "WaterChunkQuadrangle.h"
#include "GameElement/Gimmick/MoveWaterGimmick.h"

class Camera;
class Player;
class WaterManager;

class WaterChunk : public Collider
{
public:

	WaterChunk();
	WaterChunk(int no);
	WaterChunk(const int& no, const MoveWaterGimmick::GimmickWaterParam& param);
	WaterChunk(const int& no, const Vector2& pos);
	WaterChunk(const Vector2& pos, const Vector2& radius, bool isSame, const float& rotate, bool isSmall);

	static void StaticInitialize();

	void Initialize();

	void Update(const float& deltaTime, Camera* camera, const bool& globalUse = true);
	void MoveUpdate(); // 移動ベクトルをpositionに加算

	void Draw(Camera* camera) const;

	static void StaticUpdate();

	const bool IsDelete() const { return deleteTime_ <= time_; }

	static void SetPlayer(const Player* player);
	static const float& GetMinScale() { return minScale_; }

	void CreateQuadrangle();

	// めっちゃ面倒な処理
	const Vector3& GetPosition() const { return position_; }
	const float& GetScale() const { return scale_; }
	const bool& GetIsTarget() const { return isTarget_; }
	const int& GetEndNo() const { return endNo_;}

	bool IsHitMouse(const Vector2& mousePos) const;

	void SetPosition(const Vector3& pos);
	void SetScale(const float& scale);
	void SetIsQuadrangle(const bool& is, const int& endNo);

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();
	void SetGlobalVariableAndSetPos();

	static void StaticSetGlobalVariable();
	static void StaticApplyGlobalVariable();

	void ActiveCheck(Camera* camera);

private:
	void OnCollision(const Collider& collider) override;
	void SetCollider();

private:
	static InstancingModelManager* instancingManager_;
	static const InstancingMeshTexData* modelData_;

	static std::unique_ptr<GlobalVariableUser> staticGlobalVariable_;
	std::unique_ptr<StageEditor> stageEditor_;

	static float minScale_;

	static WaterManager* waterManager_;

public:

private:
	static const Player* player_;
	static float deleteTime_;
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

	std::unique_ptr<WaterChunkQuadrangle> quadrangle_;
	bool isTarget_;
	bool isQuadrangleActive_;
	int endNo_;

	MoveWaterGimmick::GimmickWaterParam moveWaterGimmickParam_;
};