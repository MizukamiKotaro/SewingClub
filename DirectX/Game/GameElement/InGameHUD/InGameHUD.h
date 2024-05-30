#pragma once
#include"Sprite.h"
#include"GlobalVariables/GlobalVariableUser.h"

#include<iostream>

class InGameHUD {
public:
	InGameHUD();
	~InGameHUD();

	void Initialize();

	void Update();

	void Draw();

private:

	void SetGlobalV();

private:

	enum Tags {
		Icon_Pase,
		Text_Pause,
		_CountTags
	};

	std::unique_ptr<Sprite> sp_[_CountTags];

	std::string paths[_CountTags] = {
		"controler_UI_=-.png",
		"ingame_pose_HUD.png"
	};


#pragma region ImGui
	GlobalVariableUser* gvu_=nullptr;

	std::string sKeysP_[_CountTags] = {
		"「＝」座標",
		"「ポーズ」座標"
	};

	std::string sKeysS_[_CountTags] = {
		"「＝」size",
		"「ポーズ」size"
	};
#pragma endregion

};