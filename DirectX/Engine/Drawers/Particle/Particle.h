#pragma once
#include "ParticleData.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include <list>
#include <memory>

class ParticleManager;

class Particle {
public:
	Particle(const std::string& particleName, const Texture* texture, BlendMode blendMode = BlendMode::kBlendModeNormal);
	Particle(const std::string& particleName, const ModelData* model, const Texture* texture = nullptr, BlendMode blendMode = BlendMode::kBlendModeNormal);

	static void StaticInitialize();

	void Initialize();

	void Update(float deltaTime);

	void Draw();

private:
	static const ModelData* plane_;
	static ParticleManager* particleManager_;
	std::unique_ptr<GlobalVariableUser> globalVariable_;
	ParticleMeshTexData drawData_;
	std::list<ParticleData> datas_;
};