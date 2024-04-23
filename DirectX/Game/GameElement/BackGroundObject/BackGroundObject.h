#pragma once
#include <string>
#include <memory>
#include "StageEditor/StageEditor.h"
#include "InstancingModelManager.h"

enum class BackGroundObjectType;

class BackGroundObject
{
public:
	BackGroundObject(const int& no, const BackGroundObjectType& type);
	
	static void StaticInitialize();

	void Initialize();

	void Update(float deltaTime);

	void Draw() const;

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void CreateStageEditor(const std::string& name);

private:
	static InstancingModelManager* instancingManager_;

private:
	std::unique_ptr<StageEditor> stageEditor_;
	const InstancingMeshTexData* modelData_;

	Vector3 position_;
	Vector3 scale_;
	Vector3 rotate_;
	Vector4 color_;

	int no_;
};