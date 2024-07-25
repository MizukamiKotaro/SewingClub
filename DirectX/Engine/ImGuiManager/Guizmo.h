#pragma once
#ifdef _DEBUG
#include "Vector3.h"
#include "Matrix4x4.h"

namespace Guizmo {
	enum class Type
	{
		TRANSLATE,
		SCALE
	};

	Vector3 ShowTransformGizmo(const Guizmo::Type& type, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, Matrix4x4& objectMatrix, const int& no);
}
#endif // _DEBUG