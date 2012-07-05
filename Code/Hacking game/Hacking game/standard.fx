Texture2D diffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer constants : register(b0)
{
	matrix MVP;
	float4 ambient;
}

struct VS_INPUT
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(float4(input.Pos, 1.0f), MVP);
	output.Tex = input.Tex;

	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	return diffuse.Sample(samLinear, input.Tex) * ambient;
}