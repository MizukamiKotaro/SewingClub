#include "StageEditor.h"
#include "SceneSystem/IScene/IScene.h"

StageEditor::StageEditor(const std::string& tree1)
{
	tree1_ = tree1;
	global_ = std::make_unique<GlobalVariableUser>("StageEditor", "Stage" + std::to_string(IScene::stageNo_));
}

void StageEditor::Initialize()
{
	global_->ResetGroupName("Stage" + std::to_string(IScene::stageNo_));
}

void StageEditor::AddItem(const std::string& key, int32_t value, const std::string& tree2, const std::string& tree3)
{
	global_->AddItem(key, value, tree1_, tree2, tree3);
}

void StageEditor::AddItem(const std::string& key, float value, const std::string& tree2, const std::string& tree3)
{
	global_->AddItem(key, value, tree1_, tree2, tree3);
}

void StageEditor::AddItem(const std::string& key, const Vector2& value, const std::string& tree2, const std::string& tree3)
{
	global_->AddItem(key, value, tree1_, tree2, tree3);
}

void StageEditor::AddItem(const std::string& key, const Vector3& value, const std::string& tree2, const std::string& tree3)
{
	global_->AddItem(key, value, tree1_, tree2, tree3);
}

void StageEditor::AddItem(const std::string& key, bool value, const std::string& tree2, const std::string& tree3)
{
	global_->AddItem(key, value, tree1_, tree2, tree3);
}

void StageEditor::AddItem(const std::string& key, const std::string& value, const std::string& tree2, const std::string& tree3)
{
	global_->AddItem(key, value, tree1_, tree2, tree3);
}

int32_t StageEditor::GetIntValue(const std::string& key, const std::string& tree2, const std::string& tree3) const
{
	return global_->GetIntValue(key, tree1_, tree2, tree3);
}

float StageEditor::GetFloatValue(const std::string& key, const std::string& tree2, const std::string& tree3) const
{
	return global_->GetFloatValue(key, tree1_, tree2, tree3);
}

Vector2 StageEditor::GetVector2Value(const std::string& key, const std::string& tree2, const std::string& tree3) const
{
	return global_->GetVector2Value(key, tree1_, tree2, tree3);
}

Vector3 StageEditor::GetVector3Value(const std::string& key, const std::string& tree2, const std::string& tree3) const
{
	return global_->GetVector3Value(key, tree1_, tree2, tree3);
}

bool StageEditor::GetBoolValue(const std::string& key, const std::string& tree2, const std::string& tree3) const
{
	return global_->GetBoolValue(key, tree1_, tree2, tree3);
}

std::string StageEditor::GetStringValue(const std::string& key, const std::string& tree2, const std::string& tree3) const
{
	return global_->GetStringValue(key, tree1_, tree2, tree3);
}

bool StageEditor::IsTreeOpen(const std::string& tree2, const std::string& tree3)
{
	return global_->IsTreeOpen(tree1_, tree2, tree3);
}

const bool StageEditor::IsChangedStage()
{
	if (preStageNo_ != IScene::stageNo_) {
		preStageNo_ = IScene::stageNo_;
		return true;
	}
	return false;
}

