#pragma once
#include <string>
#include <memory>
#include "GameElement/GravityArea/GravityArea.h"

class Camera;
class Player;
class WaterManager;
class DrawQuadrangleManager;

class WaterChunkQuadrangle : public Collider
{
public:

	WaterChunkQuadrangle();

	static void StaticInitialize();

	void Initialize();

	void Update(const float& deltaTime, Camera* camera);

	void Draw(Camera* camera) const;

	static void SetPlayer(const Player* player);
	void CreateQuadrangle(const Vector3& pos1, const float& scale1, const Vector3& pos2, const float& scale2);

private:
	void ActiveCheck(Camera* camera);

private:
	void OnCollision(const Collider& collider) override;
	void SetCollider();

private:

	static DrawQuadrangleManager* quadrangleManager_;

private:
	static const Player* player_;
	bool isWave_;

	bool isPlayer_;
	bool preIsPlayer_;

	Vector3 leftTop_;
	Vector3 rightTop_;
	Vector3 leftBottom_;
	Vector3 rightBottom_;

	Vector3 position_;
	float length_;

	float startScale_;
	float endScale_;
	Vector3 startPos_;
	Vector3 endPos_;
	
	float rotate_;
	Vector4 color_;

	bool isActive_;
	std::unique_ptr<GravityArea> gravityArea_;

	
};