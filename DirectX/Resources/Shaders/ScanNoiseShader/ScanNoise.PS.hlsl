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

struct ScanNoiseData {
	float min;
	float width;
	float power;
	int isNormal;
};
ConstantBuffer<ScanNoiseData> gScanNoise : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float32_t2 uv;
	float max = gScanNoise.min + gScanNoise.width;
	
	if (input.texcoord.y >= gScanNoise.min && input.texcoord.y <= max) {
        uv.x = sin(smoothstep(gScanNoise.min, max, input.texcoord.y) * 2 * 3.14159f) * gScanNoise.power;
    } 
	else {
        uv.x = input.texcoord.x;
    }
	uv.y = input.texcoord.y;

	output.color = gTexture.Sample(gSampler, uv);

	if (gScanNoise.isNormal == 1) {
		if (output.color.a <= 0.5f) {
			discard;
		}
	}

	return output;
}