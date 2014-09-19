#include "LightHelper.fx"


cbuffer cbPerObject
{
	float4x4 gTexTransform;
	Material gMaterial;
};

cbuffer cbPerObjectNormal
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
}; 

cbuffer cbPerObjectInstanced
{
	float4x4 gViewProj;
};


cbuffer cbSkinned
{
	float4x4 gBoneTransforms[96];
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gNormalMap;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VertexIn
{
	float3 PosL					: POSITION;
	float3 NormalL				: NORMAL;
	float2 Tex					: TEXCOORD;
	float4 TangentL				: TANGENT;
};

struct VertexInstancedIn
{
	float3 PosL					: POSITION;
	float3 NormalL				: NORMAL;
	float2 Tex					: TEXCOORD;
	float4 TangentL				: TANGENT;
	row_major float4x4 World	: WORLD;
	uint InstanceId				: SV_InstanceID;
};

struct VertexInC
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float2 Tex      : TEXCOORD;
	float4 TangentL : TANGENT;
	float4 ColorL   : COLOR;
};

struct SkinnedVertexIn
{
	float3 PosL       : POSITION;
	float3 NormalL    : NORMAL;
	float2 Tex        : TEXCOORD;
	float4 TangentL   : TANGENT;
	float3 Weights    : WEIGHTS;
	uint4 BoneIndices : BONEINDICES;
};

struct VertexOut
{
	float4 PosH       : SV_POSITION;
    float3 NormalW    : NORMAL;
	float4 TangentW   : TANGENT;
	float2 Tex        : TEXCOORD0;
	//float3 Color      : COLOR;
};

struct PsOut
{
	float4 Albedo		: COLOR0;
	float4 NormalSpec	: COLOR1;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.NormalW  = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.TangentW = mul(vin.TangentL, gWorld);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	//vout.Color = vin.ColorL;

	return vout;
}

VertexOut VSInstanced(VertexInstancedIn vin)
{
	VertexOut vout;

	// Transform to world space space.
	vout.NormalW = mul(vin.NormalL, (float3x3)vin.World);
	vout.TangentW = mul(vin.TangentL, vin.World);

	vout.PosH = mul(float4(vin.PosL, 1.0f), vin.World);

	// Transform to homogeneous clip space.
	vout.PosH = mul(vout.PosH, gViewProj);

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	//vout.Color = vin.ColorL;

	return vout;
}

VertexOut SkinnedVS(SkinnedVertexIn vin)
{
    VertexOut vout;

	// Init array or else we get strange warnings about SV_POSITION.
	float weights[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	weights[0] = vin.Weights.x;
	weights[1] = vin.Weights.y;
	weights[2] = vin.Weights.z;
	//weights[3] = vin.Weights.w;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 posL     = float3(0.0f, 0.0f, 0.0f);
	float3 normalL  = float3(0.0f, 0.0f, 0.0f);
	float3 tangentL = float3(0.0f, 0.0f, 0.0f);
	for(int i = 0; i < 4; ++i)
	{
	    // Assume no nonuniform scaling when transforming normals, so 
		// that we do not have to use the inverse-transpose.

	    posL     += weights[i]*mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;
		normalL  += weights[i]*mul(vin.NormalL,  (float3x3)gBoneTransforms[vin.BoneIndices[i]]);
		tangentL += weights[i]*mul(vin.TangentL.xyz, (float3x3)gBoneTransforms[vin.BoneIndices[i]]);
	}
 
	// Transform to world space space.
	vout.NormalW  = mul(normalL, (float3x3)gWorldInvTranspose);
	vout.TangentW = float4(mul(tangentL, (float3x3)gWorld), vin.TangentL.w);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);
	
	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	//vout.Color = float4(1, 1, 1, 1);

	return vout;
}


PsOut PS(VertexOut pin, 
		  uniform bool gUseTexure, 
		  uniform bool gUseNormalMap,
		  uniform bool gAlphaClip) : SV_Target
{
	PsOut pout;
	
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

    // Default to multiplicative identity.
    pout.Albedo = float4(1, 1, 1, 1);
	//pout.Albedo = float4(pin.Color, 1);
	//pout.Albedo = float4(pin.NormalW, 1);
    if(gUseTexure)
	{
		// Sample texture.
		pout.Albedo = gDiffuseMap.Sample( samLinear, pin.Tex );
		//return texColor;
		if(gAlphaClip)
		{
			float alpha = gDiffuseMap.SampleLevel( samPoint, pin.Tex, 0 ).a;
			clip(alpha - 0.1f);
		}
	}

	//
	// Normal mapping
	//
	float3 NormalW = pin.NormalW;
	if (gUseNormalMap)
	{
		float3 normalMapSample = gNormalMap.Sample(samLinear, pin.Tex).rgb;
		NormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);
	}

	pout.NormalSpec.xyz = (NormalW + float3(1.0f, 1.0f, 1.0f)) * 0.5f;
	pout.NormalSpec.w  	= gMaterial.SpecPower / 1000.0f;
	pout.Albedo.w  		= gMaterial.SpecIntensity;
	
	//pout.NormalSpec = float4(1,0,0,1);
	
	return pout;
}

technique11 BasicTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(false, false, false) ) );
    }
}

technique11 BasicInstancedTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSInstanced()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(false, false, false)));
	}
}

technique11 TexTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, false, false) ) );
    }
}

technique11 TexNormalTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, true, false) ) );
    }
}

technique11 TexAlphaClipTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, false, true) ) );
    }
}

technique11 TexNormalAlphaClipTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, true, true) ) );
    }
}

technique11 NormalTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(false, true, false) ) );
    }
}


///ANIMATION
technique11 TexSkinnedTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, SkinnedVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, false, false) ) );
    }
}

technique11 TexNormalSkinnedTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, SkinnedVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, true, false) ) );
    }
}

technique11 TexAlphaClipSkinnedTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, SkinnedVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, false, true) ) );
    }
}

technique11 TexNormalAlphaClipSkinnedTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, SkinnedVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, true, true) ) );
    }
}

technique11 NormalSkinnedTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, SkinnedVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(false, true, false) ) );
    }
}
