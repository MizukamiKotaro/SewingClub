#include "GameClear.h"
#include "GameElement/Animation/AnimationManager.h"

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

	for (int i = 0; i < _countValuations; i++) {
		baby_[i] = std::make_unique<Sprite>(babyPaths[i]);
		baby_[i]->SetTextureSize({baby_[i]->size_.x/3.0f,baby_[i]->size_.y});
		baby_[i]->SetTextureTopLeft({ 0,0 });
	}

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

	// 赤ちゃんアニメーション
	babyAnimation_ = std::make_unique<Animation2D>(AnimationManager::GetInstance()->AddAnimation("babynormal"));
	// UV座標のセット
	for (uint32_t index = 0u; index < Valuations::_countValuations; index++) {
		Transform handle = babyAnimation_->GetSceneUV(0u);
		baby_[index]->SetTextureTopLeft(Vector2(handle.translate_.x, handle.translate_.y));
		baby_[index]->SetTextureSize(Vector2(handle.scale_.x, handle.scale_.y));
	}
	babyAnimation_->Play(true);

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
			else if (nowSelect_ == Retry && isNextStage_) {
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
			if (nowSelect_ == StageSelect && isNextStage_) {
				nowSelect_ = NextStage;
			}
			else if (nowSelect_ == StageSelect && !isNextStage_) {
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

	kMaxGaugePos_ = sp_[Gage_Bar]->pos_;
	kMaxGaugeScale_ = sp_[Gage_Bar]->size_;

	Update(0.0f);

	isNextStage_ = nextstage;
	if (isNextStage_) {
		nowSelect_ = NextStage;
	}
	else {
		nowSelect_ = StageSelect;
	}
}

ClearAnswer GameClear::Update(const float& delta)
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

	if (babyAnimation_->Update("babynormal", delta)) {
		Transform trans = babyAnimation_->GetUVTrans();
		baby_[valuation_]->SetTextureTopLeft(Vector2(trans.translate_.x, trans.translate_.y));
		baby_[valuation_]->SetTextureSize(Vector2(trans.scale_.x, trans.scale_.y));
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
		else if (i == Baby) {
			baby_[valuation_]->pos_ = sp_[i]->pos_;
			baby_[valuation_]->size_ = sp_[i]->size_;
			baby_[valuation_]->Update();
			baby_[valuation_]->Draw();
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

void GameClear::SetBabyParam(const float& tension) {
	// ここで0 ~ 100 なのを 0 ~ 1でもらうようにしている
	float tensionPercent = tension * 0.01f;

	/*
	* pos,size,uvScale,uvTrans
	* 初期値 1060.0f,350.0f,1.0f,1.0f
	* 1097.5f,280.0f,0.8f,0.2fだった場合
	*/

	// uvを求める xのみでyはかならず固定
	// uvScaleを求める テンション率
	float uvScale = tensionPercent;
	// uv座標を求める 1.0f - テンション率
	float uvTrans = 1.0f - tensionPercent;
	// spriteSizeを求める 最大サイズ * uvScale
	float spriteSize = kMaxGaugeScale_.x * uvScale;
	// spriteの位置を求める (最大サイズ - 今のサイズ) / 2 + 定位置
	float spritePos = (kMaxGaugeScale_.x - spriteSize) * 0.5f;
	spritePos += kMaxGaugePos_.x;

	// 座標更新
	sp_[Gage_Bar]->SetTextureTopLeft(Vector2(uvTrans, 0.0f));
	sp_[Gage_Bar]->SetTextureSize(Vector2(uvScale, 1.0f));
	sp_[Gage_Bar]->pos_ = Vector2(spritePos, kMaxGaugePos_.y);
	sp_[Gage_Bar]->size_ = Vector2(spriteSize, kMaxGaugeScale_.y);
	sp_[Gage_Bar]->Update();

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
