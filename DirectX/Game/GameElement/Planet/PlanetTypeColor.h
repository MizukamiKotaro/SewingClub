#pragma once

enum class PlanetType;
class Vector4;

class PlanetTypeColor {
public:
	static Vector4 GetColor(PlanetType type);
};