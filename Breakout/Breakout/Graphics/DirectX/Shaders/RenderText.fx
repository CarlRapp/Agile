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

Texture2D gAlbedoMap;
RWTexture2D<float4> gOutput;

[numthreads(32, 32, 1)]
void main( uint3 threadID : SV_DispatchThreadID )
{
	float color = gAlbedoMap[threadID.xy].x;

	if (color > 0)
	{
		gOutput[gPosition + threadID.xy] = gColor * color;
	}	
}