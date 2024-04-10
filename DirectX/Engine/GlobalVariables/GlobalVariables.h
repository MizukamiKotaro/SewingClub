#pragma once
#include <variant>
#include <map>
#include <string>
#include <vector>

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
	void Initialize();

	void Update();

	void CreateChunk(const std::string& chunkName);
	void CreateGroup(const std::string& chunkName, const std::string& groupName);
	void CreateGroup(const std::string& groupName);

	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const int32_t& value);
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const float& value);
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector2& value);
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector3& value);
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const bool& value);
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::string& value);

	void AddItem(const std::string& groupName, const std::string& key, const int32_t& value);
	void AddItem(const std::string& groupName, const std::string& key, const float& value);
	void AddItem(const std::string& groupName, const std::string& key, const Vector2& value);
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);
	void AddItem(const std::string& groupName, const std::string& key, const bool& value);
	void AddItem(const std::string& groupName, const std::string& key, const std::string& value);


	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const int32_t& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const float& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector2& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector3& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const bool& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::string& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");

	void AddItem(const std::string& groupName, const std::string& key, const int32_t& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& groupName, const std::string& key, const float& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& groupName, const std::string& key, const Vector2& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& groupName, const std::string& key, const bool& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void AddItem(const std::string& groupName, const std::string& key, const std::string& value, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");


	const int32_t& GetIntValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const;
	const float& GetFloatValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const;
	const Vector2& GetVector2Value(const std::string& chunkName, const std::string& groupName, const std::string& key) const;
	const Vector3& GetVector3Value(const std::string& chunkName, const std::string& groupName, const std::string& key) const;
	const bool& GetBoolValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const;
	const std::string& GetStringValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const;

	const int32_t& GetIntValue(const std::string& groupName, const std::string& key) const;
	const float& GetFloatValue(const std::string& groupName, const std::string& key) const;
	const Vector2& GetVector2Value(const std::string& groupName, const std::string& key) const;
	const Vector3& GetVector3Value(const std::string& groupName, const std::string& key) const;
	const bool& GetBoolValue(const std::string& groupName, const std::string& key) const;
	const std::string& GetStringValue(const std::string& groupName, const std::string& key) const;


	const int32_t& GetIntValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const float& GetFloatValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const Vector2& GetVector2Value(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const Vector3& GetVector3Value(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const bool& GetBoolValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const std::string& GetStringValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;

	const int32_t& GetIntValue(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const float& GetFloatValue(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const Vector2& GetVector2Value(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const Vector3& GetVector3Value(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const bool& GetBoolValue(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;
	const std::string& GetStringValue(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_") const;

	bool IsTreeOpen(const std::string& chunkName, const std::string& groupName, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");

	void SaveFile(const std::string& chunkName, const std::string& groupName);

	void LoadFiles();

	void LoadFile(const std::string& chunkName, const std::string& groupName);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables& obj) = delete;
	GlobalVariables& operator=(const GlobalVariables& obj) = delete;

	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const int32_t& value);
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const float& value);
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector2& value);
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector3& value);
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const bool& value);
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::string& value);

	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const int32_t& value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const float& value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector2& value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector3& value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const bool& value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::string& value, const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_", const std::string& tree6 = "_");

private:

	using Item = std::variant<int32_t, float, Vector2, Vector3, bool, std::string>;

	using Group = std::map<std::string, Item>;
	using GroupPtr = std::map<std::string, Item*>;

	using Chunk = std::map<std::string, Group>;

	std::map<std::string, Chunk> datas_;
	std::map<std::string, Chunk> isTreeOpen_;

	const std::string kDirectoryPath = "Resources/GlobalVariables/";
	const std::string kChunkName = "GlobalVariables";
	std::vector<std::string> kTreeName_;
};