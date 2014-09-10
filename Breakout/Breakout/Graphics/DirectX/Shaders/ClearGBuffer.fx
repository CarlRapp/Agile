struct VSIn
{
	float2 Pos 			: POSITION;
};

struct PSSceneIn
{
	float4 Pos			: SV_POSITION;
};

struct PSOut
{
	float4 Albedo		: COLOR0;
	float4 NormalSpec	: COLOR1;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSIn input)
{
	PSSceneIn output 	= (PSSceneIn)0;
	output.Pos			= float4(input.Pos, 0, 1);
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
PSOut PSScene(PSSceneIn input) : SV_Target
{	
	PSOut output;
	output.Albedo 		= float4(0,0,0,0);
	output.NormalSpec 	= float4(0.0f,0.0f,0,0);
	
	
	return output;
}


//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique11 ClearTech
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
    }  
}

