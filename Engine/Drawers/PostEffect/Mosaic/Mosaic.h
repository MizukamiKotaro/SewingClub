#pragma once
#include "BasePostEffect/BasePostEffect.h"

class Mosaic : public BasePostEffect
{
public:
	Mosaic();
	~Mosaic() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct MosaicData {
		float density;
		int isSquare;
		Vector2 screenSize;
		int isNormal;
	};
private:
	void CreateMosaicRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> mosaicResource_;
public:
	MosaicData* mosaicData_;
};