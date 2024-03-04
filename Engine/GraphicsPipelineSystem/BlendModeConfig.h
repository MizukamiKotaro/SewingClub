#pragma once
enum class BlendMode
{
	kBlendModeNone, // ブレンドなし
	kBlendModeNormal, // デフォルト
	kBlendModeAdd, // 加算
	kBlendModeSubtract, // 減算
	kBlendModeMultiply, // 乗算
	kBlendModeScreen, // スクリーン
};