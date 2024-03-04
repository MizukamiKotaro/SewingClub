#include "Charactor.h"

void Charactor::CreateGlobalVariable(const std::string& groupName)
{
	globalVariable_ = std::make_unique<GlobalVariableUser>("Charactor", groupName);
	globalVariable_->CreateGroup();
}

void Charactor::SetGlobalVariable()
{
}

void Charactor::ApplyGlobalVariable()
{
}