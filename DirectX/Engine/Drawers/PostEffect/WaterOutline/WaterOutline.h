#pragma once
#include "BasePostEffect/BasePostEffect.h"

class WaterOutline : public BasePostEffect
{
public:
	WaterOutline();
	~WaterOutline() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct WaterOutlineData {
		Vector2 screenSize;
		int outlinePix;
		int isNormal; // NormalBlendかどうか
	};
private:
	void CreateWaterRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> waterResource_;
public:
	WaterOutlineData* waterData_;
};