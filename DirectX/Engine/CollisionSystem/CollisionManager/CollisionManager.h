#pragma once
#include "CollisionSystem/Collider/Collider.h"
#include <list>

class CollisionManager
{
public:
	static CollisionManager* GetInstance();
	
	void Initialize();

	void Clear();

	void SetCollider(Collider* collider);

	void CheckCollision();

private:
	CollisionManager() = default;
	~CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	const CollisionManager& operator=(const CollisionManager&) = delete;

	bool IsNeedCollision(Collider* a, Collider* b)const;
	bool IsMatchedMask(Collider* a, Collider* b) const;
	bool IsCollision(Collider* a, Collider* b);

private:
	std::list<Collider*> colliders_;

};