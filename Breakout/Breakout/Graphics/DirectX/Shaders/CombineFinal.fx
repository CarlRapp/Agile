
cbuffer Object
{
	float g_Opacity;
};

cbuffer ObjectText
{
	float3 g_Color;
	float	g_Index;
	float	g_NumSymbols;
};

Texture2D g_Texture;

SamplerState g_Sampler 
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

SamplerState g_TextSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};


SamplerState samLinearClamp
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
	output.Pos			= float4(input.Pos, 0, 1);
	output.Tex			= input.Tex;
	return output;
}


PSSceneIn VSText(VSIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	output.Pos = float4(input.Pos, 0, 1);

	if (input.Tex.x == 0)
		output.Tex.x = g_Index / g_NumSymbols;
	else
		output.Tex.x = (g_Index + 1) / g_NumSymbols;

	output.Tex.y = 1 - input.Tex.y;
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input, uniform bool gColor, uniform bool gAlphaClip, uniform bool gFixedAlpha) : SV_Target
{	
	float4 result;

	if (gColor)
	{
		result = g_Texture.Sample(g_Sampler, input.Tex);

		if (gAlphaClip)
			clip(g_Texture.SampleLevel(samLinearClamp, input.Tex, 0).a - 0.1f);
	}
	else
		result = pow(abs(g_Texture.Sample(g_Sampler, input.Tex).x), 500);

	if (gFixedAlpha)
		result.a = g_Opacity;

	return result;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSText(PSSceneIn input) : SV_Target
{
	float4 result;

	float2 tex = input.Tex;
	//tex.y = 1 - tex.y;

	result.xyz = g_Color;
	result.a = g_Texture.SampleLevel(g_TextSampler, tex, 0).x;
	//result.a = 1;
	return result;
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

BlendState Transparency
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

//-----------------------------------------------------------------------------------------
// Technique: Mono  
//-----------------------------------------------------------------------------------------
technique11 Mono
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene(false, false, false) ) );
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	    //SetRasterizerState( NoCulling );
    }  
}

//-----------------------------------------------------------------------------------------
// Technique: Color 
//-----------------------------------------------------------------------------------------
technique11 Color
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene(true, false, false) ) );
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	    //SetRasterizerState( NoCulling );
    }  
}

//-----------------------------------------------------------------------------------------
// Technique: AlphaClipColor 
//-----------------------------------------------------------------------------------------
technique11 AlphaClipColor
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene(true, true, false) ) );
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	    //SetRasterizerState( NoCulling );
    }  
}

//-----------------------------------------------------------------------------------------
// Technique: BlendColor 
//-----------------------------------------------------------------------------------------
technique11 AlphaTransparencyColor
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene(true, true, false) ) );
		SetBlendState(Transparency, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	    //SetRasterizerState( NoCulling );
    }  
}

//-----------------------------------------------------------------------------------------
// Technique: BlendColor 
//-----------------------------------------------------------------------------------------
technique11 TransparencyColor
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene(true, true, true) ) );
		SetBlendState(Transparency, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	    //SetRasterizerState( NoCulling );
    }  
}

technique11 Text
{
	pass p0
	{
		// Set VS, GS, and PS
		SetVertexShader(CompileShader(vs_4_0, VSText()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSText()));
		SetBlendState(Transparency, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		//SetRasterizerState( NoCulling );
	}
}


