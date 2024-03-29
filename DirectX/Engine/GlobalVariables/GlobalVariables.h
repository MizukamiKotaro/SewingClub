#pragma once
#include <variant>
#include <map>
#include <string>

class Vector2;
class Vector3;

enum class ImGuiType {
	DEFAULT,
	SLIDER_DEFAULT,
	DRAG_NORMALIZE,
	SLIDER_NORMALIZE,
	COLOR_EDIT,
};

class GlobalVariables {
public:
	
	static GlobalVariables* GetInstance();

	void Update();

	void CreateChunk(const std::string& chunkName);
	void CreateGroup(const std::string& chunkName, const std::string& groupName);
	void CreateGroup(const std::string& groupName);

	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, int32_t value);
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, float value);
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector2& value);
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector3& value);
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, bool value);
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::string& value);

	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	void AddItem(const std::string& groupName, const std::string& key, float value);
	void AddItem(const std::string& groupName, const std::string& key, const Vector2& value);
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);
	void AddItem(const std::string& groupName, const std::string& key, bool value);
	void AddItem(const std::string& groupName, const std::string& key, const std::string& value);


	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, int32_t value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, float value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector2& value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector3& value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, bool value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::string& value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");

	void AddItem(const std::string& groupName, const std::string& key, int32_t value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& groupName, const std::string& key, float value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& groupName, const std::string& key, const Vector2& value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& groupName, const std::string& key, bool value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void AddItem(const std::string& groupName, const std::string& key, const std::string& value, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");


	int32_t GetIntValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const;
	float GetFloatValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const;
	Vector2 GetVector2Value(const std::string& chunkName, const std::string& groupName, const std::string& key) const;
	Vector3 GetVector3Value(const std::string& chunkName, const std::string& groupName, const std::string& key) const;
	bool GetBoolValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const;
	std::string GetStringValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const;

	int32_t GetIntValue(const std::string& groupName, const std::string& key) const;
	float GetFloatValue(const std::string& groupName, const std::string& key) const;
	Vector2 GetVector2Value(const std::string& groupName, const std::string& key) const;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key) const;
	bool GetBoolValue(const std::string& groupName, const std::string& key) const;
	std::string GetStringValue(const std::string& groupName, const std::string& key) const;


	int32_t GetIntValue(const std::string& chunkName, const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	float GetFloatValue(const std::string& chunkName, const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	Vector2 GetVector2Value(const std::string& chunkName, const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	Vector3 GetVector3Value(const std::string& chunkName, const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	bool GetBoolValue(const std::string& chunkName, const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	std::string GetStringValue(const std::string& chunkName, const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;

	int32_t GetIntValue(const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	float GetFloatValue(const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	Vector2 GetVector2Value(const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	bool GetBoolValue(const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;
	std::string GetStringValue(const std::string& groupName, const std::string& key, int treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_") const;


	void SaveFile(const std::string& chunkName, const std::string& groupName);

	void LoadFiles();

	void LoadFile(const std::string& chunkName, const std::string& groupName);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables& obj) = delete;
	GlobalVariables& operator=(const GlobalVariables& obj) = delete;

	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector2& value);
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector3& value);
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, bool value);
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::string& value);

	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, int32_t value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, float value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector2& value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector3& value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, bool value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::string& value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_");

private:

	using Item = std::variant<int32_t, float, Vector2, Vector3, bool, std::string>;

	using Group = std::map<std::string, Item>;
	using GroupPtr = std::map<std::string, Item*>;

	using Chunk = std::map<std::string, Group>;

	std::map<std::string, Chunk> datas_;

	const std::string kDirectoryPath = "Resources/GlobalVariables/";
	const std::string kChunkName = "GlobalVariables";
	const std::string kTree1Name = "Tree1";
	const std::string kTree2Name = "Tree2";
	const std::string kTree3Name = "Tree3";
};