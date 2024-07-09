#include "../BasePostEffectShader/BasePostEffect.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

Texture2D<float32_t4> gMask : register(t1);

struct Material {
	float32_t4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

struct DissolveData {
	float32_t3 edgeColor;
	float32_t baseLuminance;
	float32_t difference;
};
ConstantBuffer<DissolveData> gDissolve : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float32_t4 maskColor = gMask.Sample(gSampler, input.texcoord);
	float32_t mask = dot(maskColor.rgb, float32_t3(0.2125f, 0.7154f, 0.0721f));

	if(mask <= gDissolve.baseLuminance){
		discard;
	}

	float32_t edge = 1.0f - smoothstep(gDissolve.baseLuminance, gDissolve.baseLuminance + gDissolve.difference, mask);
	output.color = gTexture.Sample(gSampler, input.texcoord);
	output.color.rgb += edge * gDissolve.edgeColor;

	return output;
}