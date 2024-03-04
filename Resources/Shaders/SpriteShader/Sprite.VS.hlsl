#include "Sprite.hlsli"

struct TransformationMatrix {
	float32_t4x4 WVP;
	//float32_t4x4 World;
};
ConstantBuffer<TransformationMatrix>gTransformationMatrix : register(b0);

struct VertexShaderInput {
	float32_t4 vertexPos : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.vertexPos = mul(input.vertexPos, gTransformationMatrix.WVP);
	output.texcoord = input.texcoord;
	return output;
}