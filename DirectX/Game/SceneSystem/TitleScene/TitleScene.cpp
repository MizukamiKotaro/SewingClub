#include "TitleScene.h"
#include "Kyoko.h"
#include "ImGuiManager/ImGuiManager.h"
#include"Audio/AudioManager/AudioManager.h"
#include"GlobalVariables/GlobalVariables.h"
#include"RandomGenerator/RandomGenerator.h"

TitleScene::TitleScene()
{
	FirstInit();

	buttonA_ = std::make_unique<Sprite>("controler_UI_A1.png");
	buttonA_->pos_.y = 500.0f;
	buttonA_->size_ = { 123.0f,123.0f };

	for (int i = 0; i < 5; i++) {
		titleLogo_[i] = std::make_unique<Sprite>("titlelogo.png");
		titleLogo_[i]->SetTextureSize({ 128,300 });
		titleLogo_[i]->SetTextureTopLeft({ 128.0f * (float)i,0 });
		titleLogo_[i]->size_ = { 120,300 };

		logos_[i] = { (-128.0f * 2) + 128.0f * i,0 };

		titleLogo_[i]->pos_ = logos_[i] + logoPos_;
		titleLogo_[i]->Update();


		logoCenters_[i] = logos_[i] + logoPos_;
	}

	startWord_ = std::make_unique<Sprite>("title_startUI.png");
	startWord_->pos_.y = 500.0f;
	startWord_->pos_.x = 700.0f;

	bg_ = std::make_unique<BackGround>();

	bgm_.LoadMP3("Music/title.mp3", "TitleBGM", bgmVolume_);

	effeUIEnterW_ = std::make_unique<EffectUIEnterWater>("TitleBubbleUI");
	waterE_ = std::make_unique<WaterEffect>(camera_->transform_.translate_);
	
	whiteS_ = std::make_unique<Sprite>();

	optionUI_ = std::make_unique<OptionUI>();


	GlobalVariables* gVari = GlobalVariables::GetInstance();
	gVari->CreateGroup(groupName_);
	gVari->AddItem(groupName_, "タイトルロゴ座標(変更適応はシーン変更で)", logoPos_);
	gVari->AddItem(groupName_, "ボタン座標", buttonA_->pos_);
	gVari->AddItem(groupName_, "ボタンサイズ", buttonA_->size_);
	gVari->AddItem(groupName_, "スタート文字座標", startWord_->pos_);
	gVari->AddItem(groupName_, "スタート文字サイズ", startWord_->size_);

	gVari->AddItem(groupName_, "ロゴのランダム速度(min/max)", randVelo_);
	gVari->AddItem(groupName_, "ロゴが移動できる範囲", logoMoveArea_);
	
}

void TitleScene::Initialize()
{
	AudioManager::GetInstance()->AllStop();

	GlobalVariables* gVari = GlobalVariables::GetInstance();

	logoPos_ = gVari->GetVector2Value(groupName_, "タイトルロゴ座標(変更適応はシーン変更で)");
	buttonA_->pos_ = gVari->GetVector2Value(groupName_, "ボタン座標");
	buttonA_->size_ = gVari->GetVector2Value(groupName_, "ボタンサイズ");
	startWord_->pos_ = gVari->GetVector2Value(groupName_, "スタート文字座標");
	startWord_->size_ = gVari->GetVector2Value(groupName_, "スタート文字サイズ");
	randVelo_ = gVari->GetVector2Value(groupName_, "ロゴのランダム速度(min/max)");
	logoMoveArea_ = gVari->GetVector2Value(groupName_, "ロゴが移動できる範囲");
	


	buttonA_->Update();
	int i = 0;
	for (auto& logo : titleLogo_) {
		logo->pos_ = logos_[i] + logoPos_;
		logo->Update();


		//以下速度
		logoVelo_[i] = RandomGenerator::GetInstance()->RandVector2(-1, 1);
		logoVelo_[i] = logoVelo_[i].Normalize() * RandomGenerator::GetInstance()->RandFloat(randVelo_.x, randVelo_.y);

		logoCenters_[i] = logos_[i] + logoPos_;
		i++;
	}
	startWord_->Update();


	bgm_.Play(true);

	effeUIEnterW_->Initialize();
	effeUIEnterW_->IsEffectActive(true);

	whiteS_->Initialize();
	whiteS_->size_ = { 1280,720 };
	whiteS_->Update();

	optionUI_->Initialize();
	optionUI_->Update();

	//いい感じの演出になったので消します
	//waterE_->Initialize();
}

void TitleScene::Update()
{

	if (!isOptionActive_) {

		GlobalVariables* gVari = GlobalVariables::GetInstance();
		logoPos_ = gVari->GetVector2Value(groupName_, "タイトルロゴ座標(変更適応はシーン変更で)");
		buttonA_->pos_ = gVari->GetVector2Value(groupName_, "ボタン座標");
		buttonA_->size_ = gVari->GetVector2Value(groupName_, "ボタンサイズ");
		startWord_->pos_ = gVari->GetVector2Value(groupName_, "スタート文字座標");
		startWord_->size_ = gVari->GetVector2Value(groupName_, "スタート文字サイズ");
		randVelo_ = gVari->GetVector2Value(groupName_, "ロゴのランダム速度(min/max)");
		logoMoveArea_ = gVari->GetVector2Value(groupName_, "ロゴが移動できる範囲");



		LogoAnimation();

		bg_->Update(camera_.get());

		buttonA_->Update();

		startWord_->Update();

		SceneChange();

		effeUIEnterW_->Update();

		waterE_->Update(0.1f);

	}
	else {
		isOptionActive_ = optionUI_->Update();
	}
}

void TitleScene::Draw()
{
	WrightPostEffect();


	Kyoko::Engine::PreDraw();

	//ポストエフェ
	waterE_->Draw();
	
	for (auto& logo : titleLogo_) {
		logo->Draw();
	}
	
	startWord_->Draw();

	buttonA_->Draw();
	

	effeUIEnterW_->Draw();

	if (isOptionActive_) {
		optionUI_->Draw();
	}

	BlackDraw();

	Kyoko::Engine::PostDraw();
}

void TitleScene::WrightPostEffect()
{
	waterE_->PreDrawBackGround();

	bg_->Draw();

	waterE_->PostDrawBackGround();

	waterE_->PreDrawWaterArea();

	//
	//bg_->Draw();
	whiteS_->Draw();

	waterE_->PostDrawWaterArea();
}

void TitleScene::LogoAnimation()
{
	int i = 0;
	for (auto& logo : titleLogo_) {
		logo->pos_ += logoVelo_[i];

		//ロゴ移動範囲外で速度リセット
		Vector2 direction = logo->pos_;
		bool isAreaOut = false;
		if (logoMoveArea_.x + logoCenters_[i].x <= (direction.x)) {
			logo->pos_.x = logoCenters_[i].x + logoMoveArea_.x;
			isAreaOut = true;
		}
		else if (-logoMoveArea_.x + logoCenters_[i].x >= direction.x) {
			logo->pos_.x = logoCenters_[i].x - logoMoveArea_.x;
			isAreaOut = true;
		}
		if (logoMoveArea_.y + logoCenters_[i].y <= direction.y) {
			logo->pos_.y = logoCenters_[i].y + logoMoveArea_.y;
			isAreaOut = true;
		}
		else if (-logoMoveArea_.y + logoCenters_[i].y >= direction.y) {
			logo->pos_.y = logoCenters_[i].y - logoMoveArea_.y;
			isAreaOut = true;
		}

		if (isAreaOut) {
			logoVelo_[i] = RandomGenerator::GetInstance()->RandVector2(-1, 1);
			logoVelo_[i] = logoVelo_[i].Normalize() * RandomGenerator::GetInstance()->RandFloat(randVelo_.x, randVelo_.y);
			//logo->pos_ += logoVelo_[i];
		}

		logo->Update();
		i++;
	}

}

void TitleScene::SceneChange()
{
	//
	if (input_->PressedGamePadButton(Input::GamePadButton::A)&&!isOptionActive_) {
		// シーン切り替え
		stageNo_ = 0;
		ChangeScene(SELECT);
		bgm_.Stop();
	}

	if (input_->PressedGamePadButton(Input::GamePadButton::START) &&!isOptionActive_) {
		isOptionActive_ = true;
	}
}

