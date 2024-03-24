#pragma once

class Collider;

class CollisionCircleCircle {
public:
	static bool IsCollision(Collider* a, Collider* b);
};