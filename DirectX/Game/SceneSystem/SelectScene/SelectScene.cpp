#include "SelectScene.h"
#include "Kyoko.h"
#include "Ease/Ease.h"

#include<imgui.h>

SelectScene::SelectScene()
{
	FirstInit();

	//オブジェクト初期化
	for (int i = 0; i < _countOfStageNumbers; i++) {
		stageBoxes_[i] = std::make_unique<Model>("WaterCircle");

		stageBoxes_[i]->Initialize();

		stageNumbers_[i] = std::make_unique<Model>("plane");
		stageNumbers_[i]->Initialize();
		
	}

	bgm_.LoadWave("Music/stageSelect.wav","SelectBGM",bgmVolume_);

}

void SelectScene::Initialize()
{
	//カメラ初期化
	camera_->Initialize();

	//ステージ箱選択
	float diff = 8;
	int count = 0;
	for (auto& box : stageBoxes_) {
		//初期化と配置
		box->Initialize();
		box->transform_.translate_.x = -diff + diff * count;
		box->Update();
		count++;
	}

	for (auto& t : ts_) {
		t = 0;
	}

	//データ初期化
	switchData_.changeReception = true;
	switchData_.moveLowerLimit = 0.8f;
	switchData_.resetFlagLimit = 0.2f;
	switchData_.maxSelectBoxScale_ = 1.5f;
	switchData_.minSelectBoxScale_ = 1.0f;

	bgm_.Play(true);
}

void SelectScene::Update()
{
	//デバッグ
	Debug();

	//カメラ更新
	camera_->Update();


	//ステージを選ぶ処理
	SelectStage();

	//シーン変更関係処理
	SceneChange();
}

void SelectScene::Draw()
{

	//必須
	Kyoko::Engine::PreDraw();

	//ステージ選択BOX
	for (auto& box : stageBoxes_) {
		box->Draw(*camera_.get());
	}

	//シーン転換時のフェードインアウト
	BlackDraw();
	//必須
	Kyoko::Engine::PostDraw();
}


void SelectScene::Debug()
{
#ifdef _DEBUG
	ImGui::Begin("debug");
	ImGui::Text("flag : %d , pickedNum : %d", switchData_.changeReception,pickedNum_);
	ImGui::End();
#endif // _DEBUG


}

void SelectScene::SceneChange()
{
	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		// シーン切り替え
		stageNo_ = pickedNum_;
		ChangeScene(STAGE);
		bgm_.Stop();
	}

	if (input_->PressedGamePadButton(Input::GamePadButton::B)) {
		// シーン切り替え
		ChangeScene(TITLE);
		bgm_.Stop();
	}
}

void SelectScene::SelectStage()
{

#pragma region 入力による選ばれている番号変更処理

	Vector2 lStick = input_->GetGamePadLStick();

	//操作受付フラグONの時
	if (switchData_.changeReception) {
		//入力をみて変更
		if (lStick.x >= switchData_.moveLowerLimit) {
			pickedNum_++;
			switchData_.changeReception = false;			
		}
		if (lStick.x <= -switchData_.moveLowerLimit) {
			pickedNum_--;
			switchData_.changeReception = false;
		}

		//選択外なら戻す
		if (pickedNum_ >= _countOfStageNumbers) {
			pickedNum_ = _countOfStageNumbers - 1;
		}
		else if (pickedNum_ < 0) {
			pickedNum_ = 0;
		}
	}
	else {
		//値がリセット範囲内の時
		if (lStick.x <= switchData_.resetFlagLimit && lStick.x >= -switchData_.resetFlagLimit) {
			switchData_.changeReception = true;
		}
	}


	
		
	//各更新処理
	for (int i = 0; i < _countOfStageNumbers; i++) {
		if (i == pickedNum_) {

			ts_[i] += switchData_.maxScaleMinute;
			if (ts_[i] > 1.0f) {
				ts_[i] = 1.0f;
			}
		}
		else {
			ts_[i] -= switchData_.minScaleMinute;
			if (ts_[i] < 0.0f) {
				ts_[i] = 0.0f;
			}
		}

		float scale = Ease::UseEase(switchData_.minSelectBoxScale_, switchData_.maxSelectBoxScale_, ts_[i]);

		Vector3 scaleV3 = { scale,scale, scale };

		//拡大
		stageBoxes_[i]->transform_.scale_ = scaleV3;
	}

#pragma endregion





	//ステージ箱の更新
	for (auto& box : stageBoxes_) {
		box->Update();
	}
}

