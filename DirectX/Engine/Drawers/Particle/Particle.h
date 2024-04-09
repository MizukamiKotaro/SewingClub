#pragma once
#include "ParticleData.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include <list>
#include <memory>
#include "Drawers/IDrawer/IDrawer.h"

class Camera;
class ParticleManager;

class Particle : public IDrawer
{
public:
	Particle(const std::string& particleName, const std::string& textureName, BlendMode blendMode = BlendMode::kBlendModeNormal);
	Particle(const std::string& particleName, const std::string& modelName, const std::string& textureName, BlendMode blendMode = BlendMode::kBlendModeNormal);

	static void StaticInitialize();

	void Initialize();

	void Update(float deltaTime, Camera* camera);

	void Draw(Camera* camera);

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

private:
	static const ModelData* plane_;
	static ParticleManager* particleManager_;
	std::unique_ptr<GlobalVariableUser> globalVariable_;
	const ParticleMeshTexData* drawData_;

	struct Active {
		Vector3 position;
		Vector3 rotate;
		Vector3 scale;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};

	struct Emitter
	{
		Vector3 pos;
		Vector3 min;
		Vector3 max;
		uint32_t generateParticleNum_; // 一回に生成する数
		float generateCoolTime_; // 生成の間隔
		float countTime_; // 生成をするための時間のカウント
	};
};