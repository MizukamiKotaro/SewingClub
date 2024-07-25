#include "Vector2.h"
#include "Vector3.h"
#include "GlobalVariableUser.h"
#include "GlobalVariables.h"

GlobalVariables* GlobalVariableUser::global_ = nullptr;

void GlobalVariableUser::StaticInitialize()
{
	global_ = GlobalVariables::GetInstance();
}

GlobalVariableUser::GlobalVariableUser(const std::string& chunkName, const std::string& groupName, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5)
{
	chunkName_ = chunkName;
	groupName_ = groupName;
	CreateGroup();

	tree_.resize(7);
	tree_ = { tree1, tree2, tree3, tree4, tree5, "_", "_" };
}

void GlobalVariableUser::ResetGroupName(const std::string& groupName)
{
	groupName_ = groupName;
	CreateGroup();
}

void GlobalVariableUser::CreateGroup()
{
	global_->CreateGroup(chunkName_, groupName_);
}

void GlobalVariableUser::AddItem(const std::string& key, const int32_t& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->AddItem<int32_t>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::AddItem(const std::string& key, const float& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->AddItem<float>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::AddItem(const std::string& key, const Vector2& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->AddItem<Vector2>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::AddItem(const std::string& key, const Vector3& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->AddItem<Vector3>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::AddItem(const std::string& key, const Vector4& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->AddItem<Vector4>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::AddItem(const std::string& key, const bool& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->AddItem<bool>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::AddItem(const std::string& key, const std::string& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->AddItem<std::string>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::AddItemColor(const std::string& key, const Vector4& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->AddItemColor(chunkName_, groupName_, key, value, tree);
}

const int32_t& GlobalVariableUser::GetIntValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	return global_->GetIntValue(chunkName_, groupName_, key, tree);
}

const float& GlobalVariableUser::GetFloatValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	return global_->GetFloatValue(chunkName_, groupName_, key, tree);
}

const Vector2& GlobalVariableUser::GetVector2Value(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	return global_->GetVector2Value(chunkName_, groupName_, key, tree);
}

const Vector3& GlobalVariableUser::GetVector3Value(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	return global_->GetVector3Value(chunkName_, groupName_, key, tree);
}

const Vector4& GlobalVariableUser::GetVector4Value(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	return global_->GetVector4Value(chunkName_, groupName_, key, tree);
}

const bool& GlobalVariableUser::GetBoolValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	return global_->GetBoolValue(chunkName_, groupName_, key, tree);
}

const std::string& GlobalVariableUser::GetStringValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	return global_->GetStringValue(chunkName_, groupName_, key, tree);
}

const Vector4& GlobalVariableUser::GetColor(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	return global_->GetColor(chunkName_, groupName_, key, tree);
}

void GlobalVariableUser::SetVariable(const std::string& key, const int32_t& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->SetVariable<int32_t>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::SetVariable(const std::string& key, const float& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->SetVariable<float>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::SetVariable(const std::string& key, const Vector2& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->SetVariable<Vector2>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::SetVariable(const std::string& key, const Vector3& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->SetVariable<Vector3>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::SetVariable(const std::string& key, const Vector4& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->SetVariable<Vector4>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::SetVariable(const std::string& key, const bool& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->SetVariable<bool>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::SetVariable(const std::string& key, const std::string& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	global_->SetVariable<std::string>(chunkName_, groupName_, key, value, tree);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const int32_t& value)
{
	global_->AddItemDontTouchImGui<int32_t>(key, value);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const float& value)
{
	global_->AddItemDontTouchImGui<float>(key, value);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const Vector2& value)
{
	global_->AddItemDontTouchImGui<Vector2>(key, value);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const Vector3& value)
{
	global_->AddItemDontTouchImGui<Vector3>(key, value);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const Vector4& value)
{
	global_->AddItemDontTouchImGui<Vector4>(key, value);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const bool& value)
{
	global_->AddItemDontTouchImGui<bool>(key, value);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const std::string& value)
{
	global_->AddItemDontTouchImGui<std::string>(key, value);
}

const int32_t& GlobalVariableUser::GetIntValueDontTouchImGui(const std::string& key) const
{
	return global_->GetIntValueDontTouchImGui(key);
}

const float& GlobalVariableUser::GetFloatValueDontTouchImGui(const std::string& key) const
{
	return global_->GetFloatValueDontTouchImGui(key);
}

const Vector2& GlobalVariableUser::GetVector2ValueDontTouchImGui(const std::string& key) const
{
	return global_->GetVector2ValueDontTouchImGui(key);
}

const Vector3& GlobalVariableUser::GetVector3ValueDontTouchImGui(const std::string& key) const
{
	return global_->GetVector3ValueDontTouchImGui(key);
}

const Vector4& GlobalVariableUser::GetVector4ValueDontTouchImGui(const std::string& key) const
{
	return global_->GetVector4ValueDontTouchImGui(key);
}

const bool& GlobalVariableUser::GetBoolValueDontTouchImGui(const std::string& key) const
{
	return global_->GetBoolValueDontTouchImGui(key);
}

const std::string& GlobalVariableUser::GetStringValueDontTouchImGui(const std::string& key) const
{
	return global_->GetStringValueDontTouchImGui(key);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const int32_t& value)
{
	global_->SaveAndSetVariableDontTouchImGui<int32_t>(key, value);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const float& value)
{
	global_->SaveAndSetVariableDontTouchImGui<float>(key, value);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const Vector2& value)
{
	global_->SaveAndSetVariableDontTouchImGui<Vector2>(key, value);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const Vector3& value)
{
	global_->SaveAndSetVariableDontTouchImGui<Vector3>(key, value);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const Vector4& value)
{
	global_->SaveAndSetVariableDontTouchImGui<Vector4>(key, value);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const bool& value)
{
	global_->SaveAndSetVariableDontTouchImGui<bool>(key, value);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const std::string& value)
{
	global_->SaveAndSetVariableDontTouchImGui<std::string>(key, value);
}

bool GlobalVariableUser::IsTreeOpen(const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	std::vector<std::string> tree = CreateTree(tree1, tree2, tree3, tree4, tree5, tree6);
	return global_->IsTreeOpen(chunkName_, groupName_, tree);
}

bool GlobalVariableUser::IsTreeOpen(const int32_t& treeNo)
{
	int32_t no = treeNo + 1;
	std::vector<std::string> tree;
	tree.resize(no);
	for (int32_t i = 0; i < no; i++) {
		tree[i] = tree_[i];
	}
	return global_->IsTreeOpen(chunkName_, groupName_, tree);
}

bool GlobalVariableUser::IsTreeOpen() const
{
	return global_->IsTreeOpen(chunkName_, groupName_, tree_);
}

std::vector<std::string> GlobalVariableUser::CreateTree(const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	std::vector<std::string> treeNames = tree_;
	if (tree1 == "_") {
		return treeNames;
	}
	std::vector<std::string> tree = { tree1, tree2, tree3, tree4, tree5, tree6 };
	uint32_t i = 0;
	for (std::string& treeName : treeNames) {
		if (treeName == "_") {
			if (tree[i] == "_") {
				if (treeNames.back() != "_") {
					treeNames.push_back("_");
				}
				return treeNames;
			}
			treeName = tree[i];
			i++;
		}
	}
	if (treeNames.back() != "_") {
		treeNames.push_back("_");
	}
	return treeNames;
}
