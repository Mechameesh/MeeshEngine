Texture2D diffuse : register(t0);
SamplerState linear : register(s0);

cbuffer constants : register(b0)
{
	matrix MVP;
	float4 ambient;
}

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, MVP);
	output.Tex = input.Tex;

	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	return diffuse.Sample(linear, input.Tex) * ambient;
}