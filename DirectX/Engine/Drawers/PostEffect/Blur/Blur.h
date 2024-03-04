#pragma once
#include "BasePostEffect/BasePostEffect.h"

class Blur : public BasePostEffect
{
public:
	Blur();
	~Blur() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct BlurData {
		float angle; // 角度。0だと横ブラー、1.57fだと縦ブラー
		float pickRange; // 取得する色の幅
		float stepWidth; // 取得する色の位置変更の幅。0.0f < stepWidth < pickRange
		int isCenterBlur; // 真ん中からブラーか。1=true,0=false;
		int isNormal; // NormalBlendかどうか
	};
private:
	void CreateBlurRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> blurResource_;
public:
	BlurData* blurData_;
};