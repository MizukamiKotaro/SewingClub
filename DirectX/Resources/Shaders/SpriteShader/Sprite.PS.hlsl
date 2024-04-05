#include "Sprite.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material {
	float32_t4 color;
	float32_t4x4 uvTransform;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	/*float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
	output.color = gMaterial.color * textureColor;*/

	float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

	output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
	output.color.a = gMaterial.color.a * textureColor.a;

	if (textureColor.a <= 0.5 || output.color.a == 0) {
		discard;
	}
	
	return output;
}