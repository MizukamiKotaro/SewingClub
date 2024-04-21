#pragma once
#include "BasePostEffect/BasePostEffect.h"

class Water : public BasePostEffect
{
public:
	Water();
	~Water() override;

	void Initialize();

	void Update(const float& time);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct WaterData {
		float density; // 密度
		float time; // 時間
		Vector2 screenSize; // スクリーンのサイズ
		int isNormal; // NormalBlendかどうか
	};
private:
	void CreateWaterRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> waterResource_;
public:
	WaterData* waterData_;
};