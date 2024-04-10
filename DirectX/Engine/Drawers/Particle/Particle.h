#pragma once
#include "ParticleData.h"
#include <list>
#include <memory>
#include "Drawers/IDrawer/IDrawer.h"
#include "StageEditor/StageEditor.h"
#include <vector>

class Camera;
class ParticleManager;

class Particle : public IDrawer
{
public:
	Particle(const std::string& particleName, const std::string& textureName, bool isStageEditor = false, BlendMode blendMode = BlendMode::kBlendModeNormal);
	Particle(const std::string& particleName, const std::string& modelName, const std::string& textureName, bool isStageEditor = false, BlendMode blendMode = BlendMode::kBlendModeNormal);

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
	const ParticleMeshTexData* drawData_;

	std::unique_ptr<GlobalVariableUser> globalVariable_;
	std::unique_ptr<StageEditor> stageEditor_;

	struct Active {
		Vector3 position;
		Vector3 rotate;
		Vector3 scale;
		Vector3 velocity;
		Vector4 color;
		float currentTime;

		float speed;
		float acceleration;
		float lifeTime;
		float rotateSpeed;
		float rotateAttenuation;

		std::vector<float> fPara;
		std::vector<std::string> fName;
		enum FEnum {
			kFirstSpeedMin, // 初速の最小
			kFirstSpeedMax, // 初速の最大
			kAccelerationMin, // 加速度の最小
			kAccelerationMax, // 加速度の最大
			kLifeTimeMin, // 生存時間の最小
			kLifeTimeMax, // 生存時間の最大
			kRotateSpeedMin, // 回転速度の最小
			kRotateSpeedMax, // 回転速度の最大
			kRotateAttenuationMin, // 回転速度の減衰率の最小
			kRotateAttenuationMax, // 回転速度の減衰率の最大
			kFEnd,
		};

		Vector3 firstAngle;
		Vector3 rotateAngle;

		std::vector<Vector3> v3Para;
		std::vector<std::string> v3Name;
		enum V3Enum {
			kFirstAngleMin, // 最初の向きの最小
			kFirstAngleMax, // 最初の向きの最大
			kFirstRotateMin, // 最初の姿勢の最小
			kFirstRotateMax, // 最初の姿勢の最大
			kRotateMin, // 回転の最小
			kRotateMax, // 回転の最大
			kV3End,
		};
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