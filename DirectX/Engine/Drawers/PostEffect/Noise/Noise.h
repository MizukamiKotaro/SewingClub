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
		WATER_SURFACE,
		WATER,
	};

	void SetCameraPos(const Vector3& pos);

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
		Vector4 waterColor; // 水の靄の色
		Vector4 lightningColor; // 水の白のうねうねの色
		Vector2 cameraPos;
		float moveScale; // 大きいほどカメラの影響を受けない
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
	const Vector3* camera;
};