#pragma once
#include "BasePostEffect/BasePostEffect.h"

class RGBShift : public BasePostEffect
{
public:
	RGBShift();
	~RGBShift() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct RGBShiftData {
		float shift; 
		int isNormal;
	};
private:
	void CreateRGBShiftRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> rgbShiftResource_;
public:
	RGBShiftData* rgbShiftData_;
};