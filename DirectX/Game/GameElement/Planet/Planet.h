#pragma once
#include <string>
#include <memory>
#include <vector>
#include "GameElement/GravityArea/GravityArea.h"
#include "PlanetType.h"
#include "GameElement/Client/Client.h"

class Player;
class RandomGenerator;
class ClientManager;

class Planet : public Collider
{
public:
	Planet(PlanetType type, const Vector3& pos, Player* player, int no);

	static void StaticInitialize();

	void Initialize();

	void Update(float deltaTime);

	static void StaticUpdate();

	void Draw() const;

	void CreateClient();

private:
	void OnCollision(const Collider& collider) override;
	void SetCollider();

	void SetGlobalVariable();
	void ApplyGlobalVariable();

	static void StaticSetGlobalVariable();
	static void StaticApplyGlobalVariable();

private:
	
	std::unique_ptr<GlobalVariableUser> globalVariable_;
	static std::unique_ptr<GlobalVariableUser> staticGlobalVariable_;

	static InstancingModelManager* instancingManager_;
	static const ModelData* modelData_;
	static RandomGenerator* rand_;
	static ClientManager* clientMnager_;

	static int MaxClientNum;

	PlanetType type_;
	Player* player_;

	std::vector<bool> isPos;
	std::list<std::unique_ptr<Client>> clients_;

	std::unique_ptr<GravityArea> gravityArea_;
	Vector3 position_;
	float scale_;
	float rotate_;
	int no_;
};