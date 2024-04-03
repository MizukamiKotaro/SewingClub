#pragma once
#include "Client.h"
#include <list>
#include <memory>

class ClientManager {
public:
	static ClientManager* GetInstance();

	void Clear() { clients_.clear(); }

	void Update(float deltaTime);

	void SetClient(PlanetType type, const Vector3& pos, const Vector3& velocity = {});

	PlanetType GetHitClientType();
	void DeleteHitClient();

	void Draw();

private:
	ClientManager() = default;
	~ClientManager() = default;
	ClientManager(const ClientManager&) = delete;
	ClientManager& operator=(const ClientManager&) = delete;

private:
	std::list<std::unique_ptr<Client>> clients_;
};