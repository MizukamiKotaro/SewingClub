#pragma once
#include <string>
#include <memory>
#include "GameElement/GravityArea/GravityArea.h"
#include "PlanetType.h"
#include "GameElement/Client/Client.h"

class Player;
class RandomGenerator;

class Planet : public Collider
{
public:
	Planet(PlanetType type, const Vector3& pos, Player* player, int no);

	static void StaticInitialize();

	void Initialize();

	void Update(float deltaTime);

	void Draw() const;

private:
	void OnCollision(const Collider& collider) override;
	void SetCollider();

	void SetGlobalVariable();
	void ApplyGlobalVariable();

	void CreateClient();

private:
	std::unique_ptr<GlobalVariableUser> globalVariable_;

	static InstancingModelManager* instancingManager_;
	static const ModelData* modelData_;
	static RandomGenerator* rand_;

	PlanetType type_;
	Player* player_;

	float gamerateTime_;
	float time_;
	static const int MaxClientNum = 6;
	bool isPos[MaxClientNum];
	std::list<std::unique_ptr<Client>> clients_;
	float clientScale_ = 0.5f;

	std::unique_ptr<GravityArea> gravityArea_;
	Vector3 position_;
	float scale_;
	float rotate_;
	int no_;
};