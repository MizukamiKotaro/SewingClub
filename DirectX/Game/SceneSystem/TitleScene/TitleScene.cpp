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
	
	titleLogo_ = std::make_unique<Sprite>("titleLogo_1.png");

	startWord_ = std::make_unique<Sprite>("title_startUI.png");
	startWord_->pos_.y = 500.0f;
	startWord_->pos_.x = 700.0f;

	bg_ = std::make_unique<BackGround>();

	bgm_.LoadWave("Music/title.wav","TitleBGM",bgmVolume_);


	GlobalVariables* gVari = GlobalVariables::GetInstance();
	
	gVari->CreateGroup(groupName_);

	gVari->AddItem(groupName_, "タイトルロゴ座標", titleLogo_->pos_);
	gVari->AddItem(groupName_, "タイトルロゴサイズ", titleLogo_->size_);
	gVari->AddItem(groupName_,"ボタン座標", buttonA_->pos_);
	gVari->AddItem(groupName_, "ボタンサイズ", buttonA_->size_);
	gVari->AddItem(groupName_, "スタート文字座標", startWord_->pos_);
	gVari->AddItem(groupName_, "スタート文字サイズ", startWord_->size_);

	titleLogo_->pos_ = gVari->GetVector2Value(groupName_, "タイトルロゴ座標");
	titleLogo_->size_ = gVari->GetVector2Value(groupName_, "タイトルロゴサイズ");
	buttonA_->pos_ = gVari->GetVector2Value(groupName_, "ボタン座標");
	buttonA_->size_ = gVari->GetVector2Value(groupName_, "ボタンサイズ");
	startWord_->pos_ = gVari->GetVector2Value(groupName_, "スタート文字座標");
	startWord_->size_ = gVari->GetVector2Value(groupName_, "スタート文字サイズ");

}

void TitleScene::Initialize()
{
	AudioManager::GetInstance()->AllStop();
	
	GlobalVariables* gVari = GlobalVariables::GetInstance();

	titleLogo_->pos_ = gVari->GetVector2Value(groupName_, "タイトルロゴ座標");
	titleLogo_->size_ = gVari->GetVector2Value(groupName_, "タイトルロゴサイズ");
	buttonA_->pos_ = gVari->GetVector2Value(groupName_, "ボタン座標");
	buttonA_->size_ = gVari->GetVector2Value(groupName_, "ボタンサイズ");
	startWord_->pos_ = gVari->GetVector2Value(groupName_, "スタート文字座標");
	startWord_->size_ = gVari->GetVector2Value(groupName_, "スタート文字サイズ");
	buttonA_->Update();
	titleLogo_->Update();
	startWord_->Update();


	bgm_.Play(true);
}

void TitleScene::Update()
{
	GlobalVariables* gVari = GlobalVariables::GetInstance();
	titleLogo_->pos_ = gVari->GetVector2Value(groupName_, "タイトルロゴ座標");
	titleLogo_->size_ = gVari->GetVector2Value(groupName_, "タイトルロゴサイズ");
	buttonA_->pos_ = gVari->GetVector2Value(groupName_, "ボタン座標");
	buttonA_->size_ = gVari->GetVector2Value(groupName_, "ボタンサイズ");
	startWord_->pos_ = gVari->GetVector2Value(groupName_, "スタート文字座標");
	startWord_->size_ = gVari->GetVector2Value(groupName_, "スタート文字サイズ");


	bg_->Update(camera_.get());

	buttonA_->Update();
	titleLogo_->Update();
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
	titleLogo_->Draw();

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

