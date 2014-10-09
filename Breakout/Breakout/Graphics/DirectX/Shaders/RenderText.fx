//--------------------------------------------------------------------------------------
// BasicCompute.fx
// Direct3D 11 Shader Model 5.0 Demo
// Copyright (c) Stefan Petersson, 2012
//--------------------------------------------------------------------------------------

cbuffer perFrame
{
	float4 gColor;
	float2 gPosition;
}

Texture2D g_Texture;

SamplerState g_Sampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VSIn
{
	float2 Pos 	: POSITION;
	float2 Tex	: TEXTURECOORD;
};

struct PSSceneIn
{
	float4 Pos	: SV_POSITION;
	float2 Tex	: TEXTURECOORD;
};

PSSceneIn VSScene(VSIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	output.Pos = float4(input.Pos, 0, 1);
	output.Tex = input.Tex;
	output.Tex.y = 1 - output.Tex;
	return output;
}

float4 PSScene(PSSceneIn input) : SV_Target
{
	float4 result;

	result = g_Texture.Sample(g_Sampler, input.Tex).x;

	clip(g_Texture.SampleLevel(g_Sampler, input.Tex, 0).a - 0.1f);

	return result;
}


BlendState NoBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = FALSE;
};

technique11 BasicTech
{
	pass p0
	{
		// Set VS, GS, and PS
		SetVertexShader(CompileShader(vs_4_0, VSScene()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSScene()));
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		//SetRasterizerState( NoCulling );
	}
}





//
//Texture2D gAlbedoMap;
//RWTexture2D<float4> gOutput;
//
//[numthreads(32, 32, 1)]
//void main( uint3 threadID : SV_DispatchThreadID )
//{
//	float color = gAlbedoMap[threadID.xy].x;
//
//	if (color > 0)
//	{
//		gOutput[gPosition + threadID.xy] = gColor * color;
//	}	
//}