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
	float	 gExplodeTime;
}; 

cbuffer cbPerObjectInstanced
{
	float4x4 gViewProj;
};



// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseTex;
Texture2D gBlendTex;
Texture2D gNormalTex;

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
	float  ExplodeTime : EXPLODETIME;

	uint InstanceId				: SV_InstanceID;
};

struct VertexOut
{
	float3 PosL       : POSITION;
    float3 NormalW    : NORMAL;
	float4 TangentW   : TANGENT;
	float2 Tex        : TEXCOORD0;


	float4x4 WVP	  : WORLDVIEWPROJ;
	float  ExplodeTime : EXPLODETIME;
	//float3 Color      : COLOR;
};

struct PixelIn
{
	float4 PosH       : SV_POSITION;
	float3 NormalW    : NORMAL;
	float4 TangentW   : TANGENT;
	float2 Tex        : TEXCOORD0;
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
	vout.PosL = vin.PosL;
	//vout.PosL = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	vout.WVP = gWorldViewProj;

	vout.ExplodeTime = gExplodeTime;

	//vout.Color = vin.ColorL;

	return vout;
}

VertexOut VSInstanced(VertexInstancedIn vin)
{
	VertexOut vout;

	// Transform to world space space.
	vout.NormalW = mul(vin.NormalL, (float3x3)vin.World);
	vout.TangentW = mul(vin.TangentL, vin.World);

	vout.PosL = vin.PosL;
	//vout.PosL = mul(float4(vin.PosL, 1.0f), vin.World);

	// Transform to homogeneous clip space.
	//vout.PosL = mul(vout.PosL, gViewProj);

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;



	vout.WVP = mul(vin.World, gViewProj);


	vout.ExplodeTime = vin.ExplodeTime;
	//vout.Color = vin.ColorL;

	return vout;
}

struct PatchTess
{
	float EdgeTess[3] : SV_TessFactor;
	float InsideTess : SV_InsideTessFactor;
	// Additional info you want associated per patch.
};

PatchTess ConstantHS(InputPatch<VertexOut, 3> patch,
	uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	float tess = 3;
	if (patch[0].ExplodeTime > 0)
		tess = 1;


	// Uniformly tessellate the patch 3 times.
	pt.EdgeTess[0] = tess; // Left edge
	pt.EdgeTess[1] = tess; // Top edge
	pt.EdgeTess[2] = tess; // Right edge
	pt.InsideTess = tess; // u-axis (columns)
	return pt;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]

VertexOut HS(InputPatch<VertexOut, 3> p, uint i:SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	VertexOut hout;

	hout.PosL = p[i].PosL;
	hout.NormalW = p[i].NormalW;
	hout.TangentW = p[i].TangentW;
	hout.Tex = p[i].Tex;
	hout.WVP = p[i].WVP;
	hout.ExplodeTime = p[i].ExplodeTime;

	return hout;

}
/*
[domain("tri")]
VertexOut DS(PatchTess patchTess, float3 baryCoords : SV_DomainLocation, const OutputPatch<VertexOut, 3> tri)
{

	VertexOut dout;

	float3 p = baryCoords.x*tri[0].PosL + baryCoords.y*tri[1].PosL + baryCoords.z*tri[2].PosL;

		dout.Tex = baryCoords.x*tri[0].Tex +
		baryCoords.y*tri[1].Tex +
		baryCoords.z*tri[2].Tex;


	dout.NormalH = baryCoords.x*tri[0].NormalH +
		baryCoords.y*tri[1].NormalH +
		baryCoords.z*tri[2].NormalH;

	dout.PosL = baryCoords.x*tri[0].PosL +
		baryCoords.y*tri[1].PosL +
		baryCoords.z*tri[2].PosL;

	dout.NormalH = normalize(dout.NormalH);


	float scale = 25.0f;

	float h = texture_displacement.SampleLevel(linearSampler, dout.Tex, 0.6f).g;

	p += (scale*(h - 1))*dout.NormalH;


	dout.PosH = mul(float4(p, 1.0f), gWVP);

	dout.ShadowPosH = baryCoords.x*tri[0].ShadowPosH +
		baryCoords.y*tri[1].ShadowPosH +
		baryCoords.z*tri[2].ShadowPosH;

	dout.SsaoPosH = baryCoords.x*tri[0].SsaoPosH +
		baryCoords.y*tri[1].SsaoPosH +
		baryCoords.z*tri[2].SsaoPosH;


	dout.Diffuse = tri[0].Diffuse;
	dout.Specular = tri[0].Specular;
	dout.TextureInfo = tri[0].TextureInfo;



	return dout;
}
*/

[maxvertexcount(3)]
void GS(triangle VertexOut gin[3],
	inout TriangleStream<PixelIn> gout)
{
	PixelIn p[3];

	if (gin[0].ExplodeTime == 0)
	{
		[unroll]
		for (int i = 0; i < 3; ++i)
		{
			p[i].PosH = mul(float4(gin[i].PosL, 1.0f), gin[i].WVP);
			p[i].NormalW = gin[i].NormalW;
			p[i].TangentW = gin[i].TangentW;
			p[i].Tex = gin[i].Tex;
			gout.Append(p[i]);
		}
	}
	else
	{
		float3 faceEdgeA = gin[2].PosL - gin[0].PosL;
		float3 faceEdgeB = gin[1].PosL - gin[0].PosL;
		float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));
		float3 ExplodeAmt = faceNormal * gin[0].ExplodeTime;

		//
		// Calculate the face center
		//
		float3 centerPos = (gin[0].PosL.xyz + gin[1].PosL.xyz + gin[2].PosL.xyz) / 3.0;
		//float2 centerTex = (gin[0].Tex + gin[1].Tex + gin[2].Tex) / 3.0;
		//centerPos += ExplodeAmt;



		[unroll]
		for (int i = 0; i < 3; ++i)
		{
			float delta = min(1, gin[0].ExplodeTime * 0.3);

			//float3 pos = gin[i].PosL + (centerPos - gin[i].PosL) * delta * 0.5f;
			float3 pos = gin[i].PosL;

				p[i].PosH = float4(pos, 1.0f) + float4(ExplodeAmt, 0);

			p[i].PosH = mul(p[i].PosH, gin[i].WVP);

			p[i].NormalW = gin[i].NormalW;
			p[i].TangentW = gin[i].TangentW;
			p[i].Tex = gin[i].Tex;
			gout.Append(p[i]);
		}
	}
} 

PsOut PS(PixelIn pin,
		  uniform bool gUseTexure, 
		  uniform bool gUseNormalTex,
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

		float4 albedo = gDiffuseTex.Sample(samLinear, pin.Tex);
		float4 blend = gBlendTex.Sample(samLinear, pin.Tex);
		pout.Albedo = albedo * (1.0f - blend.a) + blend * blend.a;
		//return texColor;
		if(gAlphaClip)
		{
			float alpha = gDiffuseTex.SampleLevel( samPoint, pin.Tex, 0 ).a;
			clip(alpha - 0.1f);
		}
	}

	//
	// Normal mapping
	//
	float3 NormalW = pin.NormalW;
	if (gUseNormalTex)
	{
		float3 normalMapSample = gNormalTex.Sample(samLinear, pin.Tex).rgb;
		NormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);
	}

	pout.NormalSpec.xyz = (NormalW + float3(1.0f, 1.0f, 1.0f)) * 0.5f;
	pout.NormalSpec.w  	= gMaterial.SpecPower / 1000.0f;
	pout.Albedo.w  		= gMaterial.SpecIntensity / 10.0f;
	
	//pout.NormalSpec = float4(1,0,0,1);
	
	return pout;
}

technique11 BasicTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader( CompileShader( ps_5_0, PS(false, false, false) ) );
    }
}

technique11 BasicInstancedTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSInstanced()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS(false, false, false)));
	}
}

technique11 TexTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader( CompileShader( ps_5_0, PS(true, false, false) ) );
    }
}

technique11 TexInstancedTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSInstanced()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS(true, false, false)));
	}
}

technique11 TexNormalTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader( CompileShader( ps_5_0, PS(true, true, false) ) );
    }
}

technique11 TexNormalInstancedTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSInstanced()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS(true, true, false)));
	}
}

technique11 TexAlphaClipTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader( CompileShader( ps_5_0, PS(true, false, true) ) );
    }
}

technique11 TexNormalAlphaClipTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader( CompileShader( ps_5_0, PS(true, true, true) ) );
    }
}

technique11 NormalTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader( CompileShader( ps_5_0, PS(false, true, false) ) );
    }
}

technique11 NormalInstancedTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSInstanced()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS(false, true, false)));
	}
}

