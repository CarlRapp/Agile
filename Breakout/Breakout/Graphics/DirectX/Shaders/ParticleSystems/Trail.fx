#include "ParticleSystem.fx"


float aliveTime = 0.5f;


DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

DepthStencilState NoDepthWrites
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

//blendinginställningar.
BlendState AdditiveBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

//definierar PT_FLARE
#define PT_EMITTER2 1
#define PT_TRAIL 2


//streamoutgeometryshader
//skickar ut partiklar som en pointstream. max 2 st.
[maxvertexcount(2)]
void StreamOutGS(point Particle input[1], inout PointStream<Particle> ptStream)
{
	input[0].age += gDeltaTime;	//gör partiklen äldre.
	if (input[0].type == PT_EMITTER)//om partiklen är en emit-partikel
	{
		//input[0].size.x += length(gEmitVelW.xyz) * gDeltaTime * 0.25f;
		//input[0].initialVelW += gEmitVelW.xyz * gDeltaTime;
		if (input[0].age > 0.033f)	//om det är dags för att skapa nya partiklar.
		{
			Particle p;
			p.initialPosW = gEmitPosW;
			p.initialVelW = gEmitPosW;
			//p.size = 0;
			p.size = input[0].size;
			p.age = 0.0f;
			p.type = PT_TRAIL;
			ptStream.Append(p);

			// reset the time to emit
			input[0].age = 0.0f;

			input[0].initialVelW = gEmitPosW;

			input[0].type = PT_EMITTER2;
		}
		ptStream.Append(input[0]); //streamar ut emit-partiklen.
	}
	else if (input[0].type == PT_EMITTER2) //om partiklen är en emit-partikel
	{
		if (input[0].age > 0.033f)	//om det är dags för att skapa nya partiklar.
		{
			input[0].size.y = input[0].size.x;
			input[0].size.x += length(gEmitPosW - input[0].initialVelW);


			Particle p;
			p.initialPosW = gEmitPosW;
			p.initialVelW = input[0].initialVelW;
			//p.size = 0;
			p.size = input[0].size;
			p.age = 0.0f;
			p.type = PT_TRAIL;
			ptStream.Append(p);

			// reset the time to emit
			input[0].age = 0.0f;

			input[0].initialVelW = gEmitPosW;
		}
		ptStream.Append(input[0]); //streamar ut emit-partiklen.
	}
	else
	{
		//om partiklen inte är för gammal så streamas den ut.
		if (input[0].age <= aliveTime)
			ptStream.Append(input[0]); //streamar ut partiklen.
	}

}

GeometryShader GS_StreamOut = ConstructGSWithSO(CompileShader(gs_4_0, StreamOutGS()), "POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x");

//StreamOutTech har  ingen pixelshader samt depth buffer avaktiverad.
technique11 StreamOutTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, StreamOutVS()));
		SetGeometryShader(GS_StreamOut);
		SetPixelShader(NULL);
		SetDepthStencilState(DisableDepth, 0);
	}
}


struct VS_OUT
{
	float3 posW : POSITION0;
	float3 pos2W : POSITION1;
	float4 color : COLOR;
	float2 texC : TEXCOORD;
	uint type : TYPE;
};

//drawvertexshader
VS_OUT DrawVS(Particle input)
{
	VS_OUT vOut;
	float t = input.age;
	vOut.posW = input.initialPosW;  //räknar ut positionen på partiklen.
	vOut.pos2W = input.initialVelW;  //räknar ut positionen på partiklen.
	float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / aliveTime);
	opacity = opacity * 0.5f;
	vOut.color = float4(1.0f, 1.0f, 1.0f, opacity);	//gör färgengenomskinlig över tid.
	vOut.texC = input.size;
	vOut.type = input.type;
	return vOut;
}


struct GS_OUT
{
	float4 posH : SV_Position;
	float4 color : COLOR;
	float2 texC : TEXCOORD;
};

#define SIZE 1.5f
#define TEXSIZE (1.0f / SIZE)

// The draw GS just expands points into camera facing quads.
[maxvertexcount(4)]
void DrawGS(point VS_OUT input[1], inout TriangleStream<GS_OUT> lineStream)
{
	if (input[0].type == PT_TRAIL)
	{
		float4 pos[2];
		pos[0] = float4(input[0].posW, 1.0f);
		pos[1] = float4(input[0].pos2W, 1.0f);

		[unroll]
		for (int i = 0; i < 2; ++i)
		{
			pos[i] = mul(pos[i], gView);
		}

		float4 dir = pos[0] - pos[1];
		float len = length(dir.xyz);

		dir = float4(normalize(cross(dir.xyz, float3(0, 0, 1))), 0.0f);

		float tex[2] = { input[0].texC.x, input[0].texC.y };

		GS_OUT gOut;
		[unroll]
		for (int i = 0; i < 2; ++i)
		{
			float4 temp = pos[i];
			temp = pos[i] - dir * input[0].color.w * SIZE;
			gOut.posH = mul(temp, gProjection);
			gOut.color = input[0].color;
			gOut.texC = float2(0.0f, tex[i] * TEXSIZE);
			lineStream.Append(gOut);	//streamar ut partiklen.

			temp = pos[i] + dir * input[0].color.w * SIZE;
			gOut.posH = mul(temp, gProjection);
			gOut.color = input[0].color;
			gOut.texC = float2(1.0f, tex[i] * TEXSIZE);
			lineStream.Append(gOut);	//streamar ut partiklen.			
		}

	}

}

float4 DrawPS(GS_OUT input) : SV_TARGET
{
	//return input.color;
	return Texture.Sample(g_Sampler, input.texC) * input.color;
}


//Drawtech har med pixelshader.
technique11 DrawTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, DrawVS()));
		SetGeometryShader(CompileShader(gs_4_0, DrawGS()));
		SetPixelShader(CompileShader(ps_4_0, DrawPS()));
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState(NoDepthWrites, 0);
	}
}
