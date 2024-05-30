#include "InGameHUD.h"

InGameHUD::InGameHUD()
{
	
	//画像群読み込みこみ
	for (int i = 0; i < _CountTags; i++) {
		sp_[i] = std::make_unique<Sprite>(paths[i]);
	}

	gvu_= new GlobalVariableUser("InGame", "InGameHUD");
	for (int i = 0; i < _CountTags; i++) {
		gvu_->AddItem(sKeysP_[i],sp_[i]->pos_);
		gvu_->AddItem(sKeysS_[i], sp_[i]->size_);

	}
}

void InGameHUD::SetGlobalV()
{
	for (int i = 0; i < _CountTags; i++) {
		sp_[i]->pos_ = gvu_->GetVector2Value(sKeysP_[i]);
		sp_[i]->size_ = gvu_->GetVector2Value(sKeysS_[i]);

	}
}


InGameHUD::~InGameHUD()
{
}

void InGameHUD::Initialize()
{
	SetGlobalV();

	Update();
}

void InGameHUD::Update()
{
#ifdef _DEBUG
	SetGlobalV();
#endif // _DEBUG

	for (int i = 0; i < _CountTags; i++) {
		sp_[i]->Update();
	}
}

void InGameHUD::Draw()
{
	for (int i = 0; i < _CountTags; i++) {
		sp_[i]->Draw();
	}
}

