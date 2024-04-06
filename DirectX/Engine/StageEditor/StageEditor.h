#pragma once
#include "GlobalVariables/GlobalVariableUser.h"
#include <memory>

class StageEditor {
public:
	StageEditor(const std::string& tree1);

	void Initialize();

	void AddItem(const std::string& key, int32_t value, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& key, float value, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& key, const Vector2& value, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& key, const Vector3& value, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& key, bool value, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& key, const std::string& value, const std::string& tree2 = "_", const std::string& tree3 = "_");

	int32_t GetIntValue(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	float GetFloatValue(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	Vector2 GetVector2Value(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	Vector3 GetVector3Value(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	bool GetBoolValue(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	std::string GetStringValue(const std::string& key, const std::string& tree2 = "_", const std::string& tree3 = "_") const;

	bool IsTreeOpen(const std::string& tree2 = "_", const std::string& tree3 = "_");

	const bool IsChangedStage();

private:
	StageEditor() = default;

private:
	int preStageNo_;
	std::string tree1_;
	std::unique_ptr<GlobalVariableUser> global_;
};