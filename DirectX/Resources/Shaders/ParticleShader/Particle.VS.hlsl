#include "Particle.hlsli"

struct ParticleForGPU {
	float32_t4x4 WVP;
	float32_t4x4 World;
	float32_t4 color;
};
StructuredBuffer<ParticleForGPU>gParticles : register(t0);

struct VertexShaderInput {
	float32_t4 vertexPos : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceID : SV_InstanceID) {
	VertexShaderOutput output;
	output.vertexPos = mul(input.vertexPos, gParticles[instanceID].WVP);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float32_t3x3)gParticles[instanceID].World));
	output.color = gParticles[instanceID].color;
	return output;
}