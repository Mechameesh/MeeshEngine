Texture2D diffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer matrixbuffer : register(b0)
{
	matrix wvp;
};

struct VS_INPUT
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;
	output.Pos = mul(float4(input.Pos, 1), wvp);
	output.Tex = input.Tex;
	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	return diffuse.Sample(samLinear, input.Tex);
}