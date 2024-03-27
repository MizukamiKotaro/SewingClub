#include "Vector2.h"
#include "Vector3.h"
#include "GlobalVariableUser.h"
#include "GlobalVariables.h"

GlobalVariables* GlobalVariableUser::globalVariable_ = nullptr;

void GlobalVariableUser::StaticInitialize()
{
	globalVariable_ = GlobalVariables::GetInstance();
}

GlobalVariableUser::GlobalVariableUser(const std::string& chunkName, const std::string& groupName)
{
	chunkName_ = chunkName;
	groupName_ = groupName;
}

GlobalVariableUser::GlobalVariableUser(const std::string& name)
{
	chunkName_ = name;
	groupName_ = name;
}

void GlobalVariableUser::ResetGroupName(const std::string& groupName)
{
	groupName_ = groupName;
}

void GlobalVariableUser::CreateGroup()
{
	globalVariable_->CreateGroup(chunkName_, groupName_);
}

void GlobalVariableUser::AddItem(const std::string& key, int32_t value, const std::string& tree1, const std::string& tree2, const std::string& tree3)
{
	if (tree1 == "_") {
		globalVariable_->AddItem(chunkName_, groupName_, key, value);
	}
	else {
		globalVariable_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3);
	}
}

void GlobalVariableUser::AddItem(const std::string& key, float value, const std::string& tree1, const std::string& tree2, const std::string& tree3)
{
	if (tree1 == "_") {
		globalVariable_->AddItem(chunkName_, groupName_, key, value);
	}
	else {
		globalVariable_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3);
	}
}

void GlobalVariableUser::AddItem(const std::string& key, const Vector2& value, const std::string& tree1, const std::string& tree2, const std::string& tree3)
{
	if (tree1 == "_") {
		globalVariable_->AddItem(chunkName_, groupName_, key, value);
	}
	else {
		globalVariable_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3);
	}
}

void GlobalVariableUser::AddItem(const std::string& key, const Vector3& value, const std::string& tree1, const std::string& tree2, const std::string& tree3)
{
	if (tree1 == "_") {
		globalVariable_->AddItem(chunkName_, groupName_, key, value);
	}
	else {
		globalVariable_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3);
	}
}

void GlobalVariableUser::AddItem(const std::string& key, bool value, const std::string& tree1, const std::string& tree2, const std::string& tree3)
{
	if (tree1 == "_") {
		globalVariable_->AddItem(chunkName_, groupName_, key, value);
	}
	else {
		globalVariable_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3);
	}
}

void GlobalVariableUser::AddItem(const std::string& key, const std::string& value, const std::string& tree1, const std::string& tree2, const std::string& tree3)
{
	if (tree1 == "_") {
		globalVariable_->AddItem(chunkName_, groupName_, key, value);
	}
	else {
		globalVariable_->AddItem(chunkName_, groupName_, key, value, 0, tree1, tree2, tree3);
	}
}

int32_t GlobalVariableUser::GetIntValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3) const
{
	if (tree1 == "_") {
		return globalVariable_->GetIntValue(chunkName_, groupName_, key);
	}
	else {
		return globalVariable_->GetIntValue(chunkName_, groupName_, key, 0, tree1, tree2, tree3);
	}
}

float GlobalVariableUser::GetFloatValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3) const
{
	if (tree1 == "_") {
		return globalVariable_->GetFloatValue(chunkName_, groupName_, key);
	}
	else {
		return globalVariable_->GetFloatValue(chunkName_, groupName_, key, 0, tree1, tree2, tree3);
	}
}

Vector2 GlobalVariableUser::GetVector2Value(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3) const
{
	if (tree1 == "_") {
		return globalVariable_->GetVector2Value(chunkName_, groupName_, key);
	}
	else {
		return globalVariable_->GetVector2Value(chunkName_, groupName_, key, 0, tree1, tree2, tree3);
	}
}

Vector3 GlobalVariableUser::GetVector3Value(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3) const
{
	if (tree1 == "_") {
		return globalVariable_->GetVector3Value(chunkName_, groupName_, key);
	}
	else {
		return globalVariable_->GetVector3Value(chunkName_, groupName_, key, 0, tree1, tree2, tree3);
	}
}

bool GlobalVariableUser::GetBoolValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3) const
{
	if (tree1 == "_") {
		return globalVariable_->GetBoolValue(chunkName_, groupName_, key);
	}
	else {
		return globalVariable_->GetBoolValue(chunkName_, groupName_, key, 0, tree1, tree2, tree3);
	}
}

std::string GlobalVariableUser::GetStringValue(const std::string& key, const std::string& tree1, const std::string& tree2, const std::string& tree3) const
{
	if (tree1 == "_") {
		return globalVariable_->GetStringValue(chunkName_, groupName_, key);
	}
	else {
		return globalVariable_->GetStringValue(chunkName_, groupName_, key, 0, tree1, tree2, tree3);
	}
}