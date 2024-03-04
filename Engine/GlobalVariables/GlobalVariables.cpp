#include "GlobalVariables.h"
#include "Externals/nlohmann/json.hpp"
#include "ImGuiManager/ImGuiManager.h"
#include <fstream>
#include "WindowsInfo/WindowsInfo.h"
#include "Vector2.h"
#include "Vector3.h"

GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables globalVariables;
	return &globalVariables;
}

void GlobalVariables::Update() {
#ifdef _DEBUG
	for (std::map<std::string, Chunk>::iterator itChunk = datas_.begin();
		itChunk != datas_.end(); ++itChunk) {

		const std::string& chunkName = itChunk->first;

		Chunk& chunk = itChunk->second;

		if (!ImGui::Begin(chunkName.c_str(), nullptr, ImGuiWindowFlags_MenuBar)) {
			ImGui::End();
			continue;
		}
		if (!ImGui::BeginMenuBar()) {
			continue;
		}

		for (std::map<std::string, Group>::iterator itGroup = chunk.begin();
			itGroup != chunk.end(); ++itGroup) {

			const std::string& groupName = itGroup->first;

			Group& group = itGroup->second;

			if (!ImGui::BeginMenu(groupName.c_str())) {
				continue;
			}

			for (std::map<std::string, Item>::iterator itItem = group.begin();
				itItem != group.end(); ++itItem) {

				const std::string& itemName = itItem->first;

				Item& item = itItem->second;

				if (std::holds_alternative<int32_t>(item)) {
					int32_t* ptr = std::get_if<int32_t>(&item);
					ImGui::DragInt(itemName.c_str(), ptr, 1);
				} else if (std::holds_alternative<float>(item)) {
					float* ptr = std::get_if<float>(&item);
					ImGui::DragFloat(itemName.c_str(), ptr, 0.01f);
				} else if (std::holds_alternative<Vector2>(item)) {
					Vector2* ptr = std::get_if<Vector2>(&item);
					ImGui::DragFloat2(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.01f);
				} else if (std::holds_alternative<Vector3>(item)) {
					Vector3* ptr = std::get_if<Vector3>(&item);
					ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.01f);
				} else if (std::holds_alternative<bool>(item)) {
					bool* ptr = std::get_if<bool>(&item);
					ImGui::Checkbox(itemName.c_str(), ptr);
				}
			}

			ImGui::Text("\n");

			if (ImGui::Button("Save")) {
				SaveFile(chunkName, groupName);
				std::string message = std::format("{}.json saved", chunkName + "_" + groupName);
				MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
		ImGui::End();
	}
#endif // _DEBUG
}

void GlobalVariables::CreateChunk(const std::string& chunkName)
{
	datas_[chunkName];
}

void GlobalVariables::CreateGroup(const std::string& chunkName, const std::string& groupName)
{
	datas_[chunkName][groupName];
}

void GlobalVariables::CreateGroup(const std::string& groupName)
{

	datas_[kChunkName][groupName];
}

void GlobalVariables::SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, int32_t value) {
	
	Group& group = datas_[chunkName][groupName];

	Item newItem{};
	newItem = value;

	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, float value) {
	Group& group = datas_[chunkName][groupName];

	Item newItem{};
	newItem = value;

	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector2& value) {
	Group& group = datas_[chunkName][groupName];

	Item newItem{};
	newItem = value;

	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector3& value) {
	Group& group = datas_[chunkName][groupName];

	Item newItem{};
	newItem = value;

	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, bool value)
{
	Group& group = datas_[chunkName][groupName];

	Item newItem{};
	newItem = value;

	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::string& value)
{
	Group& group = datas_[chunkName][groupName];

	Item newItem{};
	newItem = value;

	group[key] = newItem;
}

void GlobalVariables::AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, int32_t value) {
	Group& group = datas_[chunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(chunkName, groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, float value) {
	Group& group = datas_[chunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(chunkName, groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector2& value) {
	Group& group = datas_[chunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(chunkName, groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const Vector3& value) {
	Group& group = datas_[chunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(chunkName, groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, bool value)
{
	Group& group = datas_[chunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(chunkName, groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const std::string& value)
{
	Group& group = datas_[chunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(chunkName, groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value)
{
	Group& group = datas_[kChunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(kChunkName, groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value)
{
	Group& group = datas_[kChunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(kChunkName, groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector2& value)
{
	Group& group = datas_[kChunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(kChunkName, groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3& value)
{
	Group& group = datas_[kChunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(kChunkName, groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, bool value)
{
	Group& group = datas_[kChunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(kChunkName, groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const std::string& value)
{
	Group& group = datas_[kChunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(kChunkName, groupName, key, value);
	}
}

int32_t GlobalVariables::GetIntValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const {

	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<int32_t>(group.find(key)->second);
}

float GlobalVariables::GetFloatValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const {

	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<float>(group.find(key)->second);
}

Vector2 GlobalVariables::GetVector2Value(const std::string& chunkName, const std::string& groupName, const std::string& key) const {

	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<Vector2>(group.find(key)->second);
}

Vector3 GlobalVariables::GetVector3Value(const std::string& chunkName, const std::string& groupName, const std::string& key) const {

	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<Vector3>(group.find(key)->second);
}

bool GlobalVariables::GetBoolValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<bool>(group.find(key)->second);
}

std::string GlobalVariables::GetStringValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<std::string>(group.find(key)->second);
}

int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<int32_t>(group.find(key)->second);
}

float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<float>(group.find(key)->second);
}

Vector2 GlobalVariables::GetVector2Value(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<Vector2>(group.find(key)->second);
}

Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<Vector3>(group.find(key)->second);
}

bool GlobalVariables::GetBoolValue(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<bool>(group.find(key)->second);
}

std::string GlobalVariables::GetStringValue(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<std::string>(group.find(key)->second);
}

void GlobalVariables::SaveFile(const std::string& chunkName, const std::string& groupName) {

	std::map<std::string, Chunk>::iterator itChunk = datas_.find(chunkName);

	assert(itChunk != datas_.end());

	std::map<std::string, Group>::iterator itGroup = itChunk->second.find(groupName);

	assert(itGroup != itChunk->second.end());

	nlohmann::json root;

	root = nlohmann::json::object();
	
	root[groupName] = nlohmann::json::object();


	for (std::map<std::string, Item>::iterator itItem = itGroup->second.begin();
	     itItem != itGroup->second.end(); ++itItem) {
		
		const std::string& itemName = itItem->first;

		Item& item = itItem->second;

		if (std::holds_alternative<int32_t>(item)) {
			
			root[groupName][itemName] = std::get<int32_t>(item);
		} else if (std::holds_alternative<float>(item)) {

			root[groupName][itemName] = std::get<float>(item);
		} else if (std::holds_alternative<Vector2>(item)) {

			Vector2 value = std::get<Vector2>(item);
			root[groupName][itemName] = nlohmann::json::array({value.x, value.y});
		} else if (std::holds_alternative<Vector3>(item)) {

			Vector3 value = std::get<Vector3>(item);
			root[groupName][itemName] = nlohmann::json::array({ value.x, value.y, value.z });
		} else if (std::holds_alternative<bool>(item)) {

			root[groupName][itemName] = std::get<bool>(item);
		} else if (std::holds_alternative<std::string>(item)) {

			root[groupName][itemName] = std::get<std::string>(item);
		}

	}

	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(dir)) {
		std::filesystem::create_directories(dir);
	}


	std::string filePath = kDirectoryPath + chunkName + "_" + groupName + ".json";

	std::ofstream ofs;

	ofs.open(filePath);

	if (ofs.fail()) {
#ifdef _DEBUG
		std::string message = "Failed open data file file for write";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
#endif // _DEBUG
		assert(0);
		return;
	}

	ofs << std::setw(4) << root << std::endl;

	ofs.close();
}

void GlobalVariables::LoadFiles() {

	std::filesystem::path dir(kDirectoryPath);

	if (!std::filesystem::exists(dir)) {
		return;
	}

	std::filesystem::directory_iterator dir_it(dir);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		
		const std::filesystem::path& filePath = entry.path();

		std::string extension = filePath.extension().string();

		if (extension.compare(".json") != 0) {
			continue;
		}
		std::string filePathStr = filePath.stem().string();

		size_t underscorePos = filePathStr.find('_');
		std::string chunkName = filePathStr.substr(0, underscorePos);
		std::string groupName = filePathStr.substr(underscorePos + 1);

		LoadFile(chunkName, groupName);
	}
}

void GlobalVariables::LoadFile(const std::string& chunkName, const std::string& groupName) {

	std::string filePath = kDirectoryPath + chunkName + "_" + groupName + ".json";

	std::ifstream ifs;

	ifs.open(filePath);

	if (ifs.fail()) {
#ifdef _DEBUG
		std::string message = "Failed open data file file for read";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
#endif // _DEBUG
		assert(0);
		return;
	}

	nlohmann::json root;

	ifs >> root;

	ifs.close();


	nlohmann::json::iterator itGroup = root.find(groupName);

	assert(itGroup != root.end());


	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		
		const std::string& itemName = itItem.key();


		if (itItem->is_number_integer()) {
			
			int32_t value = itItem->get<int32_t>();
			SetValue(chunkName, groupName, itemName, value);
		} else if (itItem->is_number_float()) {

			double value = itItem->get<double>();
			SetValue(chunkName, groupName, itemName, static_cast<float>(value));
		} else if (itItem->is_array() && itItem->size() == 2) {

			Vector2 value = {itItem->at(0), itItem->at(1)};
			SetValue(chunkName, groupName, itemName, value);
		} else if (itItem->is_array() && itItem->size() == 3) {

			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(chunkName, groupName, itemName, value);
		} else if (itItem->is_boolean()) {

			bool value = itItem->get<bool>();
			SetValue(chunkName, groupName, itemName, value);
		} else if (itItem->is_string()) {

			std::string value = itItem->get<std::string>();
			SetValue(chunkName, groupName, itemName, value);
		}
	}
}
