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

void GlobalVariables::Initialize()
{
	kTreeName_.resize(6);
	for (int i = 0; i < 6; i++) {
		kTreeName_[i] = "Tree" + std::to_string(i + 1);
	}
}

void GlobalVariables::Finalize()
{
	for (std::map<std::string, Chunk>::iterator itChunk = datas_.begin();
		itChunk != datas_.end(); ++itChunk) {
		const std::string& chunkName = itChunk->first;
		Chunk& chunk = itChunk->second;
		for (std::map<std::string, Group>::iterator itGroup = chunk.begin();
			itGroup != chunk.end(); ++itGroup) {
			const std::string& groupName = itGroup->first;
			SaveFile(chunkName, groupName, true);
		}
	}
}

void GlobalVariables::Update() {
#ifdef _DEBUG
	for (std::map<std::string, Chunk>::iterator itChunk = isTreeOpen_.begin(); itChunk != isTreeOpen_.end(); itChunk++) {
		Chunk& chunk = itChunk->second;
		for (std::map<std::string, Group>::iterator itGroup = chunk.begin(); itGroup != chunk.end(); ++itGroup) {
			Group& group = itGroup->second;
			for (std::map<std::string, Item>::iterator itItem = group.begin(); itItem != group.end(); ++itItem) {
				itItem->second = false;
			}
		}
	}

	for (std::map<std::string, Chunk>::iterator itChunk = datas_.begin();
		itChunk != datas_.end(); ++itChunk) {

		const std::string& chunkName = itChunk->first;

		/*if (chunkName == "aaDontTouchPlayData") {
			continue;
		}*/

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

			std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, GroupPtr>>>>>> treeMap;

			for (std::map<std::string, Item>::iterator itItem = group.begin();
				itItem != group.end(); ++itItem) {

				const std::string& itemName = itItem->first;

				Item& item = itItem->second;

				size_t pos = itemName.find(kTreeName_[0]);
				if (pos != std::string::npos) {
					size_t underscorePos = itemName.find("_", pos + 5);
					std::string tree1 = itemName.substr(pos + 5, underscorePos - pos - 5);

					std::string text = itemName.substr(underscorePos + 1);
					pos = text.find(kTreeName_[1]);
					if (pos != std::string::npos) {
						underscorePos = text.find("_", pos + 5);
						std::string tree2 = text.substr(pos + 5, underscorePos - pos - 5);

						text = text.substr(underscorePos + 1);
						pos = text.find(kTreeName_[2]);
						if (pos != std::string::npos) {

							underscorePos = text.find("_", pos + 5);
							std::string tree3 = text.substr(pos + 5, underscorePos - pos - 5);

							text = text.substr(underscorePos + 1);
							pos = text.find(kTreeName_[3]);
							if (pos != std::string::npos) {

								underscorePos = text.find("_", pos + 5);
								std::string tree4 = text.substr(pos + 5, underscorePos - pos - 5);

								text = text.substr(underscorePos + 1);
								pos = text.find(kTreeName_[4]);
								if (pos != std::string::npos) {

									underscorePos = text.find("_", pos + 5);
									std::string tree5 = text.substr(pos + 5, underscorePos - pos - 5);

									text = text.substr(underscorePos + 1);
									pos = text.find(kTreeName_[5]);
									if (pos != std::string::npos) {


										underscorePos = text.find("_", pos + 5);
										if (underscorePos != std::string::npos) {
											std::string tree6 = text.substr(pos + 5, underscorePos - pos - 5);
											text = text.substr(underscorePos + 1);
											treeMap[tree1][tree2][tree3][tree4][tree5][tree6][text] = &item;
											isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6] = false;
										}
									}
									else {
										treeMap[tree1][tree2][tree3][tree4][tree5]["_"][text] = &item;
										isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5] = false;
									}
								}
								else {
									treeMap[tree1][tree2][tree3][tree4]["_"]["_"][text] = &item;
									isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4] = false;
								}
							}
							else {
								treeMap[tree1][tree2][tree3]["_"]["_"]["_"][text] = &item;
								isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3] = false;
							}
						}
						else {
							treeMap[tree1][tree2]["_"]["_"]["_"]["_"][text] = &item;
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2] = false;
						}
					}
					else {
						treeMap[tree1]["_"]["_"]["_"]["_"]["_"][text] = &item;
						isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1] = false;
					}
				}
				else {
					PreparationImGui(itemName, item);
				}
			}

			for (std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, GroupPtr>>>>>>::iterator itTree1 = treeMap.begin();
				itTree1 != treeMap.end(); ++itTree1) {
				const std::string& tree1 = itTree1->first;
				if (ImGui::TreeNode(tree1.c_str())) {
					isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1] = true;
					for (std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, GroupPtr>>>>>::iterator itTree2 = itTree1->second.begin();
						itTree2 != itTree1->second.end(); ++itTree2) {
						const std::string& tree2 = itTree2->first;
						if (tree2 == "_") {
							for (std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, GroupPtr>>>>::iterator itTree3 = itTree2->second.begin();
								itTree3 != itTree2->second.end(); ++itTree3) {
								for (std::map<std::string, std::map<std::string, std::map<std::string, GroupPtr>>>::iterator itTree4 = itTree3->second.begin();
									itTree4 != itTree3->second.end(); ++itTree4) {
									for (std::map<std::string, std::map<std::string, GroupPtr>>::iterator itTree5 = itTree4->second.begin();
										itTree5 != itTree4->second.end(); ++itTree5) {
										for (std::map<std::string, GroupPtr>::iterator itTree6 = itTree5->second.begin();
											itTree6 != itTree5->second.end(); ++itTree6) {
											for (std::map<std::string, Item*>::iterator itItem = itTree6->second.begin();
												itItem != itTree6->second.end(); ++itItem) {

												PreparationImGui(itItem->first, *itItem->second);
											}
										}
									}
								}
							}
						}
						else {
							if (ImGui::TreeNode(tree2.c_str())) {
								isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2] = true;
								for (std::map<std::string, std::map<std::string, std::map<std::string, std::map<std::string, GroupPtr>>>>::iterator itTree3 = itTree2->second.begin();
									itTree3 != itTree2->second.end(); ++itTree3) {
									const std::string& tree3 = itTree3->first;
									if (tree3 == "_") {
										for (std::map<std::string, std::map<std::string, std::map<std::string, GroupPtr>>>::iterator itTree4 = itTree3->second.begin();
											itTree4 != itTree3->second.end(); ++itTree4) {
											for (std::map<std::string, std::map<std::string, GroupPtr>>::iterator itTree5 = itTree4->second.begin();
												itTree5 != itTree4->second.end(); ++itTree5) {
												for (std::map<std::string, GroupPtr>::iterator itTree6 = itTree5->second.begin();
													itTree6 != itTree5->second.end(); ++itTree6) {
													for (std::map<std::string, Item*>::iterator itItem = itTree6->second.begin();
														itItem != itTree6->second.end(); ++itItem) {

														PreparationImGui(itItem->first, *itItem->second);
													}
												}
											}
										}
									}
									else {
										if (ImGui::TreeNode(tree3.c_str())) {
											isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3] = true;
											for (std::map<std::string, std::map<std::string, std::map<std::string, GroupPtr>>>::iterator itTree4 = itTree3->second.begin();
												itTree4 != itTree3->second.end(); ++itTree4) {
												const std::string& tree4 = itTree4->first;
												if (tree4 == "_") {
													for (std::map<std::string, std::map<std::string, GroupPtr>>::iterator itTree5 = itTree4->second.begin();
														itTree5 != itTree4->second.end(); ++itTree5) {
														for (std::map<std::string, GroupPtr>::iterator itTree6 = itTree5->second.begin();
															itTree6 != itTree5->second.end(); ++itTree6) {
															for (std::map<std::string, Item*>::iterator itItem = itTree6->second.begin();
																itItem != itTree6->second.end(); ++itItem) {

																PreparationImGui(itItem->first, *itItem->second);
															}
														}
													}
												}
												else {
													if (ImGui::TreeNode(tree4.c_str())) {
														isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4] = true;
														for (std::map<std::string, std::map<std::string, GroupPtr>>::iterator itTree5 = itTree4->second.begin();
															itTree5 != itTree4->second.end(); ++itTree5) {
															const std::string& tree5 = itTree5->first;
															if (tree5 == "_") {
																for (std::map<std::string, GroupPtr>::iterator itTree6 = itTree5->second.begin();
																	itTree6 != itTree5->second.end(); ++itTree6) {
																	for (std::map<std::string, Item*>::iterator itItem = itTree6->second.begin();
																		itItem != itTree6->second.end(); ++itItem) {

																		PreparationImGui(itItem->first, *itItem->second);
																	}
																}
															}
															else {
																if (ImGui::TreeNode(tree5.c_str())) {
																	isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5] = true;
																	for (std::map<std::string, GroupPtr>::iterator itTree6 = itTree5->second.begin();
																		itTree6 != itTree5->second.end(); ++itTree6) {
																		const std::string& tree6 = itTree6->first;
																		if (tree6 == "_") {
																			for (std::map<std::string, Item*>::iterator itItem = itTree6->second.begin();
																				itItem != itTree6->second.end(); ++itItem) {

																				PreparationImGui(itItem->first, *itItem->second);
																			}
																		}
																		else {
																			if (ImGui::TreeNode(tree6.c_str())) {
																				isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6] = true;

																				for (std::map<std::string, Item*>::iterator itItem = itTree6->second.begin();
																					itItem != itTree6->second.end(); ++itItem) {

																					PreparationImGui(itItem->first, *itItem->second);
																				}


																				ImGui::TreePop();
																			}
																		}
																	}
																	ImGui::TreePop();
																}
															}
														}
														ImGui::TreePop();
													}
												}
											}
											ImGui::TreePop();
										}
									}
								}
								ImGui::TreePop();
							}
						}
					}
					ImGui::TreePop();
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

template<typename T>
void GlobalVariables::SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const T& value) {
	Group& group = datas_[chunkName][groupName];
	
	Item newItem{};
	newItem = value;
	
	group[key] = newItem;
}

template void GlobalVariables::SetValue<int32_t>(const std::string&, const std::string&, const std::string&, const int32_t&);
template void GlobalVariables::SetValue<float>(const std::string&, const std::string&, const std::string&, const float&);
template void GlobalVariables::SetValue<Vector2>(const std::string&, const std::string&, const std::string&, const Vector2&);
template void GlobalVariables::SetValue<Vector3>(const std::string&, const std::string&, const std::string&, const Vector3&);
template void GlobalVariables::SetValue<bool>(const std::string&, const std::string&, const std::string&, const bool&);
template void GlobalVariables::SetValue<std::string>(const std::string&, const std::string&, const std::string&, const std::string&);

template<typename T>
void GlobalVariables::SetValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const T& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	Group& group = datas_[chunkName][groupName];

	Item newItem{};
	newItem = value;

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		if (group.find(name) == group.end()) {
			isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1] = false;
		}
		group[name] = newItem;
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			if (group.find(name) == group.end()) {
				isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1] = false;
				isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2] = false;
			}
			group[name] = newItem;
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				if (group.find(name) == group.end()) {
					isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1] = false;
					isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2] = false;
					isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3] = false;
				}
				group[name] = newItem;
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					if (group.find(name) == group.end()) {
						isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1] = false;
						isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2] = false;
						isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3] = false;
						isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4] = false;
					}
					group[name] = newItem;
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						if (group.find(name) == group.end()) {
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1] = false;
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2] = false;
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3] = false;
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4] = false;
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5] = false;
						}
						group[name] = newItem;
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						if (group.find(name) == group.end()) {
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1] = false;
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2] = false;
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3] = false;
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4] = false;
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5] = false;
							isTreeOpen_[chunkName][groupName][kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6] = false;
						}
						group[name] = newItem;
					}
				}
			}
		}
	}
}
template void GlobalVariables::SetValue<int32_t>(const std::string&, const std::string&, const std::string&, const int32_t&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SetValue<float>(const std::string&, const std::string&, const std::string&, const float&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SetValue<Vector2>(const std::string&, const std::string&, const std::string&, const Vector2&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SetValue<Vector3>(const std::string&, const std::string&, const std::string&, const Vector3&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SetValue<bool>(const std::string&, const std::string&, const std::string&, const bool&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SetValue<std::string>(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);


template<typename T>
void GlobalVariables::AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const T& value) {
	Group& group = datas_[chunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(chunkName, groupName, key, value);
	}
}
template void GlobalVariables::AddItem<int32_t>(const std::string&, const std::string&, const std::string&, const int32_t&);
template void GlobalVariables::AddItem<float>(const std::string&, const std::string&, const std::string&, const float&);
template void GlobalVariables::AddItem<Vector2>(const std::string&, const std::string&, const std::string&, const Vector2&);
template void GlobalVariables::AddItem<Vector3>(const std::string&, const std::string&, const std::string&, const Vector3&);
template void GlobalVariables::AddItem<bool>(const std::string&, const std::string&, const std::string&, const bool&);
template void GlobalVariables::AddItem<std::string>(const std::string&, const std::string&, const std::string&, const std::string&);

template<typename T>
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const T& value) {
	Group& group = datas_[kChunkName][groupName];
	if (group.find(key) == group.end()) {
		SetValue(kChunkName, groupName, key, value);
	}
}
template void GlobalVariables::AddItem<int32_t>(const std::string&, const std::string&, const int32_t&);
template void GlobalVariables::AddItem<float>(const std::string&, const std::string&, const float&);
template void GlobalVariables::AddItem<Vector2>(const std::string&, const std::string&, const Vector2&);
template void GlobalVariables::AddItem<Vector3>(const std::string&, const std::string&, const Vector3&);
template void GlobalVariables::AddItem<bool>(const std::string&, const std::string&, const bool&);
template void GlobalVariables::AddItem<std::string>(const std::string&, const std::string&, const std::string&);

template<typename T>
void GlobalVariables::AddItem(const std::string& chunkName, const std::string& groupName, const std::string& key, const T& value, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	Group& group = datas_[chunkName][groupName];
	std::string name;

	if(treeNum){}

	if (tree2 == "_") {
		name = kTreeName_[0] + tree1 + "_" + key;
		if (group.find(name) == group.end()) {
			SetValue(chunkName, groupName, key, value, tree1);
		}
	}
	else {
		if (tree3 == "_") {
			name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			if (group.find(name) == group.end()) {
				SetValue(chunkName, groupName, key, value, tree1, tree2);
			}
		}
		else {
			if (tree4 == "_") {
				name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				if (group.find(name) == group.end()) {
					SetValue(chunkName, groupName, key, value, tree1, tree2, tree3);
				}
			}
			else {
				if (tree5 == "_") {
					name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					if (group.find(name) == group.end()) {
						SetValue(chunkName, groupName, key, value, tree1, tree2, tree3, tree4);
					}
				}
				else {
					if (tree6 == "_") {
						name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						if (group.find(name) == group.end()) {
							SetValue(chunkName, groupName, key, value, tree1, tree2, tree3, tree4, tree5);
						}
					}
					else {
						name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						if (group.find(name) == group.end()) {
							SetValue(chunkName, groupName, key, value, tree1, tree2, tree3, tree4, tree5, tree6);
						}
					}
				}
			}
		}
	}
}
template void GlobalVariables::AddItem<int32_t>(const std::string&, const std::string&, const std::string&, const int32_t&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::AddItem<float>(const std::string&, const std::string&, const std::string&, const float&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::AddItem<Vector2>(const std::string&, const std::string&, const std::string&, const Vector2&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::AddItem<Vector3>(const std::string&, const std::string&, const std::string&, const Vector3&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::AddItem<bool>(const std::string&, const std::string&, const std::string&, const bool&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::AddItem<std::string>(const std::string&, const std::string&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

template<typename T>
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const T& value, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	Group& group = datas_[kChunkName][groupName];
	std::string name;

	if (treeNum) {}

	if (tree2 == "_") {
		name = kTreeName_[0] + tree1 + "_" + key;
		if (group.find(name) == group.end()) {
			SetValue(kChunkName, groupName, key, value, tree1);
		}
	}
	else {
		if (tree3 == "_") {
			name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			if (group.find(name) == group.end()) {
				SetValue(kChunkName, groupName, key, value, tree1, tree2);
			}
		}
		else {
			if (tree4 == "_") {
				name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				if (group.find(name) == group.end()) {
					SetValue(kChunkName, groupName, key, value, tree1, tree2, tree3);
				}
			}
			else {
				if (tree5 == "_") {
					name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					if (group.find(name) == group.end()) {
						SetValue(kChunkName, groupName, key, value, tree1, tree2, tree3, tree4);
					}
				}
				else {
					if (tree6 == "_") {
						name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						if (group.find(name) == group.end()) {
							SetValue(kChunkName, groupName, key, value, tree1, tree2, tree3, tree4, tree5);
						}
					}
					else {
						name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						if (group.find(name) == group.end()) {
							SetValue(kChunkName, groupName, key, value, tree1, tree2, tree3, tree4, tree5, tree6);
						}
					}
				}
			}
		}
	}
}
template void GlobalVariables::AddItem<int32_t>(const std::string&, const std::string&, const int32_t&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::AddItem<float>(const std::string&, const std::string&, const float&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::AddItem<Vector2>(const std::string&, const std::string&, const Vector2&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::AddItem<Vector3>(const std::string&, const std::string&, const Vector3&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::AddItem<bool>(const std::string&, const std::string&, const bool&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::AddItem<std::string>(const std::string&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);


template<typename T>
void GlobalVariables::GetValue(T& value, const std::string& chunkName, const std::string& groupName, const std::string& key) const {
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	value = std::get<T>(group.find(key)->second);
}
template void GlobalVariables::GetValue<int32_t>(int32_t&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<float>(float&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<Vector2>(Vector2&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<Vector3>(Vector3&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<bool>(bool&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<std::string>(std::string&, const std::string&, const std::string&, const std::string&) const;

const int32_t& GlobalVariables::GetIntValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const {

	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<int32_t>(group.find(key)->second);
}

const float& GlobalVariables::GetFloatValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const {

	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<float>(group.find(key)->second);
}

const Vector2& GlobalVariables::GetVector2Value(const std::string& chunkName, const std::string& groupName, const std::string& key) const {

	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<Vector2>(group.find(key)->second);
}

const Vector3& GlobalVariables::GetVector3Value(const std::string& chunkName, const std::string& groupName, const std::string& key) const {

	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<Vector3>(group.find(key)->second);
}

const bool& GlobalVariables::GetBoolValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<bool>(group.find(key)->second);
}

const std::string& GlobalVariables::GetStringValue(const std::string& chunkName, const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<std::string>(group.find(key)->second);
}


template<typename T>
void GlobalVariables::GetValue(T& value, const std::string& groupName, const std::string& key) const {
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	value = std::get<T>(group.find(key)->second);
}
template void GlobalVariables::GetValue<int32_t>(int32_t&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<float>(float&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<Vector2>(Vector2&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<Vector3>(Vector3&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<bool>(bool&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<std::string>(std::string&, const std::string&, const std::string&) const;

const int32_t& GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<int32_t>(group.find(key)->second);
}

const float& GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<float>(group.find(key)->second);
}

const Vector2& GlobalVariables::GetVector2Value(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<Vector2>(group.find(key)->second);
}

const Vector3& GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<Vector3>(group.find(key)->second);
}

const bool& GlobalVariables::GetBoolValue(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<bool>(group.find(key)->second);
}

const std::string& GlobalVariables::GetStringValue(const std::string& groupName, const std::string& key) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	assert(group.find(key) != group.end());
	return std::get<std::string>(group.find(key)->second);
}

template<typename T>
void GlobalVariables::GetValue(T& value, const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const {
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		value = std::get<T>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			value = std::get<T>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				value = std::get<T>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					value = std::get<T>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						value = std::get<T>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						value = std::get<T>(group.find(name)->second);
					}
				}
			}
		}
	}
}
template void GlobalVariables::GetValue<int32_t>(int32_t&, const std::string&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<float>(float&, const std::string&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<Vector2>(Vector2&, const std::string&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<Vector3>(Vector3&, const std::string&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<bool>(bool&, const std::string&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<std::string>(std::string&, const std::string&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;


const int32_t& GlobalVariables::GetIntValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<int32_t>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<int32_t>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<int32_t>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<int32_t>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<int32_t>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<int32_t>(group.find(name)->second);
					}
				}
			}
		}
	}
}

const float& GlobalVariables::GetFloatValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<float>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<float>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<float>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<float>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<float>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<float>(group.find(name)->second);
					}
				}
			}
		}
	}
}

const Vector2& GlobalVariables::GetVector2Value(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<Vector2>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<Vector2>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<Vector2>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<Vector2>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<Vector2>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<Vector2>(group.find(name)->second);
					}
				}
			}
		}
	}
}

const Vector3& GlobalVariables::GetVector3Value(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<Vector3>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<Vector3>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<Vector3>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<Vector3>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<Vector3>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<Vector3>(group.find(name)->second);
					}
				}
			}
		}
	}
}

const bool& GlobalVariables::GetBoolValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<bool>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<bool>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<bool>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<bool>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<bool>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<bool>(group.find(name)->second);
					}
				}
			}
		}
	}
}

const std::string& GlobalVariables::GetStringValue(const std::string& chunkName, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(chunkName) != datas_.end());
	const Chunk& chunk = datas_.at(chunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<std::string>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<std::string>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<std::string>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<std::string>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<std::string>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<std::string>(group.find(name)->second);
					}
				}
			}
		}
	}
}



template<typename T>
void GlobalVariables::GetValue(T& value, const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const {
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);

	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		value = std::get<T>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			value = std::get<T>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				value = std::get<T>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					value = std::get<T>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						value = std::get<T>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						value = std::get<T>(group.find(name)->second);
					}
				}
			}
		}
	}
}
template void GlobalVariables::GetValue<int32_t>(int32_t&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<float>(float&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<Vector2>(Vector2&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<Vector3>(Vector3&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<bool>(bool&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;
template void GlobalVariables::GetValue<std::string>(std::string&, const std::string&, const std::string&, const int&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&) const;


const int32_t& GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<int32_t>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<int32_t>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<int32_t>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<int32_t>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<int32_t>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<int32_t>(group.find(name)->second);
					}
				}
			}
		}
	}
}

const float& GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<float>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<float>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<float>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<float>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<float>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<float>(group.find(name)->second);
					}
				}
			}
		}
	}
}

const Vector2& GlobalVariables::GetVector2Value(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<Vector2>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<Vector2>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<Vector2>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<Vector2>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<Vector2>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<Vector2>(group.find(name)->second);
					}
				}
			}
		}
	}
}

const Vector3& GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<Vector3>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<Vector3>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<Vector3>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<Vector3>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<Vector3>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<Vector3>(group.find(name)->second);
					}
				}
			}
		}
	}
}

const bool& GlobalVariables::GetBoolValue(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<bool>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<bool>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<bool>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<bool>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<bool>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<bool>(group.find(name)->second);
					}
				}
			}
		}
	}
}

const std::string& GlobalVariables::GetStringValue(const std::string& groupName, const std::string& key, const int& treeNum, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	assert(datas_.find(kChunkName) != datas_.end());
	const Chunk& chunk = datas_.at(kChunkName);

	assert(chunk.find(groupName) != chunk.end());
	const Group& group = chunk.at(groupName);
	if (treeNum) {}

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1 + "_" + key;
		assert(group.find(name) != group.end());
		return std::get<std::string>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + key;
			assert(group.find(name) != group.end());
			return std::get<std::string>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + key;
				assert(group.find(name) != group.end());
				return std::get<std::string>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + key;
					assert(group.find(name) != group.end());
					return std::get<std::string>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<std::string>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6 + "_" + key;
						assert(group.find(name) != group.end());
						return std::get<std::string>(group.find(name)->second);
					}
				}
			}
		}
	}
}

template<typename T>
void GlobalVariables::SetVariable(const std::string& chunkName, const std::string& groupName, const std::string& key, const T& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree1 == "_") {
		SetValue(chunkName, groupName, key, value);
	}
	else {
		SetValue(chunkName, groupName, key, value, tree1, tree2, tree3, tree4, tree5, tree6);
	}
}
template void GlobalVariables::SetVariable<int32_t>(const std::string&, const std::string&, const std::string&, const int32_t&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SetVariable<float>(const std::string&, const std::string&, const std::string&, const float&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SetVariable<Vector2>(const std::string&, const std::string&, const std::string&, const Vector2&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SetVariable<Vector3>(const std::string&, const std::string&, const std::string&, const Vector3&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SetVariable<bool>(const std::string&, const std::string&, const std::string&, const bool&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SetVariable<std::string>(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

template<typename T>
void GlobalVariables::SaveAndSetVariable(const std::string& chunkName, const std::string& groupName, const std::string& key, const T& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree1 == "_") {
		SetValue(chunkName, groupName, key, value);
	}
	else {
		SetValue(chunkName, groupName, key, value, tree1, tree2, tree3, tree4, tree5, tree6);
	}

	SaveFile(chunkName, groupName);
}
template void GlobalVariables::SaveAndSetVariable<int32_t>(const std::string&, const std::string&, const std::string&, const int32_t&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SaveAndSetVariable<float>(const std::string&, const std::string&, const std::string&, const float&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SaveAndSetVariable<Vector2>(const std::string&, const std::string&, const std::string&, const Vector2&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SaveAndSetVariable<Vector3>(const std::string&, const std::string&, const std::string&, const Vector3&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SaveAndSetVariable<bool>(const std::string&, const std::string&, const std::string&, const bool&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
template void GlobalVariables::SaveAndSetVariable<std::string>(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);


template<typename T>
void GlobalVariables::AddItemDontTouchImGui(const std::string& key, const T& value)
{
	Group& group = datas_["aaDontTouchPlayData"]["DontTouch"];
	if (group.find(key) == group.end()) {
		SetValue("aaDontTouchPlayData", "DontTouch", key, value);
	}
}
template void GlobalVariables::AddItemDontTouchImGui<int32_t>(const std::string&, const int32_t&);
template void GlobalVariables::AddItemDontTouchImGui<float>(const std::string&, const float&);
template void GlobalVariables::AddItemDontTouchImGui<Vector2>(const std::string&, const Vector2&);
template void GlobalVariables::AddItemDontTouchImGui<Vector3>(const std::string&, const Vector3&);
template void GlobalVariables::AddItemDontTouchImGui<bool>(const std::string&, const bool&);
template void GlobalVariables::AddItemDontTouchImGui<std::string>(const std::string&, const std::string&);

const int32_t& GlobalVariables::GetIntValueDontTouchImGui(const std::string& key) const
{
	return GetIntValue("aaDontTouchPlayData", "DontTouch", key);
}

const float& GlobalVariables::GetFloatValueDontTouchImGui(const std::string& key) const
{
	return GetFloatValue("aaDontTouchPlayData", "DontTouch", key);
}

const Vector2& GlobalVariables::GetVector2ValueDontTouchImGui(const std::string& key) const
{
	return GetVector2Value("aaDontTouchPlayData", "DontTouch", key);
}

const Vector3& GlobalVariables::GetVector3ValueDontTouchImGui(const std::string& key) const
{
	return GetVector3Value("aaDontTouchPlayData", "DontTouch", key);
}

const bool& GlobalVariables::GetBoolValueDontTouchImGui(const std::string& key) const
{
	return GetBoolValue("aaDontTouchPlayData", "DontTouch", key);
}

const std::string& GlobalVariables::GetStringValueDontTouchImGui(const std::string& key) const
{
	return GetStringValue("aaDontTouchPlayData", "DontTouch", key);
}

template<typename T>
void GlobalVariables::SaveAndSetVariableDontTouchImGui(const std::string& key, const T& value)
{
	SetValue("aaDontTouchPlayData", "DontTouch", key, value);
	SaveFile("aaDontTouchPlayData", "DontTouch");
}
template void GlobalVariables::SaveAndSetVariableDontTouchImGui<int32_t>(const std::string&, const int32_t&);
template void GlobalVariables::SaveAndSetVariableDontTouchImGui<float>(const std::string&, const float&);
template void GlobalVariables::SaveAndSetVariableDontTouchImGui<Vector2>(const std::string&, const Vector2&);
template void GlobalVariables::SaveAndSetVariableDontTouchImGui<Vector3>(const std::string&, const Vector3&);
template void GlobalVariables::SaveAndSetVariableDontTouchImGui<bool>(const std::string&, const bool&);
template void GlobalVariables::SaveAndSetVariableDontTouchImGui<std::string>(const std::string&, const std::string&);

bool GlobalVariables::IsTreeOpen(const std::string& chunkName, const std::string& groupName, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (isTreeOpen_.find(chunkName) == isTreeOpen_.end()) {
		return false;
	}
	const Chunk& chunk = isTreeOpen_.at(chunkName);

	if (chunk.find(groupName) == chunk.end()) {
		return false;
	}
	const Group& group = chunk.at(groupName);

	if (tree2 == "_") {
		std::string name = kTreeName_[0] + tree1;
		if (group.find(name) == group.end()) {
			return false;
		}
		return std::get<bool>(group.find(name)->second);
	}
	else {
		if (tree3 == "_") {
			std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2;
			if (group.find(name) == group.end()) {
				return false;
			}
			return std::get<bool>(group.find(name)->second);
		}
		else {
			if (tree4 == "_") {
				std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3;
				if (group.find(name) == group.end()) {
					return false;
				}
				return std::get<bool>(group.find(name)->second);
			}
			else {
				if (tree5 == "_") {
					std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4;
					if (group.find(name) == group.end()) {
						return false;
					}
					return std::get<bool>(group.find(name)->second);
				}
				else {
					if (tree6 == "_") {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5;
						if (group.find(name) == group.end()) {
							return false;
						}
						return std::get<bool>(group.find(name)->second);
					}
					else {
						std::string name = kTreeName_[0] + tree1 + "_" + kTreeName_[1] + tree2 + "_" + kTreeName_[2] + tree3 + "_" + kTreeName_[3] + tree4 + "_" + kTreeName_[4] + tree5 + "_" + kTreeName_[5] + tree6;
						if (group.find(name) == group.end()) {
							return false;
						}
						return std::get<bool>(group.find(name)->second);
					}
				}
			}
		}
	}
}

void GlobalVariables::SaveFile(const std::string& chunkName, const std::string& groupName, const bool& isFin) {

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

	std::string filePath;
	if (isFin) {
		filePath = kDirectoryPath + "zzESC_" + chunkName + "_" + groupName + ".json";
	}
	else {
		filePath = kDirectoryPath + chunkName + "_" + groupName + ".json";
	}

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

	if (chunkName == "zzESC") {
		return;
	}

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

#ifdef _DEBUG
void GlobalVariables::PreparationImGui(const std::string& itemName, Item& item)
{
	if (std::holds_alternative<int32_t>(item)) {
		int32_t* ptr = std::get_if<int32_t>(&item);
		ImGui::DragInt(itemName.c_str(), ptr, 1);
	}
	else if (std::holds_alternative<float>(item)) {
		float* ptr = std::get_if<float>(&item);
		ImGui::DragFloat(itemName.c_str(), ptr, 0.01f);
	}
	else if (std::holds_alternative<Vector2>(item)) {
		Vector2* ptr = std::get_if<Vector2>(&item);
		ImGui::DragFloat2(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.01f);
	}
	else if (std::holds_alternative<Vector3>(item)) {
		Vector3* ptr = std::get_if<Vector3>(&item);
		ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.01f);
	}
	else if (std::holds_alternative<bool>(item)) {
		bool* ptr = std::get_if<bool>(&item);
		ImGui::Checkbox(itemName.c_str(), ptr);
	}
}
#endif // _DEBUG