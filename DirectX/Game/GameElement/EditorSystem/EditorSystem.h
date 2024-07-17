#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <string>
#include <memory>

class Camera;
class Input;
class WaterManager;

class EditorSystem {
public:
	EditorSystem(Camera* camera);
	void Update();

	enum class Type {
		WATER,
	};

	enum class GuizmoType {
		TRANSLATE,
		SCALE,
	};

	enum class State {
		SLECT,
		PUT,
		GUIZMO,
		MAKE_QUADRANGLE,
		NONE,
	};

private:

	

private:
	EditorSystem::Type type_;
	EditorSystem::GuizmoType guizmoType_;
	EditorSystem::State state_;

	WaterManager* waterManager_;
	Input* input_;
	Vector2 mousePos_;
	Camera* camera_ = nullptr;
	int no_;
	bool isSelectQuad_;
};