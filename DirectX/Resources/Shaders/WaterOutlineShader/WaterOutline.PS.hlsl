#include "../BasePostEffectShader/BasePostEffect.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material {
	float32_t4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct WaterOutlineData {
	float32_t2 screenSize;
	int32_t outlinePix;
	int32_t isNormal;
};
ConstantBuffer<WaterOutlineData> gWaterOutline : register(b1);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);

	output.color = textureColor;

	float n = textureColor.r * textureColor.g * textureColor.b * textureColor.a;

	if(n != 0){
		float y = 1.0 / gWaterOutline.screenSize.y;
		float x = 1.0 / gWaterOutline.screenSize.x;
		for(int i = 0; i < gWaterOutline.outlinePix; i++){
			textureColor = gTexture.Sample(gSampler, input.texcoord + float32_t2((-i - 1) * x,0));
			n = n * textureColor.a;

			textureColor = gTexture.Sample(gSampler, input.texcoord + float32_t2((i + 1) * x,0));
			n = n * textureColor.a;

			textureColor = gTexture.Sample(gSampler, input.texcoord + float32_t2(0,(-i - 1) * y));
			n = n * textureColor.a;

			textureColor = gTexture.Sample(gSampler, input.texcoord + float32_t2(0,(i + 1) * y));
			n = n * textureColor.a;
		}

		if(n == 0){
			output.color = gMaterial.color;
		}
	}

	return output;
}