#pragma once
#include "BasePostEffect/BasePostEffect.h"
#include "Vector3.h"

class Dissolve : public BasePostEffect
{
public:
	Dissolve();
	~Dissolve() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal) override;
	struct DissolveData {
		Vector3 edgeColor; // 堺の色
		float baseLuminance; // 基準の輝度
		float difference; // 輝度との差(堺の部分になる)
	};
	void SetGPUDescriptorHandle(const D3D12_GPU_DESCRIPTOR_HANDLE& mask);
private:
	void CreateDissolveRes();
	void CreateResources() override;
private:
	ComPtr<ID3D12Resource> dissolveResource_;
	D3D12_GPU_DESCRIPTOR_HANDLE maskTextureHandle_;
public:
	DissolveData* dissolveData_;
};