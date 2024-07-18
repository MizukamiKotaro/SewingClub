#include "TitleScene.h"
#include "Kyoko.h"
#include"Audio/AudioManager/AudioManager.h"
#include"RandomGenerator/RandomGenerator.h"
#include "Texture.h"
#include"DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"

TitleScene::TitleScene()
{
	FirstInit();

	buttonA_ = std::make_unique<Sprite>("controler_UI_A1.png");
	buttonA_->pos_.y = 500.0f;
	buttonA_->size_ = { 123.0f,123.0f };

	select_ = std::make_unique<Sprite>("controler_UI_=-.png");
	text_Option_ = std::make_unique<Sprite>("HUD/option_HUD.png");

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

	startWord_ = std::make_unique<Sprite>("HUD/title_satrt_HUD.png");
	startWord_->pos_.y = 500.0f;
	startWord_->pos_.x = 700.0f;

	bg_ = std::make_unique<BackGround>();

	bgm_.LoadMP3("Music/title.mp3", "TitleBGM", bgmVolume_);
	seOpenOption_.LoadMP3("SE/Scene/autgame_poseOpen.mp3");
	seSelect_.LoadMP3("SE/Scene/autgame_decision.mp3");

	effeUIEnterW_ = std::make_unique<EffectUIEnterWater>("TitleBubbleUI");
	waterE_ = std::make_unique<WaterEffect>(camera_->transform_.translate_);

	whiteS_ = std::make_unique<Sprite>();

	optionUI_ = std::make_unique<OptionUI>(OptionUI::kTitle);

	sceneTransition_ = std::make_unique<SceneTransitionEffect>("Title");

	gVari = std::make_unique<GlobalVariableUser>("GlobalVariables", groupName_);
	gVari->AddItem("タイトルロゴ座標(変更適応はシーン変更で)", logoPos_);
	gVari->AddItem("ボタン座標", buttonA_->pos_);
	gVari->AddItem("ボタンサイズ", buttonA_->size_);
	gVari->AddItem("スタート文字座標", startWord_->pos_);
	gVari->AddItem("スタート文字サイズ", startWord_->size_);

	gVari->AddItem("ロゴのランダム速度(min/max)", randVelo_);
	gVari->AddItem("ロゴが移動できる範囲", logoMoveArea_);
	gVari->AddItem("「＝」座標", select_->pos_);
	gVari->AddItem("「＝」サイズ", select_->size_);
	gVari->AddItem("「option」座標", text_Option_->pos_);
	gVari->AddItem("「option」サイズ", text_Option_->size_);


	SetGlovalV();


}

void TitleScene::SetGlovalV()
{

	logoPos_ = gVari->GetVector2Value("タイトルロゴ座標(変更適応はシーン変更で)");
	buttonA_->pos_ = gVari->GetVector2Value("ボタン座標");
	buttonA_->size_ = gVari->GetVector2Value("ボタンサイズ");
	startWord_->pos_ = gVari->GetVector2Value("スタート文字座標");
	startWord_->size_ = gVari->GetVector2Value("スタート文字サイズ");
	randVelo_ = gVari->GetVector2Value("ロゴのランダム速度(min/max)");
	logoMoveArea_ = gVari->GetVector2Value("ロゴが移動できる範囲");
	select_->pos_ = gVari->GetVector2Value("「＝」座標");
	select_->size_ = gVari->GetVector2Value("「＝」サイズ");
	text_Option_->pos_ = gVari->GetVector2Value("「option」座標");
	text_Option_->size_ = gVari->GetVector2Value("「option」サイズ");



}

void TitleScene::Initialize()
{
	AudioManager::GetInstance()->AllStop();

	SetGlovalV();

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

	select_->Update();
	text_Option_->Update();

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

	sceneTransition_->Initialize();
	isChangeScene_ = false;

}

void TitleScene::Update()
{

	ans_ = UpdateAnswer();

	float deltaTime = frameInfo_->GetDeltaTime();
	if (sceneTransition_->PreSceneTransition(deltaTime)) {

		if (!isOptionActive_) {
#ifdef _DEBUG
			SetGlovalV();
#endif // _DEBUG
			LogoAnimation();

			bg_->Update(camera_.get());

			buttonA_->Update();

			startWord_->Update();

			select_->Update();
			text_Option_->Update();

			effeUIEnterW_->Update();

			waterE_->Update(0.1f);

		}
		else {
			ans_ = optionUI_->Update();

			if (ans_.audioOption) {
				bgm_.Update();
			}
		}


		SceneChange();

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

	select_->Draw();
	text_Option_->Draw();

	effeUIEnterW_->Draw();

	if (isOptionActive_) {
		optionUI_->Draw();
	}

	BlackDraw();
	sceneTransition_->Draw();
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

	sceneTransition_->DrawPE();
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

	float deltaTime = frameInfo_->GetDeltaTime();
	//シーン開始時の遷移animationが終了してから反応可能
	if (isChangeScene_ == true) {
		//以下Dissolve更新と処理
		if (sceneTransition_->PostSceneTransition(deltaTime)) {
			stageNo_ = 0;
			ChangeScene(SELECT);
		}
	}
	else {
#ifdef _DEBUG
		//スペースで次シーンへ
		if (input_->PressedKey(DIK_SPACE)) {
			// シーン切り替え
			isChangeScene_ = true;
			bgm_.Stop();
			seSelect_.Play();
		}
#endif // _DEBUG


		if (!isOptionActive_ && input_->PressedGamePadButton(Input::GamePadButton::A) && !isOptionActive_) {
			// シーン切り替え
			isChangeScene_ = true;
			bgm_.Stop();
			seSelect_.Play();
		}
		else if (!isOptionActive_ && input_->PressedGamePadButton(Input::GamePadButton::START) && !isOptionActive_) {
			seOpenOption_.Play();
			isOptionActive_ = true;
			seSelect_.Play();
		}
		else if (isOptionActive_ && ans_.backOption) {
			//処理
			isOptionActive_ = false;
		}
		else if (isOptionActive_ && ans_.leaveGame) {
			//処理
			isBreak_ = true;
		}

	}
}

