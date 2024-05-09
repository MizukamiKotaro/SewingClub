#pragma once
class Collider;
class Vector2;

class CollisionCircleQuadrangle {
public:
	static bool IsCollision(Collider* a, Collider* b);

private:
	static float MakeOuter(const Vector2& pos1, const Vector2& pos2, const Vector2& pos3);
	static bool IsCollision(const Vector2& pos1, const Vector2& pos2, const Vector2& pos3, const Vector2& center, const float& radius);
};