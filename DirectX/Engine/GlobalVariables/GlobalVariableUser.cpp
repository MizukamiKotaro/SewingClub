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

void GlobalVariableUser::AddItem(const std::string& key, int32_t value)
{
	globalVariable_->AddItem(chunkName_, groupName_, key, value);
}

void GlobalVariableUser::AddItem(const std::string& key, float value)
{
	globalVariable_->AddItem(chunkName_, groupName_, key, value);
}

void GlobalVariableUser::AddItem(const std::string& key, const Vector2& value)
{
	globalVariable_->AddItem(chunkName_, groupName_, key, value);
}

void GlobalVariableUser::AddItem(const std::string& key, const Vector3& value)
{
	globalVariable_->AddItem(chunkName_, groupName_, key, value);
}

void GlobalVariableUser::AddItem(const std::string& key, bool value)
{
	globalVariable_->AddItem(chunkName_, groupName_, key, value);
}

void GlobalVariableUser::AddItem(const std::string& key, const std::string& value)
{
	globalVariable_->AddItem(chunkName_, groupName_, key, value);
}

int32_t GlobalVariableUser::GetIntValue(const std::string& key) const
{
	return globalVariable_->GetIntValue(chunkName_, groupName_, key);
}

float GlobalVariableUser::GetFloatValue(const std::string& key) const
{
	return globalVariable_->GetFloatValue(chunkName_, groupName_, key);
}

Vector2 GlobalVariableUser::GetVector2Value(const std::string& key) const
{
	return globalVariable_->GetVector2Value(chunkName_, groupName_, key);
}

Vector3 GlobalVariableUser::GetVector3Value(const std::string& key) const
{
	return globalVariable_->GetVector3Value(chunkName_, groupName_, key);
}

bool GlobalVariableUser::GetBoolValue(const std::string& key) const
{
	return globalVariable_->GetBoolValue(chunkName_, groupName_, key);
}

std::string GlobalVariableUser::GetStringValue(const std::string& key) const
{
	return globalVariable_->GetStringValue(chunkName_, groupName_, key);
}