#include "TitleScene.h"
#include "Kyoko.h"
#include "ImGuiManager/ImGuiManager.h"
#include"Audio/AudioManager/AudioManager.h"
#include"GlobalVariables/GlobalVariables.h"

TitleScene::TitleScene()
{
	FirstInit();

	buttonA_ = std::make_unique<Sprite>("controler_UI_A1.png");
	buttonA_->pos_.y = 500.0f;
	buttonA_->size_ = { 123.0f,123.0f };
	
	for (int i = 0; i < 5; i++) {
		titleLogo_[i] = std::make_unique<Sprite>("titlelogo.png");
		titleLogo_[i]->SetTextureSize({128,300});
		titleLogo_[i]->SetTextureTopLeft({128.0f*(float)i,0});
		titleLogo_[i]->size_ = {120,300};

		logos_[i] = {(-128.0f * 2) + 128.0f * i,0};

		titleLogo_[i]->pos_ = logos_[i] + logoPos_;
		titleLogo_[i]->Update();
	}

	startWord_ = std::make_unique<Sprite>("title_startUI.png");
	startWord_->pos_.y = 500.0f;
	startWord_->pos_.x = 700.0f;

	bg_ = std::make_unique<BackGround>();

	bgm_.LoadWave("Music/title.wav","TitleBGM",bgmVolume_);


	GlobalVariables* gVari = GlobalVariables::GetInstance();
	
	gVari->CreateGroup(groupName_);

	gVari->AddItem(groupName_, "タイトルロゴ座標", logoPos_);
	gVari->AddItem(groupName_,"ボタン座標", buttonA_->pos_);
	gVari->AddItem(groupName_, "ボタンサイズ", buttonA_->size_);
	gVari->AddItem(groupName_, "スタート文字座標", startWord_->pos_);
	gVari->AddItem(groupName_, "スタート文字サイズ", startWord_->size_);

	logoPos_ = gVari->GetVector2Value(groupName_, "タイトルロゴ座標");
	buttonA_->pos_ = gVari->GetVector2Value(groupName_, "ボタン座標");
	buttonA_->size_ = gVari->GetVector2Value(groupName_, "ボタンサイズ");
	startWord_->pos_ = gVari->GetVector2Value(groupName_, "スタート文字座標");
	startWord_->size_ = gVari->GetVector2Value(groupName_, "スタート文字サイズ");

}

void TitleScene::Initialize()
{
	AudioManager::GetInstance()->AllStop();
	
	GlobalVariables* gVari = GlobalVariables::GetInstance();

	logoPos_ = gVari->GetVector2Value(groupName_, "タイトルロゴ座標");
	buttonA_->pos_ = gVari->GetVector2Value(groupName_, "ボタン座標");
	buttonA_->size_ = gVari->GetVector2Value(groupName_, "ボタンサイズ");
	startWord_->pos_ = gVari->GetVector2Value(groupName_, "スタート文字座標");
	startWord_->size_ = gVari->GetVector2Value(groupName_, "スタート文字サイズ");
	buttonA_->Update();
	int i = 0;
	for (auto& logo : titleLogo_) {
		logo->pos_ = logos_[i] + logoPos_;
		logo->Update();

		i++;
	}
	startWord_->Update();


	bgm_.Play(true);
}

void TitleScene::Update()
{
	GlobalVariables* gVari = GlobalVariables::GetInstance();
	logoPos_ = gVari->GetVector2Value(groupName_, "タイトルロゴ座標");
	buttonA_->pos_ = gVari->GetVector2Value(groupName_, "ボタン座標");
	buttonA_->size_ = gVari->GetVector2Value(groupName_, "ボタンサイズ");
	startWord_->pos_ = gVari->GetVector2Value(groupName_, "スタート文字座標");
	startWord_->size_ = gVari->GetVector2Value(groupName_, "スタート文字サイズ");


	bg_->Update(camera_.get());

	buttonA_->Update();
	int i = 0;
	for (auto& logo : titleLogo_) {
		logo->pos_ = logos_[i] + logoPos_;
		logo->Update();

		i++;
	}
	startWord_->Update();

	SceneChange();
}

void TitleScene::Draw()
{
	WrightPostEffect();

	Kyoko::Engine::PreDraw();

	bg_->Draw();

	startWord_->Draw();

	buttonA_->Draw();
	for (auto& logo : titleLogo_) {
		logo->Draw();
	}
	BlackDraw();

	Kyoko::Engine::PostDraw();
}

void TitleScene::WrightPostEffect()
{
	
}

void TitleScene::SceneChange()
{
	//
	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		// シーン切り替え
		stageNo_ = 0;
		ChangeScene(SELECT);
		bgm_.Stop();
	}
}

