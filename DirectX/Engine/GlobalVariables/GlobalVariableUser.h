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

	void AddItem(const std::string& key, int32_t value);
	void AddItem(const std::string& key, float value);
	void AddItem(const std::string& key, const Vector2& value);
	void AddItem(const std::string& key, const Vector3& value);
	void AddItem(const std::string& key, bool value);
	void AddItem(const std::string& key, const std::string& value);

	int32_t GetIntValue(const std::string& key) const;
	float GetFloatValue(const std::string& key) const;
	Vector2 GetVector2Value(const std::string& key) const;
	Vector3 GetVector3Value(const std::string& key) const;
	bool GetBoolValue(const std::string& key) const;
	std::string GetStringValue(const std::string& key) const;

private:
	std::string chunkName_;
	std::string groupName_;

private:
	static GlobalVariables* globalVariable_;
};