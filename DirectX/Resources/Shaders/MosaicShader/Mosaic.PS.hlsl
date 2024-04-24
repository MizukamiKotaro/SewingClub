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

struct MosaicData {
	float density;
	int isSquare;
	float32_t2 screenSize;
	int isNormal;
};
ConstantBuffer<MosaicData> gMosaic : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	if(gMosaic.density == 0.0f){
		output.color = gTexture.Sample(gSampler, input.texcoord);

		if (gMosaic.isNormal == 1) {
			if (output.color.a <= 0.5f) {
				discard;
			}
		}
		return output;
	}

	if(gMosaic.isSquare == 1){
		output.color = gTexture.Sample(gSampler,
		floor(input.texcoord * gMosaic.screenSize.xy / gMosaic.density) * gMosaic.density / gMosaic.screenSize.xy);
	}
	else {
		output.color = gTexture.Sample(gSampler, floor(input.texcoord * gMosaic.density) / gMosaic.density);
	}
	
	if (gMosaic.isNormal == 1) {
		if (output.color.a <= 0.5f) {
			discard;
		}
	}

	return output;
}