#include "GameOver.h"
#include"RandomGenerator/RandomGenerator.h"
#include<numbers>

GameOver::GameOver()
{
	input_ = Input::GetInstance();

	back_ = std::make_unique<Sprite>("white.png");
	back_->size_ = { 1280,720 };
	back_->SetColor({ backColor_.x,backColor_.y,backColor_.z,alpha_ });

	arrow_ = std::make_unique<Sprite>("HUD_arrow.png");

	//開始位置
	Vector2 pos = allLogoPos_;
	pos.x -= wordLength_ * (float)_countText / 2.0f;

	int c = 0;
	for (auto& sp : GOText_) {
		sp = std::make_unique<Sprite>(paths[c]);
		sp->SetColor({ 0,0,0,1 });
		centerP_[c] = pos;
		pos.x += wordLength_;
		
		velo_[c] = {
				RandomGenerator::GetInstance()->RandFloat(-1,1),
				RandomGenerator::GetInstance()->RandFloat(-1,1)
		};

		rotates_[c] = RandomGenerator::GetInstance()->RandFloat(-rotateNUm_, rotateNUm_);

		c++;

	}

	text_[Retry] = std::make_unique<Sprite>("ingame_retry_HUD.png");
	text_[Retry]->SetColor({ 0,0,0,1 });
	text_[GoSelect] = std::make_unique<Sprite>("ingame_stageSelect_HUD.png");
	text_[GoSelect]->SetColor({ 0,0,0,1 });

	for (auto& sp : backCloud_) {
		sp = std::make_unique<Sprite>("back_obj.png");
	}

	gVU_ = new GlobalVariableUser("Scene", "GameOver");

	for (int i = 0; i < _countText; i++) {
		gVU_->AddItem(keysGOSize[i], GOText_[i]->size_);
	}
	for (int i = 0; i < _countSelectText; i++) {
		gVU_->AddItem(cKeysS[i], backCloud_[i]->size_);

		gVU_->AddItem(textKeysP[i], text_[i]->pos_);
		gVU_->AddItem(textKeysS[i], text_[i]->size_);
	}

	gVU_->AddItem(anoKeys[BackAlpha],alpha_);
	gVU_->AddItem(anoKeys[BackColor], backColor_);
	gVU_->AddItem(anoKeys[ArrowPos],arrowPos_ );
	gVU_->AddItem(anoKeys[ArrowSize], arrow_->size_);
	gVU_->AddItem(anoKeys[LogoPos], allLogoPos_);
	gVU_->AddItem(anoKeys[LogoLength], wordLength_);
	gVU_->AddItem(anoKeys[ArrowSecondSwing],swingSecond_);
	gVU_->AddItem(anoKeys[SwingNum], swingNum_);
	gVU_->AddItem(anoKeys[MinMaxSpd], minMaxSpd_);
	gVU_->AddItem(anoKeys[MaxDirection], maxDirection_);
	gVU_->AddItem(anoKeys[RotateNum], rotateNUm_);


}

void GameOver::SetGlobalV()
{
	for (int i = 0; i < _countText; i++) {
		GOText_[i]->size_ = gVU_->GetVector2Value(keysGOSize[i]);

	}
	for (int i = 0; i < _countSelectText; i++) {
		backCloud_[i]->size_ = gVU_->GetVector2Value(cKeysS[i]);

		text_[i]->pos_ = gVU_->GetVector2Value(textKeysP[i]);
		text_[i]->size_ = gVU_->GetVector2Value(textKeysS[i]);

	}

	alpha_ = gVU_->GetFloatValue(anoKeys[BackAlpha]);
	backColor_ = gVU_->GetVector3Value(anoKeys[BackColor]);
	arrowPos_ = gVU_->GetVector2Value(anoKeys[ArrowPos]);
	arrow_->size_ = gVU_->GetVector2Value(anoKeys[ArrowSize]);
	allLogoPos_ = gVU_->GetVector2Value(anoKeys[LogoPos]);
	wordLength_ = gVU_->GetFloatValue(anoKeys[LogoLength]);
	swingSecond_ = gVU_->GetFloatValue(anoKeys[ArrowSecondSwing]);
	swingNum_ = gVU_->GetFloatValue(anoKeys[SwingNum]);
	minMaxSpd_ = gVU_->GetVector2Value(anoKeys[MinMaxSpd]);
	maxDirection_ = gVU_->GetFloatValue(anoKeys[MaxDirection]);
	rotateNUm_ = gVU_->GetFloatValue(anoKeys[RotateNum]);
#ifdef _DEBUG
	back_->SetColor({ 0,0,0,alpha_ });

	//開始位置
	Vector2 pos = allLogoPos_;
	pos.x -= wordLength_ * (float)_countText / 2.0f;

	for (int c = 0; c < _countText;c++) {
		centerP_[c] = pos;
		pos.x += wordLength_;
	}
#endif // _DEBUG

	
}

GameOver::~GameOver()
{
}

void GameOver::Initialize()
{
	nowSelect_ = Retry;

	SetGlobalV();

	Update();
}

GameOverFlags GameOver::Update()
{

	SetGlobalV();

	
#pragma region 入力による選択物変更処理
	Vector2 move = input_->GetGamePadLStick();


	if (isInputMoveActive_) {
		//入力可能の場合の処理
		//左向きの処理
		if (move.x < -deadLineX_) {
			isInputMoveActive_ = false;

			if (nowSelect_ == GoSelect) {
				nowSelect_ = Retry;
			}

		}//右向きの処理
		else if (move.x > deadLineX_) {
			isInputMoveActive_ = false;

			if (nowSelect_ == Retry) {
				nowSelect_ = GoSelect;
			}
		}
	}
	else {
		//不可能状態のときの処理
		//デッドラインより内側デフラグオフ
		if (move.x > -deadLineX_&& move.x < deadLineX_) {
			isInputMoveActive_ = true;
		}
		
	}
#pragma endregion

#pragma region ロゴの動き処理
	for (int i = 0; i < _countText; i++) {
		//範囲外で向き変更
		if (movePos_[i].Length() > maxDirection_) {
			//座標を指定範囲内に収める
			movePos_[i] = movePos_[i].Normalize() * maxDirection_;

			//新しいベクトルを作成
			velo_[i] = {
				RandomGenerator::GetInstance()->RandFloat(-1,1),
				RandomGenerator::GetInstance()->RandFloat(-1,1)
			};
			velo_[i] = velo_[i].Normalize() * RandomGenerator::GetInstance()->RandFloat(minMaxSpd_.x, minMaxSpd_.y);
			rotates_[i] = RandomGenerator::GetInstance()->RandFloat(-rotateNUm_, rotateNUm_);
		}
		else {

			movePos_[i] += velo_[i];
			GOText_[i]->pos_ = centerP_[i] + movePos_[i];
			GOText_[i]->rotate_ += rotates_[i];
		}
	}
#pragma endregion


#pragma region 矢印の更新処理

	//動きの処理
	arrowSwing_ += swingSecond_ / 60.0f;
	arrowSwing_ = std::fmod(arrowSwing_, 2.0f * (float)std::numbers::pi);
	arrowAnime_.x = std::sin(arrowSwing_)*swingNum_;

	//座標更新
	arrow_->pos_ = text_[nowSelect_]->pos_ + arrowPos_ + arrowAnime_;
#pragma endregion

#pragma region 画像更新
	back_->SetColor({ backColor_.x,backColor_.y,backColor_.z,alpha_ });
	back_->Update();
	
	for (int i = 0; i < _countText; i++) {

		GOText_[i]->Update();

	}
	for (int i = 0; i < _countSelectText; i++) {

		backCloud_[i]->pos_ = text_[i]->pos_;
		backCloud_[i]->Update();
		text_[i]->Update();

	}

	arrow_->Update();

#pragma endregion

	return SceneChange();
}

void GameOver::Draw()
{

	back_->Draw();
	//
	for (auto& sp : GOText_) {
		sp->Draw();
	}
	//雲
	for (auto& sp : backCloud_) {
		sp->Draw();
	}
	//テキスト
	for (auto& sp : text_) {
		sp->Draw();
	}

	arrow_->Draw();
}



GameOverFlags GameOver::SceneChange()
{

	GameOverFlags result;

	//Aボタンで選択しているほうのフラグ音
	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		if (nowSelect_ == Retry) {
			result.restart = true;
		}
		else if (nowSelect_ == GoSelect) {
			result.goSelect = true;
		}
	}

	return result;
}
