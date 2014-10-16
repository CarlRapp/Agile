#include "ParticleSystem.fx"


float aliveTime = 2.0f;

cbuffer cbFixed
{
	float3 gAccelW = {0.0f, 0.0f, 0.0f};		//acceleration
	float2 gParticleSize = float2(2.5f, 2.5f);	//storlek på quad.
	
	//lokala positioner för quaden som skapas i GS.
	float2 gQuadTexC[25][4] =
	{
		//row 1
		{
			float2(0.0f, 0.2f),
			float2(0.2f, 0.2f),
			float2(0.0f, 0.0f),
			float2(0.2f, 0.0f)
		},
		{
			float2(0.2f, 0.2f),
			float2(0.4f, 0.2f),
			float2(0.2f, 0.0f),
			float2(0.4f, 0.0f)
		},
		{
			float2(0.4f, 0.2f),
			float2(0.6f, 0.2f),
			float2(0.4f, 0.0f),
			float2(0.6f, 0.0f)
		},
		{
			float2(0.6f, 0.2f),
			float2(0.8f, 0.2f),
			float2(0.6f, 0.0f),
			float2(0.8f, 0.0f)
		},
		{
			float2(0.8f, 0.2f),
			float2(1.0f, 0.2f),
			float2(0.8f, 0.0f),
			float2(1.0f, 0.0f)
		},
		//row 2
		{
			float2(0.0f, 0.4f),
			float2(0.2f, 0.4f),
			float2(0.0f, 0.2f),
			float2(0.2f, 0.2f)
		},
		{
			float2(0.2f, 0.4f),
			float2(0.4f, 0.4f),
			float2(0.2f, 0.2f),
			float2(0.4f, 0.2f)
		},
		{
			float2(0.4f, 0.4f),
			float2(0.6f, 0.4f),
			float2(0.4f, 0.2f),
			float2(0.6f, 0.2f)
		},
		{
			float2(0.6f, 0.4f),
			float2(0.8f, 0.4f),
			float2(0.6f, 0.2f),
			float2(0.8f, 0.2f)
		},
		{
			float2(0.8f, 0.4f),
			float2(1.0f, 0.4f),
			float2(0.8f, 0.2f),
			float2(1.0f, 0.2f)
		},
		//row 3
		{
			float2(0.0f, 0.6f),
			float2(0.2f, 0.6f),
			float2(0.0f, 0.4f),
			float2(0.2f, 0.4f)
		},
		{
			float2(0.2f, 0.6f),
			float2(0.4f, 0.6f),
			float2(0.2f, 0.4f),
			float2(0.4f, 0.4f)
		},
		{
			float2(0.4f, 0.6f),
			float2(0.6f, 0.6f),
			float2(0.4f, 0.4f),
			float2(0.6f, 0.4f)
		},
		{
			float2(0.6f, 0.6f),
			float2(0.8f, 0.6f),
			float2(0.6f, 0.4f),
			float2(0.8f, 0.4f)
		},
		{
			float2(0.8f, 0.6f),
			float2(1.0f, 0.6f),
			float2(0.8f, 0.4f),
			float2(1.0f, 0.4f)
		},
		//row 4
		{
			float2(0.0f, 0.8f),
			float2(0.2f, 0.8f),
			float2(0.0f, 0.6f),
			float2(0.2f, 0.6f)
		},
		{
			float2(0.2f, 0.8f),
			float2(0.4f, 0.8f),
			float2(0.2f, 0.6f),
			float2(0.4f, 0.6f)
		},
		{
			float2(0.4f, 0.8f),
			float2(0.6f, 0.8f),
			float2(0.4f, 0.6f),
			float2(0.6f, 0.6f)
		},
		{
			float2(0.6f, 0.8f),
			float2(0.8f, 0.8f),
			float2(0.6f, 0.6f),
			float2(0.8f, 0.6f)
		},
		{
			float2(0.8f, 0.8f),
			float2(1.0f, 0.8f),
			float2(0.8f, 0.6f),
			float2(1.0f, 0.6f)
		},
		//row 5
		{
			float2(0.0f, 1.0f),
			float2(0.2f, 1.0f),
			float2(0.0f, 0.8f),
			float2(0.2f, 0.8f)
		},
		{
			float2(0.2f, 1.0f),
			float2(0.4f, 1.0f),
			float2(0.2f, 0.8f),
			float2(0.4f, 0.8f)
		},
		{
			float2(0.4f, 1.0f),
			float2(0.6f, 1.0f),
			float2(0.4f, 0.8f),
			float2(0.6f, 0.8f)
		},
		{
			float2(0.6f, 1.0f),
			float2(0.8f, 1.0f),
			float2(0.6f, 0.8f),
			float2(0.8f, 0.8f)
		},
		{
			float2(0.8f, 0.0f),
			float2(1.0f, 0.0f),
			float2(0.8f, 0.8f),
			float2(1.0f, 0.8f)
		}
	};
};


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
#define PT_FLARE 1


//streamoutgeometryshader
//skickar ut partiklar som en pointstream. max 2 st.
[maxvertexcount(6)]
void StreamOutGS(point Particle input[1], inout PointStream<Particle> ptStream)
{
	input[0].age += gDeltaTime;	//gör partiklen äldre.
	if( input[0].type == PT_EMITTER )//om partiklen är en emit-partikel
	{
		if( input[0].age > 0.033f )	//om det är dags för att skapa nya partiklar.
		{
			for (int i = 0; i < 5; i++)
			{
				float3 velocity = RandUnitVec3(0.001f * i);	//slumpar hastighet.
				
				//minskar hastigheten i x och z-led.
				//velocity.x *= 0.8f;
				//velocity.z *= 0.8f;
				
				//skapar den nya partiklen.
				Particle p;
				p.initialPosW = gEmitPosW;
				p.initialVelW = 4.0f * velocity;
				p.size = gParticleSize;
				p.age = 0.0f;
				p.type = PT_FLARE;
				ptStream.Append(p); //streamar ut partiklen.
			}
			// reset the time to emit
			input[0].age = 0.0f;
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

GeometryShader GS_StreamOut = ConstructGSWithSO(CompileShader( gs_4_0, StreamOutGS() ), "POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x" );

//StreamOutTech har  ingen pixelshader samt depth buffer avaktiverad.
technique11 StreamOutTech
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, StreamOutVS() ) );
		SetGeometryShader( GS_StreamOut );
		SetPixelShader(NULL);
		SetDepthStencilState( DisableDepth, 0 );
	}
}


struct VS_OUT
{
	float3 posW : POSITION;
	float2 size : SIZE;
	float4 color : COLOR;
	uint type : TYPE;
	uint texIndex : TEXINDEX;
};

//drawvertexshader
VS_OUT DrawVS(Particle input)
{
	VS_OUT vOut;
	float t = input.age;
	vOut.posW = 0.5f * t * t * gAccelW + t * input.initialVelW + input.initialPosW;  //räknar ut positionen på partiklen.
	float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / aliveTime);
	opacity = opacity * 0.5f;
	vOut.color = float4(1.0f, 1.0f, 1.0f, opacity);	//gör färgengenomskinlig över tid.
	vOut.size = input.size * (1+( 1.5f * input.age / aliveTime));
	vOut.type = input.type;
	vOut.texIndex = 24 * (input.age / aliveTime);
	return vOut;
}


struct GS_OUT
{
	float4 posH : SV_Position;
	float4 color : COLOR;
	float2 texC : TEXCOORD;
};

// The draw GS just expands points into camera facing quads.
[maxvertexcount(4)]
void DrawGS(point VS_OUT input[1], inout TriangleStream<GS_OUT> triStream)
{
	// do not draw emitter particles.
	if( input[0].type != PT_EMITTER )
	{
		//
		// Räknar ut world-matrisen så att billboarden riktas mot kameran.
		//
		float3 look = normalize(gCameraPos.xyz - input[0].posW);
		float3 right = normalize(cross(float3(0,1,0), look));
		float3 up = cross(look, right);
		float4x4 World;
		World[0] = float4(right, 0.0f);
		World[1] = float4(up, 0.0f);
		World[2] = float4(look, 0.0f);
		World[3] = float4(input[0].posW, 1.0f);
		float4x4 WVP = mul(World, gView); //räknar ut WV.
		WVP = mul(WVP, gProjection); //räknar ut WVP.

		// Räknar ut 4 trianglestrip vertices (quad) i lokala koordinater.
		float halfWidth = 0.5f*input[0].size.x;
		float halfHeight = 0.5f*input[0].size.y;
		float4 v[4];
		v[0] = float4(-halfWidth, -halfHeight, 0.0f, 1.0f);
		v[1] = float4(+halfWidth, -halfHeight, 0.0f, 1.0f);
		v[2] = float4(-halfWidth, +halfHeight, 0.0f, 1.0f);
		v[3] = float4(+halfWidth, +halfHeight, 0.0f, 1.0f);
		
		//Transfomerar koodinaterna till koodinater på skärmen och stremar ut.
		GS_OUT gOut;
		[unroll]
		for(int i = 0; i < 4; ++i)
		{
			gOut.posH = mul(v[i], WVP);
			gOut.texC = gQuadTexC[input[0].texIndex][i];
			gOut.color = input[0].color;
			triStream.Append(gOut);	//streamar ut partiklen.
		}
	}
}

float4 DrawPS(GS_OUT input) : SV_TARGET
{
	return Texture.Sample(g_Sampler, input.texC) * input.color;
}


//Drawtech har med pixelshader.
technique11 DrawTech
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, DrawVS() ) );
		SetGeometryShader( CompileShader( gs_4_0, DrawGS() ) );
		SetPixelShader( CompileShader( ps_4_0, DrawPS() ) );
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState( NoDepthWrites, 0 );
	}
}
