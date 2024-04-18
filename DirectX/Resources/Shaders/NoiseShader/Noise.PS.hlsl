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

struct NoiseData {
	float density;
	float time;
	float32_t2 screenSize;
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
		float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

		float r = random(transformedUV.xy);
		output.color = float32_t4(r,r,r,1);
	}
	else if(gNoise.type == 1){
		float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

		float r = random(floor(transformedUV.xy * gNoise.screenSize.xy / gNoise.density) * gNoise.density / gNoise.screenSize.xy);
		output.color = float32_t4(r,r,r,1);
	}
	else if(gNoise.type == 2){
		float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

		float v00 = random(floor(transformedUV.xy * gNoise.screenSize.xy / gNoise.density) * gNoise.density / gNoise.screenSize.xy);
		float v01 = random((floor(transformedUV.xy * gNoise.screenSize.xy / gNoise.density) + float32_t2(0,1)) * gNoise.density / gNoise.screenSize.xy);
		float v10 = random((floor(transformedUV.xy * gNoise.screenSize.xy / gNoise.density) + float32_t2(1,0)) * gNoise.density / gNoise.screenSize.xy);
		float v11 = random((floor(transformedUV.xy * gNoise.screenSize.xy / gNoise.density) + float32_t2(1,1)) * gNoise.density / gNoise.screenSize.xy);
		
		float32_t2 p = frac(transformedUV.xy * gNoise.screenSize.xy / gNoise.density);
		float v0010 = lerp(v00,v10,p.x);
		float v0111 = lerp(v01,v11,p.x);
		float n = lerp(v0010,v0111,p.y);

		output.color = float32_t4(n,n,n,1);
	}
	else if(gNoise.type == 3){
		float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

		float v00 = random(floor(transformedUV.xy * gNoise.screenSize.xy / gNoise.density) * gNoise.density / gNoise.screenSize.xy);
		float v01 = random((floor(transformedUV.xy * gNoise.screenSize.xy / gNoise.density) + float32_t2(0,1)) * gNoise.density / gNoise.screenSize.xy);
		float v10 = random((floor(transformedUV.xy * gNoise.screenSize.xy / gNoise.density) + float32_t2(1,0)) * gNoise.density / gNoise.screenSize.xy);
		float v11 = random((floor(transformedUV.xy * gNoise.screenSize.xy / gNoise.density) + float32_t2(1,1)) * gNoise.density / gNoise.screenSize.xy);
		
		float32_t2 p = frac(transformedUV.xy * gNoise.screenSize.xy / gNoise.density);
		float32_t2 v = p * p * (3 - 2 * p);

		float v0010 = lerp(v00,v10,v.x);
		float v0111 = lerp(v01,v11,v.x);
		float n = lerp(v0010,v0111,v.y);

		output.color = float32_t4(n,n,n,1);
	}
	else if(gNoise.type == 4){
		float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

		float n = PerlinNoise(gNoise.density, transformedUV.xy, gNoise.screenSize);

		output.color = float32_t4(n,n,n,1);
	}
	else if(gNoise.type == 5){
		float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

		float n = FractalSumNoise(gNoise.density, transformedUV.xy);

		output.color = float32_t4(n,n,n,1);
	}
	else if(gNoise.type == 6){
		float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

		float n1 = FractalSumNoise(gNoise.density, transformedUV.xy + float32_t2(3 * gNoise.time,0));
		float n2 = FractalSumNoise(gNoise.density, transformedUV.xy + float32_t2(gNoise.time,gNoise.time));
		float n3 = FractalSumNoise(gNoise.density, transformedUV.xy + float32_t2(gNoise.time,-gNoise.time));

		float n = (n1 + n2 + n3) / 3;

		output.color = float32_t4(n,n,n,1);
	}
	else if(gNoise.type == 7){
		float32_t4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

		float n1 = FractalSumNoise(gNoise.density, transformedUV.xy + float32_t2(gNoise.time,gNoise.time));
		float n2 = FractalSumNoise(gNoise.density, transformedUV.xy + float32_t2(-gNoise.time,-gNoise.time));
		
		float n = step(0.985,1-abs(n1-n2));

		output.color = float32_t4(n,n,n,1);
	}

	output.color.r = output.color.r * gMaterial.color.r;
	output.color.g = output.color.g * gMaterial.color.g;
	output.color.b = output.color.b * gMaterial.color.b;
	output.color.a = output.color.a * gMaterial.color.a;

	return output;
}