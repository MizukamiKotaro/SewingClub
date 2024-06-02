#pragma once
#include "BasePostEffect/BasePostEffect.h"

class Vignette : public BasePostEffect
{
public:
	Vignette();
	~Vignette() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct VignetteData {
		float scale;
		float powNum;
	};
private:
	void CreateVignetteRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> vignetteResource_;
public:
	VignetteData* vignetteData_;
};