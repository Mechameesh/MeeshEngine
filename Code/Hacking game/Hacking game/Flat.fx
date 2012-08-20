cbuffer constants : register(b0)
{
	matrix WVP;	
}

struct VS_INPUT
{
	float3 Pos : POSITION;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color: COLOR;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(float4(input.Pos, 1.0f), WVP);
	output.Color = float4(1.0f, 1.0f, 0.0f, 1.0f);
	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	return input.Color;
}