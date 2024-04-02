#include "ClientManager.h"

ClientManager* ClientManager::GetInstance()
{
	static ClientManager instance;
	return &instance;
}

void ClientManager::Update(float deltaTime)
{
	for (std::list<std::unique_ptr<Client>>::iterator it = clients_.begin(); it != clients_.end();) {
		if ((*it)->IsInPlanet()) {
			(*it).reset(nullptr);
			it = clients_.erase(it);
		}
		else {
			(*it)->Update(deltaTime);
			it++;
		}
	}
}

void ClientManager::SetClient(PlanetType type, const Vector3& pos, const Vector3& velocity)
{
	clients_.push_back(std::make_unique<Client>(type, pos, velocity));
}

PlanetType ClientManager::GetHitClientType()
{
	for (std::list<std::unique_ptr<Client>>::iterator it = clients_.begin(); it != clients_.end();) {
		if ((*it)->GetIsHit()) {
			return (*it)->GetType();
		}
		else {
			it++;
		}
	}
	return PlanetType::kEnd;
}

void ClientManager::Draw()
{
	for (std::unique_ptr<Client>& client : clients_) {
		client->Draw();
	}
}
