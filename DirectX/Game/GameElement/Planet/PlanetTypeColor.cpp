#include "PlanetTypeColor.h"
#include "Vector4.h"
#include "PlanetType.h"

Vector4 PlanetTypeColor::GetColor(PlanetType type)
{
	switch (type)
	{
	case PlanetType::Kamata:
		return Vector4{ 1.0f,0.7f,0.1f,1.0f };
		break;
	case PlanetType::Kawasaki:
		return Vector4{ 0.0f,0.1f,1.0f,1.0f };
		break;
	case PlanetType::Oomori:
		return Vector4{ 0.1f,1.0f,0.2f,1.0f };
		break;
	default:
		break;
	}
	return Vector4{ 0.0f, 0.0f, 0.0f, 1.0f };
}
