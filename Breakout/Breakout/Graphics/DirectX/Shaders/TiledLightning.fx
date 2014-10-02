#define COMPUTE_SHADER_TILE_GROUP_DIM 16
#define COMPUTE_SHADER_TILE_GROUP_SIZE (COMPUTE_SHADER_TILE_GROUP_DIM*COMPUTE_SHADER_TILE_GROUP_DIM)

#include "LightHelper.fx"
#define MAX_LIGHTS 1024
#define MAX_SHADOWMAPS 256

cbuffer perFrame
{
	/*float4x4    gLightViewProjTex[MAX_SHADOWMAPS];
	float4x4    gLightViewProj[MAX_SHADOWMAPS];
	float4x4    gLightTex[MAX_SHADOWMAPS];*/
	float4x4	gInvViewProj;
	float4		gCamPosition;
	//float3		gShadowMapSwitches;
	//float3		gGlobalLight;
	float2		gResolution;
	//float2		gShadowMapResolution;
};

//Lights
StructuredBuffer<DirectionalLight>	gDirLightBuffer;
StructuredBuffer<PointLight>		gPointLightBuffer;
StructuredBuffer<SpotLight>			gSpotLightBuffer;

//GBuffer
Texture2D gAlbedoMap;
Texture2D gNormalSpecMap;
Texture2D gDepthMap;

//Shadowmap
//Texture2D gShadowMap0;
//Texture2D gShadowMap1;
//Texture2D gShadowMap2;
//Texture2D gShadowMap3;

//Output
RWTexture2D<float4> gOutput;

//Group Variables
groupshared int sMinX;
groupshared int sMaxX;
groupshared int sMinY;
groupshared int sMaxY;
groupshared int sMinZ;
groupshared int sMaxZ;

groupshared uint sTilePointLightIndices[MAX_LIGHTS];
groupshared uint sTileSpotLightIndices[MAX_LIGHTS];

groupshared uint sTileNumPointLights;
groupshared uint sTileNumSpotLights;

void ConeToAABB(float3 pos, float3 dir, float angle, out float3 min, out float3 max)
{

}

bool AABBvsSphere(float3 min, float3 max, float3 c, float r)
{
	float d = 0;
	
	for (int i = 0; i < 3; ++i)
	{
		float e1 = c[i] - min[i];
		float e2 = c[i] - max[i];
		if (e1 < 0)
		{
			if (e1 < -r)
			{
				return false;
			}
			d = d + pow(e1, 2);			
		}
	
		else if (e2 > 0)
		{
			if (e2 > r)
			{
				return false;
			}
			d = d + pow(e2, 2);			
		}	
	}

	if (d <= pow(r, 2))
	{
		return true;
	}
	return false;
}

//float SimpleShadow(int2 texCoord, float depth, uint shadowIndex)
//{
//	if (shadowIndex < gShadowMapSwitches.x)
//	{
//		if (depth > gShadowMap0[texCoord].x)
//			return 0;
//	}
//	
//	else if (shadowIndex < gShadowMapSwitches.y)
//	{
//		if (depth > gShadowMap1[texCoord].x)
//			return 0;
//	}
//	else if (shadowIndex < gShadowMapSwitches.z)
//	{
//		if (depth > gShadowMap2[texCoord].x)
//			return 0;
//	}
//	else
//	{
//		if (depth > gShadowMap3[texCoord].x)
//			return 0;
//	}
//	
//	return 1;
//}

//float PCF(int2 texCoord, float depth, uint shadowIndex)
//{
//	const float2 offsets[9] =
//	{
//		float2(-1,  -1),	float2(0, -1),	float2(1,  -1),
//		float2(-1,   0),	float2(0,  0),	float2(1,   0), 
//		float2(-1,   1),	float2(0,  1),	float2(1,   1) 		
//	};
//	
//	
//	float percentLit = 0.0f;
//	for (int i = 0; i < 9; ++i)
//	{
//		percentLit += SimpleShadow(texCoord + offsets[i], depth, shadowIndex);
//	}
//	return percentLit /= 9.0f;
//}



//float CalcualteShadowFactor(DirectionalLight light, float4 posW, uint viewportIndex)
//{
//	uint ShadowIndex = light.ShadowIndex[viewportIndex];
//	if (light.HasShadow && ShadowIndex != 9999)
//		{
//			float4x4 VPT = gLightViewProjTex[ShadowIndex];
//			float4x4 VP = gLightViewProj[ShadowIndex];
//			float4x4 T = gLightTex[ShadowIndex];
//
//			float4 posH = mul(posW, VP);
//			posH = posH / posH.w;
//			posH.xy = (posH.xy + float2(1,-1)) * float2(0.5f, -0.5f);
//
//			float4 posT = mul(posH, T);
//
//			return PCF(int2(posT.xy * gShadowMapResolution), posT.z, ShadowIndex);
//		}
//	else
//		return 1;
//}

//float CalcualteShadowFactor(PointLight light, float4 posW)
//{
//	uint lightShadowIndex = 0;
//
//	float3 dist = posW.xyz - light.Position;
//
//	if (abs(dist.x) > abs(dist.y) && abs(dist.x) > abs(dist.z))
//	{
//		if (dist.x < 0)
//			lightShadowIndex = 3; //vänster
//		else
//			lightShadowIndex = 0; //höger
//	}
//	else if (abs(dist.y) > abs(dist.z))
//	{
//		if (dist.y < 0)
//			lightShadowIndex = 4; //ner
//		else
//			lightShadowIndex = 1; //upp
//	}
//	else
//	{
//		if (dist.z < 0)
//			lightShadowIndex = 5; //bak
//		else
//			lightShadowIndex = 2; //fram
//	}
//
//	uint ShadowIndex = light.ShadowIndex[lightShadowIndex];
//	if (light.HasShadow && ShadowIndex != 9999)
//		{
//			float4x4 VPT = gLightViewProjTex[ShadowIndex];
//			float4x4 VP = gLightViewProj[ShadowIndex];
//			float4x4 T = gLightTex[ShadowIndex];
//
//			float4 posH = mul(posW, VP);
//			posH = posH / posH.w;
//			posH.xy = (posH.xy + float2(1,-1)) * float2(0.5f, -0.5f);
//
//			float4 posT = mul(posH, T);
//
//			return PCF(int2(posT.xy * gShadowMapResolution), posT.z, ShadowIndex);
//		}
//	else
//		return 1;
//}

//float CalcualteShadowFactor(SpotLight light, float4 posW)
//{
//	if (light.HasShadow && light.ShadowIndex != 9999)
//		{
//			float4x4 VPT = gLightViewProjTex[light.ShadowIndex];
//			float4x4 VP = gLightViewProj[light.ShadowIndex];
//			float4x4 T = gLightTex[light.ShadowIndex];
//
//			float4 posH = mul(posW, VP);
//			posH = posH / posH.w;
//			posH.xy = (posH.xy + float2(1,-1)) * float2(0.5f, -0.5f);
//
//			float4 posT = mul(posH, T);
//
//
//			return PCF(int2(posT.xy * gShadowMapResolution), posT.z, light.ShadowIndex);
//
//		}
//	else
//		return 1;
//}


[numthreads(COMPUTE_SHADER_TILE_GROUP_DIM, COMPUTE_SHADER_TILE_GROUP_DIM, 1)]	//Antal trådar per grupp x,y,z
void TiledLightningCS(	uint3 dispatchThreadID : SV_DispatchThreadID,
						uint3 groupThreadID    : SV_GroupThreadID
                         )
{
	uint groupIndex = groupThreadID.y * COMPUTE_SHADER_TILE_GROUP_DIM + groupThreadID.x;	//Lokalt trådindex


	//Extracting albedo, normal(world), pos(proj), material(specpower+specintesity)
	float4 albedo		= gAlbedoMap[dispatchThreadID.xy];
	float4 normalspec	= gNormalSpecMap[dispatchThreadID.xy];

	float4 normalW		= float4(normalspec.x, normalspec.y, normalspec.z, 0);	//Get X, Y-components of the Normal
	normalW				= (normalW * 2.0f) - float4(1, 1, 1, 0);	//[0, 1] to [-1, 1]

	//normalW 			= (normalW + float4(1,1,1,1)) * 0.5f;		//[-1, 1] to [0, 1] for testing purpose



	float depth			= gDepthMap[dispatchThreadID.xy].x;

	float4 posW;
	posW.xy = dispatchThreadID.xy / gResolution;
	posW.x  = posW.x * 2.0f		- 1.0f;
	posW.y	= posW.y * -2.0f	+ 1.0f;
	//posW.y	= posW.y * -1.0f	+ 1.0f;
	posW.z = depth;
	//posW.z = 0;
	posW.w = 1.0f;
	posW = mul(posW, gInvViewProj);
	posW /= posW.w;

	Material mat;
	mat.SpecIntensity	= albedo.w;
	mat.SpecPower		= normalspec.w * 1000.0f;


	float3 toEye = normalize(gCamPosition.xyz - posW.xyz);

	
	// Initialize shared memory light list and Z bounds
    if (groupIndex == 0) {	//Om vi är i första tråden för gruppen:
        sMinX = 0x7F7FFFFF;      // Max float
        sMaxX = 0x00000000;
		sMinY = 0x7F7FFFFF;      // Max float
        sMaxY = 0x00000000;
		sMinZ = 0x7F7FFFFF;      // Max float
        sMaxZ = 0x00000000;
		sTileNumPointLights = 0;
		sTileNumSpotLights = 0;
    }

	GroupMemoryBarrierWithGroupSync(); //Väntar på alla trådarna i gruppen
    /*
	sMinX
		*/

	if (depth != 1.0f)
	{
		InterlockedMin(sMinX, asint(posW.x)); //Ändrar delad sMinZ/sMaxZ till lokala variablen om den är mindre/större.
		InterlockedMax(sMaxX, asint(posW.x));

		InterlockedMin(sMinY, asint(posW.y)); //Ändrar delad sMinZ/sMaxZ till lokala variablen om den är mindre/större.
		InterlockedMax(sMaxY, asint(posW.y));

		InterlockedMin(sMinZ, asint(posW.z)); //Ändrar delad sMinZ/sMaxZ till lokala variablen om den är mindre/större.
		InterlockedMax(sMaxZ, asint(posW.z));
	}
    

	GroupMemoryBarrierWithGroupSync(); //Väntar på alla trådarna i gruppen

    float minTileX = asfloat(sMinX); //Hämtar minsta x-värdet för tilen
    float maxTileX = asfloat(sMaxX); //Hämtar största x-värdet för tilen
	float minTileY = asfloat(sMinY); //Hämtar minsta y-värdet för tilen
    float maxTileY = asfloat(sMaxY); //Hämtar största y-värdet för tilen
	float minTileZ = asfloat(sMinZ); //Hämtar minsta z-värdet för tilen
    float maxTileZ = asfloat(sMaxZ); //Hämtar största z-värdet för tilen

	float3 minTile = float3(minTileX, minTileY, minTileZ);
	float3 maxTile = float3(maxTileX, maxTileY, maxTileZ);

	uint totalDirLights = 0, totalPointLights = 0, totalSpotLights = 0, dummy;
	gDirLightBuffer.GetDimensions(totalDirLights, dummy);
    gPointLightBuffer.GetDimensions(totalPointLights, dummy);	
    gSpotLightBuffer.GetDimensions(totalSpotLights, dummy);

	//Kollar vilka ljus som berör tilen. 

	//Alla ytor berörs av alla directionallights.

	//PointLights
	for (uint lightIndex = groupIndex; lightIndex < totalPointLights; lightIndex += COMPUTE_SHADER_TILE_GROUP_SIZE) 
	{
		PointLight light = gPointLightBuffer[lightIndex];        
		if (AABBvsSphere(minTile, maxTile, light.Position, light.Range))
		{
			uint listIndex;
			InterlockedAdd(sTileNumPointLights, 1, listIndex);
			sTilePointLightIndices[listIndex] = lightIndex;
		}
    }


	//SpotLights
	for (uint lightIndex = groupIndex; lightIndex < totalSpotLights; lightIndex += COMPUTE_SHADER_TILE_GROUP_SIZE) 
	{
		SpotLight light = gSpotLightBuffer[lightIndex];        
		if (AABBvsSphere(minTile, maxTile, light.Position, light.Range))
		{
			uint listIndex;
			InterlockedAdd(sTileNumSpotLights, 1, listIndex);
			sTileSpotLightIndices[listIndex] = lightIndex;
		}
    }

	GroupMemoryBarrierWithGroupSync(); //Väntar på alla trådarna i gruppen

	float4 ambient	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse	= float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec		= float4(0.0f, 0.0f, 0.0f, 0.0f);

	//Calculate DirectionLights
	if (depth != 1.0f)
	{
		
		for(int i = 0; i < totalDirLights; ++i)
		{
			DirectionalLight light = gDirLightBuffer[i];			
			//float shadowFactor = CalcualteShadowFactor(light, posW, 0);

			float4 A, D, S;
			ComputeDirectionalLight(mat, light, normalW, toEye, A, D, S);

			ambient += A;
			diffuse += D;// *shadowFactor;
			spec += S;// *shadowFactor;

		}
	}
	
	//Calculate PointLights	
	
	for (uint tileLightIndex = 0; tileLightIndex < sTileNumPointLights; ++tileLightIndex) 
	{
		PointLight light = gPointLightBuffer[sTilePointLightIndices[tileLightIndex]]; //hämtar pointlight

		//float shadowFactor = CalcualteShadowFactor(light, posW);

		float4 A, D, S;
		ComputePointLight(mat, light, posW, normalW, toEye, A, D, S);

		ambient += A;    
		diffuse += D;// * shadowFactor;
		spec += S;// * shadowFactor;
	}

	/*
	for(int i = 0; i < totalPointLights; ++i)
	{

		float4 A, D, S;
		ComputePointLight(mat, gPointLightBuffer[i], posW, normalW, toEye, A, D, S);

		ambient += A;    
		diffuse += D;
		spec    += S;
	}


	*/
	//Calculate SpotLights
	
	for (uint tileLightIndex = 0; tileLightIndex < sTileNumSpotLights; ++tileLightIndex) 
	{
		
		SpotLight light = gSpotLightBuffer[sTileSpotLightIndices[tileLightIndex]]; //hämtar pointlight
		
		
		//float shadowFactor = CalcualteShadowFactor(light, posW);

		float4 A, D, S;
		ComputeSpotLight(mat, light, posW, normalW, toEye, A, D, S);
		
		ambient += A;    
		diffuse += D;// * shadowFactor;
		spec    += S;// * shadowFactor;
	}



	//Final result.
	//gOutput[dispatchThreadID.xy] = sTileNumSpotLights / 100.0f;
	//gOutput[dispatchThreadID.xy] = sTileNumPointLights / 100.0f;
	//gOutput[dispatchThreadID.xy] = posW.x / 4000;
	//gOutput[dispatchThreadID.xy] = (ambient + diffuse) + spec;
	//gOutput[dispatchThreadID.xy] = albedo;
	//gOutput[dispatchThreadID.xy] = normalW;
	//gOutput[dispatchThreadID.xy] = normalspec;
	gOutput[dispatchThreadID.xy] = albedo * (ambient + diffuse) + spec;
	//gOutput[dispatchThreadID.xy] = albedo * diffuse;
	//gOutput[dispatchThreadID.xy] = viewportColor * (albedo * (ambient + diffuse) + spec);
}


technique11 Basic
{
    pass P0
    {
		SetVertexShader( NULL );
        SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, TiledLightningCS() ) );
    }
}