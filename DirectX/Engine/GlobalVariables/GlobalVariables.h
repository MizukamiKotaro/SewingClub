#pragma once
#include <variant>
#include <map>
#include <string>

class Vector2;
class Vector3;

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

private:

	using Item = std::variant<int32_t, float, Vector2, Vector3, bool, std::string>;

	using Group = std::map<std::string, Item>;

	using Chunk = std::map<std::string, Group>;

	std::map<std::string, Chunk> datas_;

	const std::string kDirectoryPath = "Resources/GlobalVariables/";

	const std::string kChunkName = "GlobalVariables";
};