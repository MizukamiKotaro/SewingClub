#pragma once
#include <variant>
#include <map>
#include <string>
#include <vector>
#include "Vector4.h"
#include "Externals/nlohmann/json.hpp"

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
	void Finalize();

	void Update();

	void CreateChunk(const std::string& chunkName);
	void CreateGroup(const std::string& chunkName, const std::string& groupName);

	template<typename T>
	void AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const T& value, const std::vector<std::string>& tree);
	void AddItemColor(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector4& value, const std::vector<std::string>& tree);

	const int32_t& GetIntValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::vector<std::string>& tree) const;
	const float& GetFloatValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::vector<std::string>& tree) const;
	const Vector2& GetVector2Value(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::vector<std::string>& tree) const;
	const Vector3& GetVector3Value(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::vector<std::string>& tree) const;
	const Vector4& GetVector4Value(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::vector<std::string>& tree) const;
	const bool& GetBoolValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::vector<std::string>& tree) const;
	const std::string& GetStringValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::vector<std::string>& tree) const;
	const Vector4& GetColor(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::vector<std::string>& tree) const;

	const int32_t& GetIntValueDontTouchImGui(const std::string& key) const;
	const float& GetFloatValueDontTouchImGui(const std::string& key) const;
	const Vector2& GetVector2ValueDontTouchImGui(const std::string& key) const;
	const Vector3& GetVector3ValueDontTouchImGui(const std::string& key) const;
	const Vector4& GetVector4ValueDontTouchImGui(const std::string& key) const;
	const bool& GetBoolValueDontTouchImGui(const std::string& key) const;
	const std::string& GetStringValueDontTouchImGui(const std::string& key) const;

	template<typename T>
	void SetVariable(const std::string& chunkName, const std::string& groupName, const std::string& key, const T& value, const std::vector<std::string>& tree);

	template<typename T>
	void SaveAndSetVariable(const std::string& chunkName, const std::string& groupName, const std::string& key, const T& value, const std::vector<std::string>& tree);

	template<typename T>
	void AddItemDontTouchImGui(const std::string& key, const T& value);

	template<typename T>
	void SaveAndSetVariableDontTouchImGui(const std::string& key, const T& value);

	bool IsTreeOpen(const std::string& chunkName, const std::string& groupName, const std::vector<std::string>& tree);

	void SaveFile(const std::string& chunkName, const std::string& groupName, const bool& isFin = false);

	void LoadFiles();

	void LoadFile(const std::string& chunkName, const std::string& groupName);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables& obj) = delete;
	GlobalVariables& operator=(const GlobalVariables& obj) = delete;

	using Item = std::variant<int32_t, float, Vector2, Vector3, bool, std::string, Vector4>;

	using Group = std::map<std::string, Item>;
	using GroupPtr = std::map<std::string, Item*>;

	using Chunk = std::map<std::string, Group>;

	std::string GetAfterName(const std::string& itemName, const std::string& findName);

	template<typename T>
	const T& GetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::vector<std::string>& tree) const;
	template<typename T>
	const T& GetValueDontTouchImGui(const std::string& key) const;

	template<typename T>
	void SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const T& value, const std::vector<std::string>& tree, const bool& isAddItem = true);

	void ItemToRoot(nlohmann::json& root, Item& item, const std::string& key, const std::vector<std::string>& treeNames, uint32_t level = 0);
	void MakeTreeNames(std::string& key, std::vector<std::string>& treeNames, uint32_t level = 0);

	void RootToSetValue(const std::string& chunkName, const std::string& groupName, nlohmann::json::iterator& itGroup, const std::vector<std::string>& treeNames, uint32_t level = 0);
#ifdef _DEBUG
	void MakeTreeMap(const std::string& chunkName, const std::string& groupName,
		std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, GroupPtr>>>>>>& treeMap,
		Item& item, const std::string& itemName, std::vector<std::string>& treeNames, uint32_t level = 0);

	template <typename T>
	void DrawTreeImGui(T& tree, const std::string& chunkName, const std::string& groupName, const std::string& treeName, uint32_t level = 1);

	void PreparationImGui(const std::string& itemName, Item& item);
#endif // _DEBUG

private:

	std::map<std::string, Chunk> datas_;
	std::map<std::string, Chunk> isTreeOpen_;
	std::map<std::string, bool> isDrawTitle_;
	bool isDraw_;
	const std::string kDirectoryPath = "Resources/GlobalVariables/";
	std::vector<std::string> kTreeName_;
	const std::string kColorName = "COLOR";
};
