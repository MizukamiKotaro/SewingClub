#pragma once
#include <string>
#include <stdint.h>
#include <vector>

class Vector2;
class Vector3;
class Vector4;
class GlobalVariables;

class GlobalVariableUser {
public:
	static void StaticInitialize();

public:
	GlobalVariableUser(const std::string& chunkName, const std::string& groupName, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_");
	void ResetGroupName(const std::string& groupName);

	void CreateGroup();

	void AddItem(const std::string& key, const int32_t& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& key, const float& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& key, const Vector2& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& key, const Vector3& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& key, const Vector4& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& key, const bool& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& key, const std::string& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");

	void AddItemColor(const std::string& key, const Vector4& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");

	const int32_t& GetIntValue(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const float& GetFloatValue(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const Vector2& GetVector2Value(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const Vector3& GetVector3Value(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const Vector4& GetVector4Value(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const bool& GetBoolValue(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const std::string& GetStringValue(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;

	const Vector4& GetColor(const std::string& key, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;

	void SetVariable(const std::string& key, const int32_t& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void SetVariable(const std::string& key, const float& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void SetVariable(const std::string& key, const Vector2& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void SetVariable(const std::string& key, const Vector3& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void SetVariable(const std::string& key, const Vector4& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void SetVariable(const std::string& key, const bool& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void SetVariable(const std::string& key, const std::string& value, const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");

	void AddItemDontTouchImGui(const std::string& key, const int32_t& value);
	void AddItemDontTouchImGui(const std::string& key, const float& value);
	void AddItemDontTouchImGui(const std::string& key, const Vector2& value);
	void AddItemDontTouchImGui(const std::string& key, const Vector3& value);
	void AddItemDontTouchImGui(const std::string& key, const Vector4& value);
	void AddItemDontTouchImGui(const std::string& key, const bool& value);
	void AddItemDontTouchImGui(const std::string& key, const std::string& value);

	const int32_t& GetIntValueDontTouchImGui(const std::string& key) const;
	const float& GetFloatValueDontTouchImGui(const std::string& key) const;
	const Vector2& GetVector2ValueDontTouchImGui(const std::string& key) const;
	const Vector3& GetVector3ValueDontTouchImGui(const std::string& key) const;
	const Vector4& GetVector4ValueDontTouchImGui(const std::string& key) const;
	const bool& GetBoolValueDontTouchImGui(const std::string& key) const;
	const std::string& GetStringValueDontTouchImGui(const std::string& key) const;

	void SaveDontTouchImGui(const std::string& key, const int32_t& value);
	void SaveDontTouchImGui(const std::string& key, const float& value);
	void SaveDontTouchImGui(const std::string& key, const Vector2& value);
	void SaveDontTouchImGui(const std::string& key, const Vector3& value);
	void SaveDontTouchImGui(const std::string& key, const Vector4& value);
	void SaveDontTouchImGui(const std::string& key, const bool& value);
	void SaveDontTouchImGui(const std::string& key, const std::string& value);

	bool IsTreeOpen(const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	bool IsTreeOpen(const int32_t& treeNo);
	bool IsTreeOpen() const;

	const std::string& GetChunkName() const { return chunkName_; }

private:
	std::string chunkName_;
	std::string groupName_;

	std::vector<std::string> CreateTree(const std::string& tree1 = "_", const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;

private:
	static GlobalVariables* global_;
	std::vector<std::string> tree_;
};