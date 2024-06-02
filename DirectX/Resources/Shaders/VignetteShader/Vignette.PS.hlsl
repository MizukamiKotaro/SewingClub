#include "../BasePostEffectShader/BasePostEffect.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material {
	float32_t4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

struct VignetteData {
	float scale;
	float powNum;
};
ConstantBuffer<VignetteData> gVignette : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	output.color = gTexture.Sample(gSampler, input.texcoord);
	float32_t2 correct = input.texcoord * (1.0f - input.texcoord.yx);
	float vignette = correct.x * correct.y * gVignette.scale;
	vignette = saturate(pow(vignette,gVignette.powNum));
	output.color.rgb = output.color.rgb * (1.0f - vignette) * gMaterial.color.rgb;
	output.color.a = (1.0f - vignette) * gMaterial.color.a; 

	return output;
}