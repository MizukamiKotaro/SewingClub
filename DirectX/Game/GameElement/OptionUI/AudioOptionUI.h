#pragma once
#include"Input.h"
#include"Sprite.h"
#include"Audio/VolumeManager/VolumeManager.h"
#include"GlobalVariables/GlobalVariableUser.h"
#include"Vector3.h"
#include"Audio.h"

class AudioOptionUI
{
public:
	AudioOptionUI();
	~AudioOptionUI();

	void Initialize();

	bool Update();

	void Draw();
private:

	void SetGlobalVData();

	void AudioBarUpdate();

	//画像更新
	void UpdateSprite();

private:
	
	Input* input_=nullptr;
	VolumeManager* VM_;

	Audio seMove_;
	Audio seSelectSE_;
	Audio seSelectBGM_;

#pragma region 画像群

	//ゲージまとめの数
	enum VolumeType
	{
		BGM,
		SE,
		Back,
		_countVolumeType 
	};

	//ゲージ一つに使っている画像の数
	enum Sprites {
		Frame,
		Gage,
		_countSprite
	};
	//ゲージ画像
	std::unique_ptr<Sprite>gage_[_countVolumeType][_countSprite];
	//矢印
	std::unique_ptr<Sprite>arrow_;
	
	//テキスト「音設定」
	std::unique_ptr<Sprite>text_volume_;

	std::string paths[_countVolumeType][_countSprite] = {
		{"SoundGageBar_All_BGM.png","soundGage.png"},
		{"SoundGageBar_Al_SE.png","soundGage.png"},
		{"ingame_pose_back_HUD.png","ingame_pose_back_HUD.png"},
	};
#pragma endregion
	//選択物
	VolumeType nowSelect_ = BGM;

	//ゲージの最大スケール保存
	Vector2 maxGageScale_[_countVolumeType];

	//矢印座標
	Vector2 aPos_ = { 0,0 };
	Vector2 animeA_ = { 0,0 };
	//振幅のカウント
	float swingCount_ = 0;
	//矢印の振幅
	float arrowSwingNum_ = 1.0f;
	float addSwingCount_ = 1.0f;

	//入力初期化範囲
	float inputDline_ = 0.5f;
	//入力可能フラグ
	bool isInputActive_ = false;

	float volume_[_countVolumeType] = { 0.5f };

	//入力で変わる音量
	float moveValue_ = 0.01f;

	enum Colors
	{
		Text,
		SelectFrame,
		SelectGage,
		NonSelectFrame,
		NonSelectGage,
		_countColors
	};

	Vector3 colors_[_countColors ];

#pragma region ImGui
	GlobalVariableUser* gvu_;

	std::string keysP[_countVolumeType][_countSprite] = {
		{"BGM フレーム　座標","BGM ゲージ 座標"},
		{"SE フレーム　座標","SE ゲージ 座標"},
		{"戻る 座標","関係なし"}
	};

	std::string keysS[_countVolumeType][_countSprite] = {
		{"BGM フレーム　サイズ","BGM ゲージ サイズ"},
		{"SE フレーム　サイズ","SE ゲージ サイズ"},
		{"戻る　サイズ","関係なし"},
	};

	enum Another {
		ArrowPos,
		ArrowSize,
		SwingS,
		SwingN,
		TextVolumePos,
		TextVolumeSize,
		_countAno
	};

	std::string anoKeys[_countAno] = {
		"矢印座標",
		"矢印サイズ",
		"矢印振幅速度",
		"矢印振幅量",
		"「音量」 座標",
		"「音量」 サイズ",
	};



	std::string colorKey_[_countColors] = {
		"「音量」色",
		"選んでる フレーム 色",
		"選んでる ゲージ 色",
		"選んでいない フレーム 色",
		"選んでいない ゲージ 色",
	};
#pragma endregion

};

