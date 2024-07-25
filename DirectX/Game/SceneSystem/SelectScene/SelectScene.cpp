#include "SelectScene.h"
#include "Kyoko.h"
#include "ImGuiManager/ImGuiManager.h"
#include"calc.h"
#include "Texture.h"
#include"DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"

#include<numbers>
SelectScene::SelectScene()
{
	FirstInit();

	input_ = Input::GetInstance();

	optionUI_ = std::make_unique<OptionUI>(OptionUI::kSelect);

	bg_ = std::make_unique<BackGround>();
	bg_->Update(camera_.get());

	effeBSleep_ = std::make_unique<EffectBabySleep >();

	bgm_.LoadMP3("Music/stageSelect.mp3", "SelectBGM", bgmVolume_);
	seOpenOption_.LoadMP3("SE/Scene/autgame_poseOpen.mp3");
	seMove_.LoadMP3("SE/Scene/outgame_selectNow.mp3");
	seSelect_.LoadMP3("SE/Scene/autgame_decision.mp3");

	//画像初期化
	for (int i = 0; i < _countTags; i++) {
		sp_[i] = std::make_unique<Sprite>(paths[i]);
	}

	sp_[LArrow]->SetIsFlipX(true);

	//画像の数字のみサイズ変更
	sp_[Num1]->SetTextureSize({ 125, 125 });
	sp_[Num10]->SetTextureSize({ 125, 125 });

	//マップ画像初期化
	for (int i = 0; i < _countStages; i++) {
		mapSprite_[i] = std::make_unique<Sprite>(mapPaths_[i]);
	}

	sceneTransition_ = std::make_unique<SceneTransitionEffect>("select");

	gvu_ = new GlobalVariableUser("Scene", "Selects");
	for (int i = 0; i < _countTags; i++) {
		if (i == LArrow) {
			gvu_->AddItem(spKeysP[i], arrowPos_[Left]);

		}
		else if (i == RArrow) {
			gvu_->AddItem(spKeysP[i], arrowPos_[Right]);
		}
		else {
			gvu_->AddItem(spKeysP[i], sp_[i]->pos_);
		}
		gvu_->AddItem(spKeysS[i], sp_[i]->size_);
	}
	for (int i = 0; i < _countStages; i++) {
		gvu_->AddItem(mapKeysP[i], mapSprite_[i]->pos_);
		gvu_->AddItem(mapKeysS[i], mapSprite_[i]->size_);
	}

	gvu_->AddItem(anoKeys[NumPos], numberCPos_);
	gvu_->AddItem(anoKeys[NumsDistance], numDistance_);
	gvu_->AddItem(anoKeys[SwingSecond], swingSecond_);
	gvu_->AddItem(anoKeys[SwingNum], swingNum_);
	gvu_->AddItem(anoKeys[BSwingSecond], bSwingSecond_);
	gvu_->AddItem(anoKeys[BSwingNum], bSwingNum_);
	gvu_->AddItem(anoKeys[MapPos], mapPos_);
	gvu_->AddItem(anoKeys[MapSize], mapSize_);
	gvu_->AddItem(anoKeys[AnimeCount], animeCount_);

	gvu_->AddItem(anoKeys[cS1], cSwingSeconds_[Spawn1]);
	gvu_->AddItem(anoKeys[cS2], cSwingSeconds_[Spawn2]);
	gvu_->AddItem(anoKeys[cS3], cSwingSeconds_[Spawn3]);
	gvu_->AddItem(anoKeys[cS4], cSwingSeconds_[None]);


	surface_ = std::make_unique<WaterSurface>("セレクトの雲");
	surface2_ = std::make_unique<WaterSurface>("セレクトのステージイメージ");
}

void SelectScene::SetGlobalV()
{

	for (int i = 0; i < _countTags; i++) {
		if (i == LArrow) {
			arrowPos_[Left] = gvu_->GetVector2Value(spKeysP[i]);
		}
		else if (i == RArrow) {
			arrowPos_[Right] = gvu_->GetVector2Value(spKeysP[i]);
		}
		else if (i == SmallClound1) {
			cPos_[Spawn1] = gvu_->GetVector2Value(spKeysP[i]);
		}
		else if (i == SmallClound2) {
			cPos_[Spawn2] = gvu_->GetVector2Value(spKeysP[i]);
		}
		else if (i == SmallClound3) {
			cPos_[Spawn3] = gvu_->GetVector2Value(spKeysP[i]);
		}
		else if (i == Clound) {
			cPos_[None] = gvu_->GetVector2Value(spKeysP[i]);
		}
		else {
			sp_[i]->pos_ = gvu_->GetVector2Value(spKeysP[i]);
		}
		sp_[i]->size_ = gvu_->GetVector2Value(spKeysS[i]);
	}
	for (int i = 0; i < _countStages; i++) {
		mapSprite_[i]->pos_ = gvu_->GetVector2Value(mapKeysP[i]);
		mapSprite_[i]->size_ = gvu_->GetVector2Value(mapKeysS[i]);
	}
	numberCPos_ = gvu_->GetVector2Value(anoKeys[NumPos]);
	numDistance_ = gvu_->GetFloatValue(anoKeys[NumsDistance]);
	swingSecond_ = gvu_->GetFloatValue(anoKeys[SwingSecond]);
	swingNum_ = gvu_->GetFloatValue(anoKeys[SwingNum]);
	bSwingSecond_ = gvu_->GetFloatValue(anoKeys[BSwingSecond]);
	bSwingNum_ = gvu_->GetFloatValue(anoKeys[BSwingNum]);

	mapPos_ = gvu_->GetVector2Value(anoKeys[MapPos]);
	mapSize_ = gvu_->GetVector2Value(anoKeys[MapSize]);
	maxAnimeCount_ = gvu_->GetIntValue(anoKeys[AnimeCount]);
	cSwingSeconds_[Spawn1] = gvu_->GetFloatValue(anoKeys[cS1]);
	cSwingSeconds_[Spawn2] = gvu_->GetFloatValue(anoKeys[cS2]);
	cSwingSeconds_[Spawn3] = gvu_->GetFloatValue(anoKeys[cS3]);
	cSwingSeconds_[None] = gvu_->GetFloatValue(anoKeys[cS4]);

	for (int i = 0; i < _countStages; i++) {
		mapSprite_[i]->pos_ = mapPos_;
		mapSprite_[i]->size_ = mapSize_;
	}
}

SelectScene::~SelectScene() {}

void SelectScene::Initialize()
{
	//カメラ初期化
	camera_->Initialize();

	//option初期化
	optionUI_->Initialize();

	bg_->Initialize();

	effeBSleep_->Initialize();

	pickedNum_ = stageNo_;

	bgm_.Play(true);

	SetGlobalV();



	ArrowUpdate();

	NumberUpdate();

	CloudUpdate();

	UpdateSprite();

	sceneTransition_->Initialize();
	isChangeScene_ = false;
}

void SelectScene::Update()
{
	camera_->Update();

	float deltaTime = frameInfo_->GetDeltaTime();
	if (sceneTransition_->PreSceneTransition(deltaTime)) {

#ifdef _DEBUG
		SetGlobalV();
		if (!isChangeScene_) {
			sceneTransition_->Debug();
		}
#endif // _DEBUG

		//オプション有効時の処理
		if (isOptionActive_) {
			ans_ = optionUI_->Update();
			if (ans_.audioOption) {
				bgm_.Update();
			}
		}
		else {
			//オプション非有効の時

			InputUpdate();

			ArrowUpdate();

			NumberUpdate();

			CloudUpdate();

			surface_->Update(deltaTime);
			surface2_->Update(deltaTime);

			effeBSleep_->Update(1.0f);

			bCount_ += bSwingSecond_ / 60.0f;
			bCount_ = std::fmod(bCount_, 2.0f * (float)std::numbers::pi);
			bAnimeP_.y = +std::sin(bCount_) * bSwingNum_;
			sp_[Baby]->pos_ += bAnimeP_;
		}


		SceneChange();

		UpdateSprite();

	}
}

void SelectScene::Draw()
{
	sceneTransition_->DrawPE();

	surface_->PreDraw();
	for (int i = 0; i < _countTags; i++) {
		if (i == Clound) {
			sp_[i]->Draw();
		}
		else if (i == SmallClound1) {
			if (isDraw_[Spawn1]) {
				sp_[i]->Draw();
			}
		}
		else if (i == SmallClound2) {
			if (isDraw_[Spawn2]) {
				sp_[i]->Draw();
			}
		}
		else if (i == SmallClound3) {
			if (isDraw_[Spawn3]) {
				sp_[i]->Draw();
			}
		}
	}
	surface_->PostDraw();
	surface2_->PreDraw();
	mapSprite_[pickedNum_]->Draw();
	surface2_->PostDraw();
	//必須
	Kyoko::Engine::PreDraw();

	bg_->Draw();

	//描画
	for (int i = 0; i < _countTags; i++) {
		if (i == Clound) {
			surface_->Draw();
		}
		else if (i == (int)Num10 && pickedNum_ < 9) {

		}
		else if (i == LArrow && pickedNum_ == 0) {
		}
		else if (i == RArrow && pickedNum_ == maxStageNum_ - 1) {
		}
		else if (i == SmallClound1) {
			
		}
		else if (i == SmallClound2) {
			
		}
		else if (i == SmallClound3) {
			
		}
		else {
			sp_[i]->Draw();
		}
	}
	surface2_->Draw();

	effeBSleep_->Draw();

	if (isOptionActive_) {
		optionUI_->Draw();
	}

	//シーン転換時のフェードインアウト
	BlackDraw();
	sceneTransition_->Draw();
	//必須
	Kyoko::Engine::PostDraw();
}



void SelectScene::SceneChange()
{

	float deltaTime = frameInfo_->GetDeltaTime();
	//シーン開始時の遷移animationが終了してから反応可能
	if (isChangeScene_ == true) {
		//以下Dissolve更新と処理
		if (sceneTransition_->PostSceneTransition(deltaTime)) {
			stageNo_ = pickedNum_;
			ChangeScene(STAGE);
		}
	}
	else {
		//シーン変更演出していないときの処理

#ifdef _DEBUG
		if (input_->PressedKey(DIK_SPACE)) {
			// シーン切り替え
			isChangeScene_ = true;
			bgm_.Stop();
			seSelect_.Play();
		}
#endif // _DEBUG

		//optionのアンサーによる処理
		if (isOptionActive_) {
			//optionから抜ける
			if (ans_.backOption) {
				isOptionActive_ = false;
			}
			//タイトルに戻る
			else if (ans_.backtitle) {
				// シーン切り替え
				ChangeScene(TITLE);
				bgm_.Stop();
			}
		}
		else {
			//ステージを選択する処理
			if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
				// シーン切り替え
				isChangeScene_ = true;
				bgm_.Stop();
				seSelect_.Play();
			}//オプション開く処理
			else if (input_->PressedGamePadButton(Input::GamePadButton::START)) {
				isOptionActive_ = true;
				seOpenOption_.Play();
			}
		}

	}

}

void SelectScene::InputUpdate()
{
	Vector2 move = input_->GetGamePadLStick();

	if (isInputActive_) {
		//右入力
		if (move.x > inputDeadline_) {
			isInputActive_ = false;
			seMove_.Play();
			//最後のステージじゃないなら処理
			if (pickedNum_ != maxStageNum_ - 1) {
				pickedNum_++;
			}

		}//左入力
		else if (move.x < -inputDeadline_) {
			isInputActive_ = false;
			seMove_.Play();
			//最後のステージじゃないなら処理
			if (pickedNum_ != 0) {
				pickedNum_--;
			}
		}
	}
	else {
		if (move.x < inputDeadline_ && move.x > -inputDeadline_) {
			isInputActive_ = true;
		}
	}
}

void SelectScene::ArrowUpdate()
{

	for (int i = 0; i < _countLR; i++) {

		if (i == (int)Left) {
			//動きの処理
			count_[i] -= swingSecond_ / 60.0f;
			count_[i] = std::fmod(count_[i], -2.0f * (float)std::numbers::pi);
			animeMove_[i].x = +std::sin(count_[i]) * swingNum_;
			sp_[LArrow]->pos_ = arrowPos_[i] + animeMove_[i];
		}
		else {
			//動きの処理
			count_[i] += swingSecond_ / 60.0f;
			count_[i] = std::fmod(count_[i], 2.0f * (float)std::numbers::pi);
			animeMove_[i].x = +std::sin(count_[i]) * swingNum_;
			sp_[RArrow]->pos_ = arrowPos_[i] + animeMove_[i];
		}
	}




}

void SelectScene::NumberUpdate()
{

	//配置の処理
	//10の位以下で1の位の値が中央に
	if (pickedNum_ + 1 < 10) {
		sp_[Num1]->pos_ = numberCPos_;
		//番号管理
		sp_[Num1]->SetTextureTopLeft({ 125.0f * (pickedNum_ + 1),0 });
	}
	else {
		//10の位がある場合距離を置いて表示
		Vector2 newPos = numberCPos_;
		newPos.x -= numDistance_ / 2;
		sp_[Num1]->pos_ = newPos;
		newPos.x += numDistance_;
		sp_[Num10]->pos_ = newPos;

		//番号管理
		int num1 = (pickedNum_ + 1) % 10;
		sp_[Num1]->SetTextureTopLeft({ 125.0f * (num1),0 });
		int num10 = pickedNum_ - num1;
		num10 /= 10;
		sp_[Num10]->SetTextureTopLeft({ 125.0f * (num10 + 1),0 });

	}

}

void SelectScene::CloudUpdate()
{

	if (!isStateChange_) {

		float t = (float)animeCount_ / (float)maxAnimeCount_;
		if (state_ == FadeOut) {
			alpha_[Spawn1] = Calc::Lerp(1, 0, t);
			alpha_[Spawn2] = Calc::Lerp(1, 0, t);
			alpha_[Spawn3] = Calc::Lerp(1, 0, t);
		}
		else if (state_ != None) {

			alpha_[state_] = Calc::Lerp(0, 1, t);

		}

		if (animeCount_++ >= maxAnimeCount_) {
			animeCount_ = 0;
			isStateChange_ = true;
			if (state_ == FadeOut) {
				alpha_[Spawn1] = 0;
				alpha_[Spawn2] = 0;
				alpha_[Spawn3] = 0;

				isDraw_[Spawn1] = false;
				isDraw_[Spawn2] = false;
				isDraw_[Spawn3] = false;

			}
			else if (state_ != None) {

				alpha_[state_] = 1;

			}
		}


	}
	else {
		isStateChange_ = false;

		if (state_ == None) {
			state_ = Spawn1;
		}
		else if (state_ == Spawn1) {
			state_ = Spawn2;
		}
		else if (state_ == Spawn2) {
			state_ = Spawn3;
		}
		else if (state_ == Spawn3) {
			state_ = FadeOut;
		}
		else if (state_ == FadeOut) {
			state_ = None;
		}

		switch (state_)
		{
		case SelectScene::None:
			isDraw_[Spawn1] = false;
			isDraw_[Spawn2] = false;
			isDraw_[Spawn3] = false;

			break;

		case SelectScene::Spawn1:
			isDraw_[Spawn1] = true;
			//alpha_[Spawn1] = 0;
			break;
		case SelectScene::Spawn2:
			isDraw_[Spawn2] = true;
			//alpha_[Spawn2] = 0;
			break;
		case SelectScene::Spawn3:
			isDraw_[Spawn3] = true;
			//alpha_[Spawn3] = 0;
			break;
		case SelectScene::FadeOut:
			break;
		case SelectScene::_countState:
			break;
		default:
			break;
		}
	}

	//動きの処理
	cCount_[Spawn1] += cSwingSeconds_[Spawn1] / 60.0f;
	cCount_[Spawn1] = std::fmod(cCount_[Spawn1], 2.0f * (float)std::numbers::pi);
	animePos_[Spawn1].y = +std::sin(cCount_[Spawn1]) * swingNum_;

	cCount_[Spawn2] += cSwingSeconds_[Spawn2] / 60.0f;
	cCount_[Spawn2] = std::fmod(cCount_[Spawn2], 2.0f * (float)std::numbers::pi);
	animePos_[Spawn2].y = +std::sin(cCount_[Spawn2]) * swingNum_;

	cCount_[Spawn3] += cSwingSeconds_[Spawn3] / 60.0f;
	cCount_[Spawn3] = std::fmod(cCount_[Spawn3], 2.0f * (float)std::numbers::pi);
	animePos_[Spawn3].y = +std::sin(cCount_[Spawn3]) * swingNum_;

	cCount_[None] += cSwingSeconds_[None] / 60.0f;
	cCount_[None] = std::fmod(cCount_[None], 2.0f * (float)std::numbers::pi);
	animePos_[None].y = +std::sin(cCount_[None]) * swingNum_;

}

void SelectScene::UpdateSprite()
{
	bg_->Update(camera_.get());
	//更新
	for (int i = 0; i < _countTags; i++) {

		if (i == SmallClound1) {
			if (isDraw_[Spawn1]) {
				sp_[i]->SetColor({ 1,1,1,alpha_[Spawn1] });
				sp_[i]->pos_ = cPos_[Spawn1] + animePos_[Spawn1];

			}
		}
		else if (i == SmallClound2) {
			if (isDraw_[Spawn2]) {
				sp_[i]->SetColor({ 1,1,1,alpha_[Spawn2] });
				sp_[i]->pos_ = cPos_[Spawn2] + animePos_[Spawn2];
			}
		}
		else if (i == SmallClound3) {
			if (isDraw_[Spawn3]) {
				sp_[i]->SetColor({ 1,1,1,alpha_[Spawn3] });
				sp_[i]->pos_ = cPos_[Spawn3] + animePos_[Spawn3];
			}
		}
		else if (i == Clound) {
			sp_[i]->pos_ = cPos_[None] + animePos_[None];
		}

		sp_[i]->Update();
	}
	mapSprite_[pickedNum_]->Update();
}
