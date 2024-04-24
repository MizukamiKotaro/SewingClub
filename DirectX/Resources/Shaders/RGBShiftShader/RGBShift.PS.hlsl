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

struct RGBShiftData {
	float shift;
	int isNormal;
};
ConstantBuffer<RGBShiftData> gRGBShift : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	if(gRGBShift.shift == 0.0f){
		output.color = gTexture.Sample(gSampler, input.texcoord);

		if (gRGBShift.isNormal == 1) {
			if (output.color.a <= 0.5f) {
				discard;
			}
		}
		return output;
	}

	float r = gTexture.Sample(gSampler, input.texcoord + float32_t2(-gRGBShift.shift,0.0f)).r;
	float g = gTexture.Sample(gSampler, input.texcoord).g;
	float b = gTexture.Sample(gSampler, input.texcoord + float32_t2(gRGBShift.shift,0.0f)).b;
	float a = gTexture.Sample(gSampler, input.texcoord).a;
	output.color = float32_t4(r,g,b,a);
	
	if (gRGBShift.isNormal == 1) {
		if (output.color.a <= 0.5f) {
			discard;
		}
	}

	return output;
}