#pragma once
#include "BasePostEffect/BasePostEffect.h"

class HighLumi : public BasePostEffect
{
public:
	HighLumi();
	~HighLumi() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct HighLumiData {
		float min;
		float max;
		int isToWhite;
	};
private:
	void CreateHighLumiRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> highLumiResource_;
public:
	HighLumiData* highLumiData_;
};