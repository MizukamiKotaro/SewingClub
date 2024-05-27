#include "GameClear.h"

#include<numbers>

GameClear::GameClear()
{

	input_ = Input::GetInstance();

	for (int i = 0; i < _countText; i++) {
		sp_[i] = std::make_unique<Sprite>(paths[i]);
	}

	for (int i = 0; i < _countPSelect; i++) {
		selects_[i] = std::make_unique<Sprite>(selePath_[i]);
	}

	sp_[Back]->size_ = { 1280,720 };

	gvu_ = new GlobalVariableUser("Scene", "GameClear");

	for (int i = 0; i < _countText; i++) {
		if (i == (int)Arrow) {
			gvu_->AddItem(spKeysP[i], arrowPos_);
			gvu_->AddItem(spKeysS[i], sp_[i]->size_);
		}
		else {
			gvu_->AddItem(spKeysP[i], sp_[i]->pos_);
			gvu_->AddItem(spKeysS[i], sp_[i]->size_);

		}

	}

	for (int i = 0; i < _countPSelect; i++) {
		gvu_->AddItem(keysPseleP[i], selects_[i]->pos_);
		gvu_->AddItem(keysPseleS[i], selects_[i]->size_);
	}

	gvu_->AddItem(anoKeys[BackAlpha], alpha_);
	gvu_->AddItem(anoKeys[BackColor], color_);
	gvu_->AddItem(anoKeys[TextColor], HUDColor_);
	gvu_->AddItem(anoKeys[SwingSecond], swingSecond_);
	gvu_->AddItem(anoKeys[SwingNum], swingNum_);
	gvu_->AddItem(anoKeys[GageColor], GageColor_);

	SetGlobalV();
}

void GameClear::SetGlobalV()
{
	for (int i = 0; i < _countText; i++) {

		if (i == (int)Arrow) {
			arrowPos_ = gvu_->GetVector2Value(spKeysP[i]);
			sp_[i]->size_ = gvu_->GetVector2Value(spKeysS[i]);
		}
		else {
			sp_[i]->pos_ = gvu_->GetVector2Value(spKeysP[i]);
			sp_[i]->size_ = gvu_->GetVector2Value(spKeysS[i]);


		}

	}
	for (int i = 0; i < _countPSelect; i++) {

		selects_[i]->pos_ = gvu_->GetVector2Value(keysPseleP[i]);
		selects_[i]->size_ = gvu_->GetVector2Value(keysPseleS[i]);
	}

	alpha_ = gvu_->GetFloatValue(anoKeys[BackAlpha]);
	color_ = gvu_->GetVector3Value(anoKeys[BackColor]);
	HUDColor_ = gvu_->GetVector3Value(anoKeys[TextColor]);
	swingSecond_ = gvu_->GetFloatValue(anoKeys[SwingSecond]);
	swingNum_ = gvu_->GetFloatValue(anoKeys[SwingNum]);
	GageColor_ = gvu_->GetVector3Value(anoKeys[GageColor]);
}

void GameClear::InputUpdate()
{

	Vector2 move = input_->GetGamePadLStick();
	if (inputActive_) {
		//右入力
		if (move.x > stickD_) {
			inputActive_ = false;
			if (nowSelect_ == StageSelect) {
				nowSelect_ = Retry;
			}
			else if (nowSelect_ == Retry&&isNextStage_) {
				nowSelect_ = NextStage;
			}
			else if (nowSelect_ == Retry && !isNextStage_) {
				nowSelect_ = Retry;
			}
			else if (nowSelect_ == NextStage) {
				nowSelect_ = StageSelect;
			}

		}
		if (move.x < -stickD_) {
			inputActive_ = false;
			if (nowSelect_ == StageSelect&&isNextStage_) {
				nowSelect_ = NextStage;
			}else if (nowSelect_ == StageSelect && !isNextStage_) {
				nowSelect_ = Retry;
			}
			else if (nowSelect_ == Retry) {
				nowSelect_ = StageSelect;
			}
			else if (nowSelect_ == NextStage) {
				nowSelect_ = Retry;
			}

		}

	}
	else {
		if (move.x < stickD_ && move.x > -stickD_) {
			inputActive_ = true;
		}
	}
}

void GameClear::ArrowUpdate()
{
	//動きの処理
	count_ += swingSecond_ / 60.0f;
	count_ = std::fmod(count_, 2.0f * (float)std::numbers::pi);

	animeMove_.x = +std::sin(count_) * swingNum_;

	sp_[Arrow]->pos_ = selects_[nowSelect_]->pos_ + arrowPos_ + animeMove_;
	sp_[Arrow_Frame]->pos_ = selects_[nowSelect_]->pos_;
	
}

GameClear::~GameClear()
{
}

void GameClear::Initialize(bool nextstage)
{
	isNextStage_ = nextstage;
	if (isNextStage_) {
		nowSelect_ = NextStage;
	}
	else {
		nowSelect_ = StageSelect;
	}
	count_ = 0;

	sp_[text_Num]->SetTextureTopLeft({ 125,0 });
	sp_[text_Num]->SetTextureSize({ 125,125 });
	sp_[text_Num]->size_ = { 64,64 };
	SetGlobalV();

	sp_[Back]->SetColor({ color_.x,color_.y,color_.z,alpha_ });
	sp_[Valuation_Normal]->SetColor({ HUDColor_.x,HUDColor_.y,HUDColor_.z,1 });
	sp_[Valuation_Good]->SetColor({ HUDColor_.x,HUDColor_.y,HUDColor_.z,1 });
	sp_[Valuation_Parfect]->SetColor({ HUDColor_.x,HUDColor_.y,HUDColor_.z,1 });
	sp_[Gage_Bar]->SetColor({ GageColor_.x,GageColor_.y,GageColor_.z,1 });

	Update();

	isNextStage_ = nextstage;
	if (isNextStage_) {
		nowSelect_ = NextStage;
	}
	else {
		nowSelect_ = StageSelect;
	}
}

ClearAnswer GameClear::Update()
{
#ifdef _DEBUG
	SetGlobalV();

	sp_[Back]->SetColor({ color_.x,color_.y,color_.z,alpha_ });
	sp_[Valuation_Normal]->SetColor({ HUDColor_.x,HUDColor_.y,HUDColor_.z,1 });
	sp_[Valuation_Good]->SetColor({ HUDColor_.x,HUDColor_.y,HUDColor_.z,1 });
	sp_[Valuation_Parfect]->SetColor({ HUDColor_.x,HUDColor_.y,HUDColor_.z,1 });
	sp_[Gage_Bar]->SetColor({ GageColor_.x,GageColor_.y,GageColor_.z,1 });
#endif // _DEBUG



	InputUpdate();

	ArrowUpdate();

	//選択物のみいろっを変える処理
	for (int i = 0; i < _countPSelect; i++) {
		if (i == (int)nowSelect_) {
			selects_[i]->SetColor({ HUDColor_.x,HUDColor_.y,HUDColor_.z,1 });
		}
		else {
			selects_[i]->SetColor({ 1,1,1,1 });
		}
	}

	//画像更新
	for (int i = 0; i < _countText; i++) {
		sp_[i]->Update();
	}
	for (int i = 0; i < _countPSelect; i++) {
		selects_[i]->Update();
	}

	return ScceneChange();
}

void GameClear::Draw()
{
	//描画
	for (int i = 0; i < _countText; i++) {

		//ノーマル評価画像で評価同じなら描画
		if (i == (int)Valuation_Normal){
			if (valuation_ == Normal) {
				sp_[i]->Draw();
			}
		}//good評価画像で評価同じなら描画
		else if (i == (int)Valuation_Good){
			if (valuation_ == Good) {
				sp_[i]->Draw();
			}
		}//perfec評価画像で評価同じなら描画
		else if (i == (int)Valuation_Parfect){
			if (valuation_ == Perfect) {
				sp_[i]->Draw();
			}
		}
		else {
			//それ以外のごみ
			sp_[i]->Draw();
		}


	}

	for (int i = 0; i < _countPSelect; i++) {
		if (i == (int)NextStage && !isNextStage_) {
			
		}
		else {
			selects_[i]->Draw();
		}
	}
}



ClearAnswer GameClear::ScceneChange()
{
	ClearAnswer result;

	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		if (nowSelect_ == StageSelect) {
			result.goSelect = true;
		}
		else if (nowSelect_ == Retry) {
			result.reTry = true;
		}
		else if (nowSelect_ == NextStage) {
			result.goNeext = true;
		}
	}

	return result;
}
