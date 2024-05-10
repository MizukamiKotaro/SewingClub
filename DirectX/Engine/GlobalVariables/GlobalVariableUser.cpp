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

	if (tree5 != "_") {
		tree_.resize(5);
		tree_[0] = tree1;
		tree_[1] = tree2;
		tree_[2] = tree3;
		tree_[3] = tree4;
		tree_[4] = tree5;
	}
	else if (tree4 != "_") {
		tree_.resize(4);
		tree_[0] = tree1;
		tree_[1] = tree2;
		tree_[2] = tree3;
		tree_[3] = tree4;
	}
	else if (tree3 != "_") {
		tree_.resize(3);
		tree_[0] = tree1;
		tree_[1] = tree2;
		tree_[2] = tree3;
	}
	else if (tree2 != "_") {
		tree_.resize(2);
		tree_[0] = tree1;
		tree_[1] = tree2;
	}
	else if (tree1 != "_") {
		tree_.resize(1);
		tree_[0] = tree1;
	}
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
	if (tree_.size() == 0) {
		if (tree1 == "_") {
			global_->AddItem(chunkName_, groupName_, key, value);
		}
		else {
			global_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3, tree4, tree5, tree6);
		}
	}
	else if (tree_.size() == 1) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

void GlobalVariableUser::AddItem(const std::string& key, const float& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 0) {
		if (tree1 == "_") {
			global_->AddItem(chunkName_, groupName_, key, value);
		}
		else {
			global_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3, tree4, tree5, tree6);
		}
	}
	else if (tree_.size() == 1) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

void GlobalVariableUser::AddItem(const std::string& key, const Vector2& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 0) {
		if (tree1 == "_") {
			global_->AddItem(chunkName_, groupName_, key, value);
		}
		else {
			global_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3, tree4, tree5, tree6);
		}
	}
	else if (tree_.size() == 1) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

void GlobalVariableUser::AddItem(const std::string& key, const Vector3& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 0) {
		if (tree1 == "_") {
			global_->AddItem(chunkName_, groupName_, key, value);
		}
		else {
			global_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3, tree4, tree5, tree6);
		}
	}
	else if (tree_.size() == 1) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

void GlobalVariableUser::AddItem(const std::string& key, const bool& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 0) {
		if (tree1 == "_") {
			global_->AddItem(chunkName_, groupName_, key, value);
		}
		else {
			global_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3, tree4, tree5, tree6);
		}
	}
	else if (tree_.size() == 1) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

void GlobalVariableUser::AddItem(const std::string& key, const std::string& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 0) {
		if (tree1 == "_") {
			global_->AddItem(chunkName_, groupName_, key, value);
		}
		else {
			global_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3, tree4, tree5, tree6);
		}
	}
	else if (tree_.size() == 1) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(chunkName_, groupName_, key, value, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

const int32_t& GlobalVariableUser::GetIntValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetIntValue(chunkName_, groupName_, key, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		return global_->GetIntValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		return global_->GetIntValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		return global_->GetIntValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		return global_->GetIntValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}

	if (tree1 == "_") {
		return global_->GetIntValue(chunkName_, groupName_, key);
	}
	else {
		return global_->GetIntValue(chunkName_, groupName_, key, 0, tree1, tree2, tree3, tree4, tree5, tree6);
	}
}

const float& GlobalVariableUser::GetFloatValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetFloatValue(chunkName_, groupName_, key, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		return global_->GetFloatValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		return global_->GetFloatValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		return global_->GetFloatValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		return global_->GetFloatValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}

	if (tree1 == "_") {
		return global_->GetFloatValue(chunkName_, groupName_, key);
	}
	else {
		return global_->GetFloatValue(chunkName_, groupName_, key, 0, tree1, tree2, tree3, tree4, tree5, tree6);
	}
}

const Vector2& GlobalVariableUser::GetVector2Value(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetVector2Value(chunkName_, groupName_, key, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		return global_->GetVector2Value(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		return global_->GetVector2Value(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		return global_->GetVector2Value(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		return global_->GetVector2Value(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}

	if (tree1 == "_") {
		return global_->GetVector2Value(chunkName_, groupName_, key);
	}
	else {
		return global_->GetVector2Value(chunkName_, groupName_, key, 0, tree1, tree2, tree3, tree4, tree5, tree6);
	}
}

const Vector3& GlobalVariableUser::GetVector3Value(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetVector3Value(chunkName_, groupName_, key, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		return global_->GetVector3Value(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		return global_->GetVector3Value(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		return global_->GetVector3Value(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		return global_->GetVector3Value(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}

	if (tree1 == "_") {
		return global_->GetVector3Value(chunkName_, groupName_, key);
	}
	else {
		return global_->GetVector3Value(chunkName_, groupName_, key, 0, tree1, tree2, tree3, tree4, tree5, tree6);
	}
}

const bool& GlobalVariableUser::GetBoolValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetBoolValue(chunkName_, groupName_, key, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		return global_->GetBoolValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		return global_->GetBoolValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		return global_->GetBoolValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		return global_->GetBoolValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}

	if (tree1 == "_") {
		return global_->GetBoolValue(chunkName_, groupName_, key);
	}
	else {
		return global_->GetBoolValue(chunkName_, groupName_, key, 0, tree1, tree2, tree3, tree4, tree5, tree6);
	}
}

const std::string& GlobalVariableUser::GetStringValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetStringValue(chunkName_, groupName_, key, 0, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		return global_->GetStringValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		return global_->GetStringValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		return global_->GetStringValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		return global_->GetStringValue(chunkName_, groupName_, key, 0, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}

	if (tree1 == "_") {
		return global_->GetStringValue(chunkName_, groupName_, key);
	}
	else {
		return global_->GetStringValue(chunkName_, groupName_, key, 0, tree1, tree2, tree3, tree4, tree5, tree6);
	}
}

void GlobalVariableUser::SetVariable(const std::string& key, const int32_t& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 0) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree1, tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 1) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

void GlobalVariableUser::SetVariable(const std::string& key, const float& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 0) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree1, tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 1) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

void GlobalVariableUser::SetVariable(const std::string& key, const Vector2& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 0) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree1, tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 1) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

void GlobalVariableUser::SetVariable(const std::string& key, const Vector3& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 0) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree1, tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 1) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

void GlobalVariableUser::SetVariable(const std::string& key, const bool& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 0) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree1, tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 1) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

void GlobalVariableUser::SetVariable(const std::string& key, const std::string& value, const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 0) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree1, tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 1) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		global_->SetVariable(chunkName_, groupName_, key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const int32_t& value)
{
	global_->AddItemDontTouchImGui(key, value);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const float& value)
{
	global_->AddItemDontTouchImGui(key, value);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const Vector2& value)
{
	global_->AddItemDontTouchImGui(key, value);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const Vector3& value)
{
	global_->AddItemDontTouchImGui(key, value);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const bool& value)
{
	global_->AddItemDontTouchImGui(key, value);
}

void GlobalVariableUser::AddItemDontTouchImGui(const std::string& key, const std::string& value)
{
	global_->AddItemDontTouchImGui(key, value);
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
	global_->SaveAndSetVariableDontTouchImGui(key, value);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const float& value)
{
	global_->SaveAndSetVariableDontTouchImGui(key, value);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const Vector2& value)
{
	global_->SaveAndSetVariableDontTouchImGui(key, value);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const Vector3& value)
{
	global_->SaveAndSetVariableDontTouchImGui(key, value);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const bool& value)
{
	global_->SaveAndSetVariableDontTouchImGui(key, value);
}

void GlobalVariableUser::SaveDontTouchImGui(const std::string& key, const std::string& value)
{
	global_->SaveAndSetVariableDontTouchImGui(key, value);
}

bool GlobalVariableUser::IsTreeOpen(const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 1) {
		return global_->IsTreeOpen(chunkName_, groupName_, tree_[0], tree1, tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 2) {
		return global_->IsTreeOpen(chunkName_, groupName_, tree_[0], tree_[1], tree1, tree2, tree3, tree4);
	}
	else if (tree_.size() == 3) {
		return global_->IsTreeOpen(chunkName_, groupName_, tree_[0], tree_[1], tree_[2], tree1, tree2, tree3);
	}
	else if (tree_.size() == 4) {
		return global_->IsTreeOpen(chunkName_, groupName_, tree_[0], tree_[1], tree_[2], tree_[3], tree1, tree2);
	}
	else if (tree_.size() == 5) {
		return global_->IsTreeOpen(chunkName_, groupName_, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree1);
	}
	return global_->IsTreeOpen(chunkName_, groupName_, tree1, tree2, tree3, tree4, tree5, tree6);
}

bool GlobalVariableUser::IsTreeOpen(const int& treeNo)
{
	int no = treeNo + 1;
	if (int(tree_.size()) < no) {
		return false;
	}
	if (no == 1) {
		return global_->IsTreeOpen(chunkName_, groupName_, tree_[0]);
	}
	else if (no == 2) {
		return global_->IsTreeOpen(chunkName_, groupName_, tree_[0], tree_[1]);
	}
	else if (no == 3) {
		return global_->IsTreeOpen(chunkName_, groupName_, tree_[0], tree_[1], tree_[2]);
	}
	else if (no == 4) {
		return global_->IsTreeOpen(chunkName_, groupName_, tree_[0], tree_[1], tree_[2], tree_[3]);
	}
	else if (no == 5) {
		return global_->IsTreeOpen(chunkName_, groupName_, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4]);
	}
	else if (no == 6) {
		return global_->IsTreeOpen(chunkName_, groupName_, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree_[5]);
	}
	return false;
}
