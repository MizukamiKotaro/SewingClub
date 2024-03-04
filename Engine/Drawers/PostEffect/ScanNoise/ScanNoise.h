#pragma once
#include "BasePostEffect/BasePostEffect.h"

class ScanNoise : public BasePostEffect
{
public:
	ScanNoise();
	~ScanNoise() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct ScanNoiseData {
		float minY; 
		float width;
		float power;
		int isNormal;
	};
private:
	void CreateScanNoiseRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> scanNoiseResource_;
public:
	ScanNoiseData* scanNoiseData_;
};