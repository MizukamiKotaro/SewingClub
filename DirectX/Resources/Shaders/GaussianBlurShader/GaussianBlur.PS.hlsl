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

struct GaussianBlurData {
	float pickRange;
	float stepWidth;
};
ConstantBuffer<GaussianBlurData> gGaussianBlur : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float32_t4 color = { 0.0f,0.0f,0.0f,0.0f };

	float32_t2 uv;
	float totalWeight = 0.0f;

	for (float py = -gGaussianBlur.pickRange * 2; py <= gGaussianBlur.pickRange * 2; py += gGaussianBlur.stepWidth) {
		for (float px = -gGaussianBlur.pickRange * 2; px <= gGaussianBlur.pickRange * 2; px += gGaussianBlur.stepWidth) {

			uv.x = input.texcoord.x + px;
			uv.y = input.texcoord.y + py;

			/// Gaussian
			float d = distance(input.texcoord, uv);
			float weight = exp(-(d * d) / (2.0f * gGaussianBlur.pickRange * gGaussianBlur.pickRange));
			///
			color += gTexture.Sample(gSampler, uv) * weight;
			totalWeight += weight;
		}
	}

	output.color.rgb = color.rgb / totalWeight;
	output.color.a = 1.0f;

	return output;
}