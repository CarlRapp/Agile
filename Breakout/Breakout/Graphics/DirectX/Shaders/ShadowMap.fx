cbuffer Object
{
	matrix				gWorldViewProj;
	matrix				gTexTransform;
};

cbuffer cbSkinned
{
	float4x4 gBoneTransforms[96];
};

Texture2D gDiffuseMap;

SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VSIn
{
	float3 PosL 			: POSITION;
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

struct PSSceneIn
{
	float4 PosH			: SV_POSITION;
};


//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
RasterizerState NoCulling
{
	CullMode = NONE;
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSIn input)
{
	PSSceneIn output 	= (PSSceneIn)0;
	//output.PosH			= mul(float4(input.Pos, 1), World);
	//output.PosH			= mul(output.Pos, ViewProjection);
	output.PosH			= mul(float4(input.PosL, 1.0f), gWorldViewProj);
	//output.Pos.z		= 1.0f;
	return output;
}

PSSceneIn VSSkinned(SkinnedVertexIn vin)
{
    PSSceneIn vout;

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
	}
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);

	return vout;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
void PSScene(PSSceneIn input)// : SV_Target
{
}



struct VSInPosTex
{
	float3 PosL 		: POSITION;
	float3 Normal		: NORMAL;
	float2 Tex 			: TEXCOORD;
};




struct PSSceneInPosTex
{
	float4 PosH			: SV_POSITION;
	float2 Tex 			: TEXCOORD;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneInPosTex VSSceneAlphaClip(VSInPosTex input)
{
	PSSceneInPosTex output 	= (PSSceneInPosTex)0;
	//output.PosH				= mul(float4(input.Pos, 1), World);
	//output.PosH				= mul(output.Pos, ViewProjection);

	output.PosH				= mul(float4(input.PosL, 1.0f), gWorldViewProj);

	//output.Tex				= input.Tex;
	output.Tex				= mul(float4(input.Tex, 0.0f, 1.0f), gTexTransform).xy;
	//output.Pos.z			= 1.0f;
	return output;
}

PSSceneInPosTex VSSkinnedAlphaClip(SkinnedVertexIn vin)
{
    PSSceneInPosTex vout;

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
	}
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);
	
	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}


//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
void PSSceneAlphaClip(PSSceneInPosTex input)// : SV_Target
{
	clip(gDiffuseMap.SampleLevel(samPoint, input.Tex, 0).a - 0.1f);
}

DepthStencilState DepthStencil
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

BlendState NoBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = FALSE;
};

RasterizerState DepthDir
{
	// [From MSDN]
	// If the depth buffer currently bound to the output-merger stage has a UNORM format or
	// no depth buffer is bound the bias value is calculated like this: 
	//
	// Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope;
	//
	// where r is the minimum representable value > 0 in the depth-buffer format converted to float32.
	// [/End MSDN]
	// 
	// For a 24-bit depth buffer, r = 1 / 2^24.
	//
	// Example: DepthBias = 100000 ==> Actual DepthBias = 100000/2^24 = .006

	// You need to experiment with these values for your scene.

	DepthBias = 7000;
    DepthBiasClamp = 0.0f;
	SlopeScaledDepthBias = 1.8f;
	CullMode = NONE;
};

RasterizerState DepthPoint
{
	DepthBias = 250.0f;
    DepthBiasClamp = 0.0f;
	SlopeScaledDepthBias = 0.8f;
	CullMode = NONE;
};

RasterizerState DepthSpot
{
	DepthBias = 50.0f;
    DepthBiasClamp = 0.0f;
	SlopeScaledDepthBias = 0.8f;
	CullMode = NONE;
};


//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique11 BasicShadowDir
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthDir );
    }  
}

technique11 BasicShadowPoint
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthPoint );
    }  
}

technique11 BasicShadowSpot
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthSpot );
    }  
}


//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique11 AlphaClipShadowDir
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSSceneAlphaClip() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSSceneAlphaClip() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthDir );
    }  
}

technique11 AlphaClipShadowPoint
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSSceneAlphaClip() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSSceneAlphaClip() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthPoint );
    }  
}

technique11 AlphaClipShadowSpot
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSSceneAlphaClip() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSSceneAlphaClip() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthSpot );
    }  
}







//ANIMATION


//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique11 SkinnedBasicShadowDir
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSSkinned() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthDir );
    }  
}

technique11 SkinnedBasicShadowPoint
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSSkinned() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthPoint );
    }  
}

technique11 SkinnedBasicShadowSpot
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSSkinned() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthSpot );
    }  
}


//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique11 SkinnedAlphaClipShadowDir
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSSkinnedAlphaClip() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSSceneAlphaClip() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthDir );
    }  
}

technique11 SkinnedAlphaClipShadowPoint
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSSkinnedAlphaClip() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSSceneAlphaClip() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthPoint );
    }  
}

technique11 SkinnedAlphaClipShadowSpot
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSSkinnedAlphaClip() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSSceneAlphaClip() ) );
	    SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( DepthStencil, 0 );
	    SetRasterizerState( DepthSpot );
    }  
}

