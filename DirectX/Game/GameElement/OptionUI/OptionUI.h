#pragma once
//#include"GameElement/OptionUI/AudioOptionUI.h"
#include"GameElement/OptionUI/AudioOptionUI.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include"Audio.h"

struct UpdateAnswer {
	bool backOption = false;	//optionをやめるフラグ
	bool backSelect = false;	//セレクトに戻る
	bool backtitle = false;		//タイトルに戻る
	bool leaveGame = false;		//ゲームをやめる
	bool audioOption = false;	//オーディオいじってるか
};

class OptionUI {

public:

	enum SceneType {
		kTitle,
		kSelect,
		kStage,
		_countOfType
	};

	OptionUI(SceneType type);
	~OptionUI();

	void Initialize();

	UpdateAnswer Update();

	void Draw();

private:
	//シーン転換やONOFFの処理
	void SceneChange();

	//GlobalVの値代入
	void SetGrobalV();

	//矢印の更新
	void ArrowUpdate();

	void TitleUpdate();

	void SelectUpdate();

	void StageUpdate();

	void TitleDraw();

	void SelectDraw();

	void StageDraw();
private:

	Input* input_;

	//背景スプライト
	std::unique_ptr<Sprite>backSprite_;
	//背景透過度
	float backalpha_ = 0.5f;

	//枠
	std::unique_ptr<Sprite>blackFrame_;

	//シーンタイプ取得
	SceneType type_;

	//option内でのシーン
	enum OptionScene {
		kNone,
		kSound,
		_countOption
	};

	//現在の状態
	OptionScene opScene_ = kNone;

	//None内での選択
	enum NowSelect {
		BackGame,
		BackSelect,
		BackTitle,
		SoundSelect,
		LeaveGame,
	};

	NowSelect nowSelect_ = BackGame;

	std::unique_ptr<AudioOptionUI> audioOption_;
	//戻ったばっかの場合の誤作動防止
	bool isBacked_ = false;

	bool isStickBack_ = true;

	//選択矢印
	std::unique_ptr<Sprite>selectArrow_;

	//動きによる値の誤差
	Vector2 arrowPosition_ = { 0,0 };

	//矢印の振幅
	float arrowSwingNum_ = 1.0f;
	//振幅のカウント
	float swingCount_ = 0;

	float addSwingCount_ = 1.0f;

	std::unique_ptr<Sprite>textPause_;
	std::unique_ptr<Sprite>textBackGame_;
	std::unique_ptr<Sprite>textBackSelect_;
	std::unique_ptr<Sprite>textBackTitle_;
	std::unique_ptr<Sprite>textSoundEdit_;
	std::unique_ptr<Sprite>textLeaveGame_;


	UpdateAnswer ans_;

#pragma region 音
	Audio seMove_;
	Audio seSelect_;

#pragma endregion


#pragma region ImGui

	GlobalVariableUser* gVUser_ = nullptr;

	enum tags {
		SpriteSelectArrow,
		SelectArrowSize,
		ArrowSwing,
		ArrowSwingCount,
		TextBackGamePos,
		TextBackGameSize,
		TextBackSelectPos,
		TextBackSelectSize,
		TextBackTitlePos,
		TextBackTitleSize,
		TextSoundEditPos,
		TextSoundEditSize,
		TextLeaveGamePos,
		TextLeaveGameSize,
		BlackScreenAlpha,
		TextPause,
		TextPauseSize,
		_countTags
	};

	std::string keys[_countTags] = {
		"選択の矢印",
		"矢印のサイズ",
		"矢印の振幅",
		"一秒間での振幅速度",
		"テキスト　戻る　座標",
		"テキスト　戻る　size",
		"テキスト　ステージ選択　座標",
		"テキスト　ステージ選択　size",
		"テキスト　タイトル　座標",
		"テキスト　タイトル　size",
		"テキスト　音　座標",
		"テキスト　音　size",
		"テキスト　ゲームをやめる　座標",
		"テキスト　ゲームをやめる　size",
		"黒背景の透明度",
		"テキスト　ポーズ　座標",
		"テキスト　ポーズ　サイズ",
	};
#pragma endregion


};