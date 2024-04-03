#include "CollisionManager.h"
#include "CollisionSystem/CollisionProcesses/CollisionCircleCircle/CollisionCircleCircle.h"

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::Initialize()
{
	Clear();
}

void CollisionManager::Clear()
{
	colliders_.clear();
}

void CollisionManager::SetCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void CollisionManager::CheckCollision()
{
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); itrA++) {
		Collider* colliderA = *itrA;
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); itrB++) {
			Collider* colliderB = *itrB;
			colliderA->SetIsHit(false);
			colliderB->SetIsHit(false);
			if (IsNeedCollision(colliderA, colliderB) && IsMatchedMask(colliderA, colliderB)) {
				if (IsCollision(colliderA, colliderB)) {
					colliderA->SetIsHit(true);
					colliderB->SetIsHit(true);
					colliderA->OnCollision(*colliderB);
					colliderB->OnCollision(*colliderA);
				}
			}
		}
	}
}

bool CollisionManager::IsNeedCollision(Collider* a, Collider* b) const
{
	if (a->GetType() == ColliderType::UNKNOWN || b->GetType() == ColliderType::UNKNOWN) {
		return false;
	}
	return true;
}

bool CollisionManager::IsMatchedMask(Collider* a, Collider* b) const
{
	const ColliderMask bMask = b->GetMask();
	const std::list<ColliderMask>& aTargets = a->GetTargetMasks();
	for (const ColliderMask& target : aTargets) {
		if (bMask == target) {
			return true;
		}
	}
	const ColliderMask aMask = a->GetMask();
	const std::list<ColliderMask>& bTargets = b->GetTargetMasks();
	for (const ColliderMask& target : bTargets) {
		if (aMask == target) {
			return true;
		}
	}
	return false;
}

bool CollisionManager::IsCollision(Collider* a, Collider* b)
{
	if (a->GetShape() == ColliderShape::CIRCLE && b->GetShape() == ColliderShape::CIRCLE) {
		if (CollisionCircleCircle::IsCollision(a,b)) {
			return true;
		}
		return false;
	}

	return false;
}