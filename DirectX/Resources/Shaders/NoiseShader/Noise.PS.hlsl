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

struct BlurData {
	float angle;
	float pickRange;
	float stepWidth;
	int32_t isCenterBlur;
	int32_t isNormal;
};
ConstantBuffer<BlurData> gBlur : register(b1);

float random(float32_t2 fact) {
	return frac(sin(dot(fact,float32_t(12.9898,78.233))) * 43758.5453);
}

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

	float r = random(transformedUV.xy);
	output.color = float32_t4(r,r,r,1);

	return output;
}