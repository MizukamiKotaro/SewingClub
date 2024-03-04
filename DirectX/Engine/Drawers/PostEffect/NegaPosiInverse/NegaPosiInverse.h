#pragma once
#include "BasePostEffect/BasePostEffect.h"

class NegaPosiInverse : public BasePostEffect
{
public:
	NegaPosiInverse();
	~NegaPosiInverse() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct NegaPosiInverseData {
		int isNormal;
	};
private:
	void CreateNegaPosiInverseRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> negaPosiInverseResource_;
public:
	NegaPosiInverseData* negaPosiInverseData_;
};