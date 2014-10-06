cbuffer cbPerFrame
{
	float4		gCameraPos;
	float4		gEmitPosW;
	float4		gEmitVelW;
	float		gTime;
	float		gDeltaTime;
	matrix		gView;
	matrix		gProjection;
};


//texturer.
Texture1D randomTex;
Texture2D Texture;

//sampler.
SamplerState g_Sampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

//räkanr ut en slumpmässig normaliserad vektor med hjälp av randomtexturen.
float3 RandUnitVec3(float offset)
{
	// Use game time plus offset to sample random texture.
	float u = (gTime + offset);
	// coordinates in [-1,1]
	float3 v = randomTex.SampleLevel(g_Sampler, u, 0);
	//float3 v = float3(1,1,1);
	// project onto unit sphere
	return normalize(v);
}

//partikelstrukt
struct Particle
{
	float3	initialPosW	: POSITION;
	float3	initialVelW	: VELOCITY;
	float2	size		: SIZE;
	float	age			: AGE;
	uint	type		: TYPE;
};

//streamoutvertexshader.
Particle StreamOutVS(Particle input)
{
	return input;
}

#define PT_EMITTER 0
