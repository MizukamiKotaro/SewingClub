#pragma once
#include "BasePostEffect/BasePostEffect.h"

class Noise : public BasePostEffect
{
public:
	Noise();
	~Noise() override;

	enum NoiseType {
		RAND_NOISE,
		MOSAIC_NOISE,
		MOSAIC_LERP_NOISE,
		MOSAIC_VALUE_NOISE,
		MOSAIC_PERLIN_NOISE,
		MOSAIC_FRACTAL_SUM_PERLIN_NOISE,
		SANDSTORM,
		LIGHTNING,
	};

	void Initialize();

	void Update(const float& time);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct NoiseData {
		float density; // 密度
		float time; // 時間
		Vector2 screenSize; // スクリーンのサイズ
		int type;
		int isNormal; // NormalBlendかどうか
	};
private:
	void CreateNoiseRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> noiseResource_;
public:
	NoiseData* noiseData_;
};