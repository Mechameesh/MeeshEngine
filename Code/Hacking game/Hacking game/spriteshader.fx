Texture2D diffuse : register(t0);
SamplerState linear : register(s0);

cbuffer matrixbuffer
{
	matrix wvp;
};

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
	PS_INPUT output;
	output.Pos = mul(input.Pos, wvp);
	output.Tex = input.Tex;
	return output;
}

float4 PS(float4 pos : SV_POSITION) : SV_Target
{
	return diffuse.Sample(linear, input.Tex);
}