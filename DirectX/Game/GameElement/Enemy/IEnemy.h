#pragma once
#include <string>
#include <memory>
#include "CollisionSystem/Collider/Collider.h"
#include "ParticleManager.h"
#include "StageEditor/StageEditor.h"
#include "Camera.h"
#include "GameElement/Animation/Animation2D.h"

class IEnemy : public Collider
{
public:
	virtual ~IEnemy() = default;

	static void StaticInitialize();

	virtual void Initialize() = 0;

	virtual void Update(const float& deltaTime, Camera* camera, const uint32_t& babyTension) = 0;

	virtual void Draw() const = 0;

	Vector3& GetPosition() { return position_; }
protected:
	virtual void OnCollision(const Collider& collider) override = 0;
	void SetCollider();
	virtual void SetGlobalVariable() = 0;

	virtual void ApplyGlobalVariable() = 0;

	void CreateStageEditor(const std::string& name, const int& no);
	void DrawDefault(const ParticleMeshTexData* data) const;

	static const ParticleMeshTexData* CreateData(const std::string& textureName);

protected:
	static ParticleManager* instancingManager_;

	std::unique_ptr<StageEditor> stageEditor_;

	float scale_;
	Vector3 position_;
	bool isActive_;
	Vector4 color_;

	std::unique_ptr<Animation2D> animation_;
};