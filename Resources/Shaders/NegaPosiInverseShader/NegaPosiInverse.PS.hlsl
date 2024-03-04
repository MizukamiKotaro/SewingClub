#include "../SpriteShader/Sprite.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material {
	float32_t4 color;
	//int32_t enableLighting;
	float32_t4x4 uvTransform;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

struct NegaPosiInverseData {
	int isNormal;
};
ConstantBuffer<NegaPosiInverseData> gNegaPosiInverse : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

	output.color = gTexture.Sample(gSampler, transformedUV.xy);

	output.color = float32_t4(1.0f - output.color.r,1.0f - output.color.g,1.0f - output.color.b,output.color.a);

	if (gNegaPosiInverse.isNormal == 1) {
		if (output.color.a <= 0.5f) {
			discard;
		}
	}

	return output;
}