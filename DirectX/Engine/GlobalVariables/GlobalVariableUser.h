#pragma once
#include <string>
#include <stdint.h>

class Vector2;
class Vector3;
class GlobalVariables;

class GlobalVariableUser {
public:
	static void StaticInitialize();

public:
	GlobalVariableUser(const std::string& chunkName, const std::string& groupName);
	GlobalVariableUser(const std::string& name);
	void ResetGroupName(const std::string& groupName);

	void CreateGroup();

	void AddItem(const std::string& key, int32_t value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& key, float value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& key, const Vector2& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& key, const Vector3& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& key, bool value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& key, const std::string& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_");

	int32_t GetIntValue(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	float GetFloatValue(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	Vector2 GetVector2Value(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	Vector3 GetVector3Value(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	bool GetBoolValue(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	std::string GetStringValue(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_") const;

private:
	std::string chunkName_;
	std::string groupName_;

private:
	static GlobalVariables* globalVariable_;
};