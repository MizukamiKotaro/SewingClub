#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Transform/Transform.h"

class Camera;
class Input;

class DebugCamera {
public:
	DebugCamera(Camera* camera, Input* input);

	void DebugUpdate();
	void Update();

	const bool IsDebug() const { return isDebug_; }

private:
	Input* input_;
	Camera* camera_;
	Transform beforeDebugTransform_;
	bool isDebug_;
	bool isPreDebug_;
	bool is2D_;

	Vector3 subRotate_;
	Vector3 subTranslate_;
	Vector2 mousePos_;

	bool isTranslate_;
	bool isRotate_;
	bool isRotateTranslate_;
};