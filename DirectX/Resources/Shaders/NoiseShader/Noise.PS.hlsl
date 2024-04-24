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

struct NoiseData {
	float density;
	float time;
	float32_t2 screenSize;
	float32_t4 waterColor;
	float32_t4 lightningColor;
	float32_t2 cameraPos;
	float moveScale;
	int32_t type;
	int32_t isNormal;
};
ConstantBuffer<NoiseData> gNoise : register(b1);

float random(float32_t2 fact) {
	return frac(sin(dot(fact,float32_t2(12.9898,78.233))) * 43758.5453);
}

float32_t2 randomVec(float32_t2 fact){
	float32_t2 angle = float32_t2(
		dot(fact,float32_t2(127.1,311.7)),
		dot(fact,float32_t2(269.5,183.3))
	);
	return frac(sin(angle) * 43758.5453123) * 2 - 1;
}

float PerlinNoise(float density, float32_t2 uv, float32_t2 screenSize){
	float32_t2 uvFloor = floor(uv * screenSize / density);
	float32_t2 uvFrac = frac(uv * screenSize / density);

	float32_t2 v00 = randomVec(uvFloor);
	float32_t2 v01 = randomVec(uvFloor + float32_t2(0,1));
	float32_t2 v10 = randomVec(uvFloor + float32_t2(1,0));
	float32_t2 v11 = randomVec(uvFloor + float32_t2(1,1));

	float c00 = dot(v00, uvFrac);
	float c01 = dot(v01, uvFrac - float32_t2(0,1));
	float c10 = dot(v10, uvFrac - float32_t2(1,0));
	float c11 = dot(v11, uvFrac - float32_t2(1,1));

	float32_t2 u = uvFrac * uvFrac * (3 - 2 * uvFrac);

	float v0010 = lerp(c00,c10,u.x);
	float v0111 = lerp(c01,c11,u.x);

	return lerp(v0010,v0111,u.y) / 2 + 0.5;
}

float FractalPerlinNoise(float density, float32_t2 uv){
	float32_t2 uvFloor = floor(uv * density);
	float32_t2 uvFrac = frac(uv * density);

	float32_t2 v00 = randomVec(uvFloor);
	float32_t2 v01 = randomVec(uvFloor + float32_t2(0,1));
	float32_t2 v10 = randomVec(uvFloor + float32_t2(1,0));
	float32_t2 v11 = randomVec(uvFloor + float32_t2(1,1));

	float c00 = dot(v00, uvFrac);
	float c01 = dot(v01, uvFrac - float32_t2(0,1));
	float c10 = dot(v10, uvFrac - float32_t2(1,0));
	float c11 = dot(v11, uvFrac - float32_t2(1,1));

	float32_t2 u = uvFrac * uvFrac * (3 - 2 * uvFrac);

	float v0010 = lerp(c00,c10,u.x);
	float v0111 = lerp(c01,c11,u.x);

	return lerp(v0010,v0111,u.y) / 2 + 0.5;
}

float FractalSumNoise(float density, float32_t2 uv){
	float fn;
	fn = FractalPerlinNoise(density, uv) / 2;
	fn += FractalPerlinNoise(density * 2, uv) / 4;
	fn += FractalPerlinNoise(density * 4, uv) / 8;
	fn += FractalPerlinNoise(density * 8, uv) / 16;
	return fn;
}

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	
	if(gNoise.type == 0){
		float r = random(input.texcoord);
		output.color = float32_t4(r,r,r,1);
	}
	else if(gNoise.type == 1){
		float r = random(floor(input.texcoord * gNoise.screenSize.xy / gNoise.density) * gNoise.density / gNoise.screenSize.xy);
		output.color = float32_t4(r,r,r,1);
	}
	else if(gNoise.type == 2){
		float v00 = random(floor(input.texcoord * gNoise.screenSize.xy / gNoise.density) * gNoise.density / gNoise.screenSize.xy);
		float v01 = random((floor(input.texcoord * gNoise.screenSize.xy / gNoise.density) + float32_t2(0,1)) * gNoise.density / gNoise.screenSize.xy);
		float v10 = random((floor(input.texcoord * gNoise.screenSize.xy / gNoise.density) + float32_t2(1,0)) * gNoise.density / gNoise.screenSize.xy);
		float v11 = random((floor(input.texcoord * gNoise.screenSize.xy / gNoise.density) + float32_t2(1,1)) * gNoise.density / gNoise.screenSize.xy);
		
		float32_t2 p = frac(input.texcoord * gNoise.screenSize.xy / gNoise.density);
		float v0010 = lerp(v00,v10,p.x);
		float v0111 = lerp(v01,v11,p.x);
		float n = lerp(v0010,v0111,p.y);

		output.color = float32_t4(n,n,n,1);
	}
	else if(gNoise.type == 3){
		float v00 = random(floor(input.texcoord * gNoise.screenSize.xy / gNoise.density) * gNoise.density / gNoise.screenSize.xy);
		float v01 = random((floor(input.texcoord * gNoise.screenSize.xy / gNoise.density) + float32_t2(0,1)) * gNoise.density / gNoise.screenSize.xy);
		float v10 = random((floor(input.texcoord * gNoise.screenSize.xy / gNoise.density) + float32_t2(1,0)) * gNoise.density / gNoise.screenSize.xy);
		float v11 = random((floor(input.texcoord * gNoise.screenSize.xy / gNoise.density) + float32_t2(1,1)) * gNoise.density / gNoise.screenSize.xy);
		
		float32_t2 p = frac(input.texcoord * gNoise.screenSize.xy / gNoise.density);
		float32_t2 v = p * p * (3 - 2 * p);

		float v0010 = lerp(v00,v10,v.x);
		float v0111 = lerp(v01,v11,v.x);
		float n = lerp(v0010,v0111,v.y);

		output.color = float32_t4(n,n,n,1);
	}
	else if(gNoise.type == 4){
		float n = PerlinNoise(gNoise.density, input.texcoord, gNoise.screenSize);

		output.color = float32_t4(n,n,n,1);
	}
	else if(gNoise.type == 5){
		float n = FractalSumNoise(gNoise.density, input.texcoord);

		output.color = float32_t4(n,n,n,1);
	}
	else if(gNoise.type == 6){
		float n1 = FractalSumNoise(gNoise.density, input.texcoord + float32_t2(3 * gNoise.time,0));
		float n2 = FractalSumNoise(gNoise.density, input.texcoord + float32_t2(gNoise.time,gNoise.time));
		float n3 = FractalSumNoise(gNoise.density, input.texcoord + float32_t2(gNoise.time,-gNoise.time));

		float n = (n1 + n2 + n3) / 3;

		output.color = float32_t4(n,n,n,1);
	}
	else if(gNoise.type == 7){
		float n1 = FractalSumNoise(gNoise.density, input.texcoord + float32_t2(gNoise.time,gNoise.time));
		float n2 = FractalSumNoise(gNoise.density, input.texcoord + float32_t2(-gNoise.time,-gNoise.time));
		
		float n = step(0.985,1-abs(n1-n2));

		output.color = float32_t4(n,n,n,1);
	}
	else if(gNoise.type == 8){
		float n1 = FractalSumNoise(gNoise.density, input.texcoord + float32_t2(gNoise.time,gNoise.time));
		float n2 = FractalSumNoise(gNoise.density, input.texcoord + float32_t2(-gNoise.time,-gNoise.time));
		
		float32_t4 textureColor = gTexture.Sample(gSampler, float32_t2(input.texcoord.x + n1 / 5, input.texcoord.y + n2 / 5));

		output.color = textureColor;
	}
	else if(gNoise.type == 9){
		float32_t2 pos = float32_t2(gNoise.cameraPos.x / (16 * gNoise.moveScale),-gNoise.cameraPos.y / (9 * gNoise.moveScale));

		float n1 = FractalSumNoise(gNoise.density, input.texcoord + pos + float32_t2(gNoise.time,gNoise.time));
		float n2 = FractalSumNoise(gNoise.density, input.texcoord + pos + float32_t2(-gNoise.time,-gNoise.time));
		
		float32_t4 textureColor = gTexture.Sample(gSampler, float32_t2(input.texcoord.x + n1 / 5, input.texcoord.y + n2 / 5));

		float n3 = (n1 + n2) / 2;

		float n4 = step(0.985,1-abs(n1-n2));

		float32_t4 waterColor = float32_t4(n3,n3,n3,1);
		waterColor.r = waterColor.r * gNoise.waterColor.r;
		waterColor.g = waterColor.g * gNoise.waterColor.g;
		waterColor.b = waterColor.b * gNoise.waterColor.b;

		float32_t4 lightningColor = float32_t4(n4,n4,n4,1);
		lightningColor.r = lightningColor.r * gNoise.lightningColor.r;
		lightningColor.g = lightningColor.g * gNoise.lightningColor.g;
		lightningColor.b = lightningColor.b * gNoise.lightningColor.b;

		output.color.r = clamp(waterColor.r + lightningColor.r + textureColor.r, 0, 1);
		output.color.g = clamp(waterColor.g + lightningColor.g + textureColor.g, 0, 1);
		output.color.b = clamp(waterColor.b + lightningColor.b + textureColor.b, 0, 1);
		output.color.a = 1;
	}

	output.color.r = output.color.r * gMaterial.color.r;
	output.color.g = output.color.g * gMaterial.color.g;
	output.color.b = output.color.b * gMaterial.color.b;
	output.color.a = output.color.a * gMaterial.color.a;

	return output;
}