#pragma once
#include "Matrix4x4.h"
#include "Vector4.h"

class ParticleData {
public:
	Matrix4x4 matrix_;
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
};