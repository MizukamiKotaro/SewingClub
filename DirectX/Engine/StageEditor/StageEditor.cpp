#include "StageEditor.h"
#include "SceneSystem/IScene/IScene.h"

StageEditor::StageEditor(const std::string& tree1, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5)
{
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
	else {
		tree_.resize(1);
		tree_[0] = tree1;
	}

	global_ = std::make_unique<GlobalVariableUser>("StageEditor", "Stage" + std::to_string(IScene::stageNo_));
}

void StageEditor::Initialize()
{
	global_->ResetGroupName("Stage" + std::to_string(IScene::stageNo_));
}

void StageEditor::AddItem(const std::string& key, const int32_t& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 1) {
		global_->AddItem(key, value, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
}

void StageEditor::AddItem(const std::string& key, const float& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 1) {
		global_->AddItem(key, value, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
}

void StageEditor::AddItem(const std::string& key, const Vector2& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 1) {
		global_->AddItem(key, value, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
}

void StageEditor::AddItem(const std::string& key, const Vector3& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 1) {
		global_->AddItem(key, value, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
}

void StageEditor::AddItem(const std::string& key, const bool& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 1) {
		global_->AddItem(key, value, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
}

void StageEditor::AddItem(const std::string& key, const std::string& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 1) {
		global_->AddItem(key, value, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		global_->AddItem(key, value, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
}

const int32_t& StageEditor::GetIntValue(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetIntValue(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		return global_->GetIntValue(key, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		return global_->GetIntValue(key, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		return global_->GetIntValue(key, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		return global_->GetIntValue(key, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
	return global_->GetIntValue(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
}

const float& StageEditor::GetFloatValue(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetFloatValue(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		return global_->GetFloatValue(key, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		return global_->GetFloatValue(key, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		return global_->GetFloatValue(key, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		return global_->GetFloatValue(key, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
	return global_->GetFloatValue(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
}

const Vector2& StageEditor::GetVector2Value(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetVector2Value(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		return global_->GetVector2Value(key, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		return global_->GetVector2Value(key, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		return global_->GetVector2Value(key, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		return global_->GetVector2Value(key, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
	return global_->GetVector2Value(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
}

const Vector3& StageEditor::GetVector3Value(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetVector3Value(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		return global_->GetVector3Value(key, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		return global_->GetVector3Value(key, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		return global_->GetVector3Value(key, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		return global_->GetVector3Value(key, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
	return global_->GetVector3Value(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
}

const bool& StageEditor::GetBoolValue(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetBoolValue(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		return global_->GetBoolValue(key, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		return global_->GetBoolValue(key, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		return global_->GetBoolValue(key, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		return global_->GetBoolValue(key, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
	return global_->GetBoolValue(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
}

const std::string& StageEditor::GetStringValue(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	if (tree_.size() == 1) {
		return global_->GetStringValue(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		return global_->GetStringValue(key, tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		return global_->GetStringValue(key, tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		return global_->GetStringValue(key, tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		return global_->GetStringValue(key, tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
	return global_->GetStringValue(key, tree_[0], tree2, tree3, tree4, tree5, tree6);
}

bool StageEditor::IsTreeOpen(const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	if (tree_.size() == 1) {
		return global_->IsTreeOpen(tree_[0], tree2, tree3, tree4, tree5, tree6);
	}
	else if (tree_.size() == 2) {
		return global_->IsTreeOpen(tree_[0], tree_[1], tree2, tree3, tree4, tree5);
	}
	else if (tree_.size() == 3) {
		return global_->IsTreeOpen(tree_[0], tree_[1], tree_[2], tree2, tree3, tree4);
	}
	else if (tree_.size() == 4) {
		return global_->IsTreeOpen(tree_[0], tree_[1], tree_[2], tree_[3], tree2, tree3);
	}
	else if (tree_.size() == 5) {
		return global_->IsTreeOpen(tree_[0], tree_[1], tree_[2], tree_[3], tree_[4], tree2);
	}
	return global_->IsTreeOpen(tree_[0], tree2, tree3, tree4, tree5, tree6);
}

bool StageEditor::IsTreeOpen(const int& treeNo)
{
	int no = treeNo + 1;
	if (int(tree_.size()) < no) {
		return false;
	}
	if (no == 1) {
		return global_->IsTreeOpen(tree_[0]);
	}
	else if (no == 2) {
		return global_->IsTreeOpen(tree_[0], tree_[1]);
	}
	else if (no == 3) {
		return global_->IsTreeOpen(tree_[0], tree_[1], tree_[2]);
	}
	else if (no == 4) {
		return global_->IsTreeOpen(tree_[0], tree_[1], tree_[2], tree_[3]);
	}
	else if (no == 5) {
		return global_->IsTreeOpen(tree_[0], tree_[1], tree_[2], tree_[3], tree_[4]);
	}
	return false;
}

const bool StageEditor::IsChangedStage()
{
	if (preStageNo_ != IScene::stageNo_) {
		preStageNo_ = IScene::stageNo_;
		return true;
	}
	return false;
}

