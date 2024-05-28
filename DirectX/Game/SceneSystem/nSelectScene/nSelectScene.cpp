#include "nSelectScene.h"
#include "Kyoko.h"
#include "ImGuiManager/ImGuiManager.h"
#include<numbers>
nSelectScene::nSelectScene()
{
	FirstInit();

	input_ = Input::GetInstance();

	optionUI_ = std::make_unique<OptionUI>(OptionUI::kSelect);

	bg_ = std::make_unique<BackGround>();
	bg_->Update(camera_.get());

	bgm_.LoadMP3("Music/stageSelect.mp3", "SelectBGM", bgmVolume_);

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
	gvu_->AddItem(anoKeys[MapPos], mapPos_);
	gvu_->AddItem(anoKeys[MapSize], mapSize_);

}

void nSelectScene::SetGlobalV()
{

	for (int i = 0; i < _countTags; i++) {
		if (i == LArrow) {
			arrowPos_[Left] = gvu_->GetVector2Value(spKeysP[i]);
		}
		else if (i == RArrow) {
			arrowPos_[Right] = gvu_->GetVector2Value(spKeysP[i]);
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
	mapPos_ = gvu_->GetVector2Value(anoKeys[MapPos]);
	mapSize_ = gvu_->GetVector2Value(anoKeys[MapSize]);

}

nSelectScene::~nSelectScene() {}

void nSelectScene::Initialize()
{
	//カメラ初期化
	camera_->Initialize();

	//option初期化
	optionUI_->Initialize();

	bg_->Initialize();

	bgm_.Play(true);

	SetGlobalV();

	Update();
}

void nSelectScene::Update()
{
	SetGlobalV();

#ifdef _DEBUG
	for (int i = 0; i < _countStages; i++) {
		mapSprite_[i]->pos_ = mapPos_;
		mapSprite_[i]->size_ = mapSize_;
	}
#endif // _DEBUG


	camera_->Update();

	if (isOptionActive_) {
		ans_ = optionUI_->Update();
	}
	else {
		InputUpdate();
	}

	ArrowUpdate();

	NumberUpdate();

	SceneChange();

	bg_->Update(camera_.get());

	//更新
	for (int i = 0; i < _countTags; i++) {
		sp_[i]->Update();
	}
	mapSprite_[pickedNum_]->Update();
}

void nSelectScene::Draw()
{
	//必須
	Kyoko::Engine::PreDraw();

	bg_->Draw();

	//描画
	for (int i = 0; i < _countTags; i++) {
		if (i == (int)Num10 && pickedNum_ < 9) {

		}
		else if (i == LArrow && pickedNum_ == 0) {
		}
		else if (i == RArrow && pickedNum_ == maxStageNum_ - 1) {
		}
		else {
			sp_[i]->Draw();
		}
	}

	mapSprite_[pickedNum_]->Draw();

	if (isOptionActive_) {
		optionUI_->Draw();
	}

	//シーン転換時のフェードインアウト
	BlackDraw();
	//必須
	Kyoko::Engine::PostDraw();
}



void nSelectScene::SceneChange()
{
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
			stageNo_ = pickedNum_;
			ChangeScene(STAGE);
			bgm_.Stop();
		}//オプション開く処理
		else if (input_->PressedGamePadButton(Input::GamePadButton::START)) {
			isOptionActive_ = true;
		}

	}
}

void nSelectScene::InputUpdate()
{
	Vector2 move = input_->GetGamePadLStick();

	if (isInputActive_) {
		//右入力
		if (move.x > inputDeadline_) {
			isInputActive_ = false;
			//最後のステージじゃないなら処理
			if (pickedNum_ != maxStageNum_ - 1) {
				pickedNum_++;
			}

		}//左入力
		else if (move.x < -inputDeadline_) {
			isInputActive_ = false;
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

void nSelectScene::ArrowUpdate()
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

void nSelectScene::NumberUpdate()
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
