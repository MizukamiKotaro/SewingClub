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

struct BlurData {
	float angle;
	float pickRange;
	float stepWidth;
	int32_t isCenterBlur;
	int32_t isNormal;
};
ConstantBuffer<BlurData> gBlur : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float32_t4 color = { 0.0f,0.0f,0.0f,0.0f };

	float angle = gBlur.angle * 3.14159f / 180;
	float32_t2 uv;
	float totalWeight = 0.0f;

	for (float i = -gBlur.pickRange; i <= gBlur.pickRange; i += gBlur.stepWidth) {
		float x = cos(angle) * i;
		float y = -sin(angle) * i;

		if (gBlur.isCenterBlur != 1) {
			x -= cos(angle) * gBlur.pickRange;
			y += sin(angle) * gBlur.pickRange;
		}
		uv.x = input.texcoord.x + x;
		uv.y = input.texcoord.y + y;

		/// Gaussian
		float d = distance(input.texcoord, uv);
		float weight = exp(-(d * d) / (2.0f * gBlur.pickRange * gBlur.pickRange));
		///
		color += gTexture.Sample(gSampler, uv) * weight;
		totalWeight += weight;
	}

	output.color = color / totalWeight * gMaterial.color;

	if (gBlur.isNormal == 1) {
		if (output.color.a <= 0.5f) {
			discard;
		}
	}

	return output;
}