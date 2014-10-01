#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 m_matView;
uniform mat4 m_matProj;

varying mat4 modelView;

in VS_GS
{
    vec4    worldPos;
    vec3    normal;
    float   explode;
    vec4    color;
    mat4    matModel;

} GSIn[];

out GS_FS
{
    vec4    worldPos;
    vec4    color;
    vec3    normal;
    float   pad;

} GSOut;

void main() 
{
    mat4 localMatModel = GSIn[0].matModel;

    vec3 faceEdgeB = vec3(GSIn[1].worldPos - GSIn[0].worldPos);
    vec3 faceEdgeA = vec3(GSIn[2].worldPos - GSIn[0].worldPos);
    vec3 faceNormal = normalize( cross(faceEdgeA, faceEdgeB) );
    vec3 explodeVec = faceNormal;

    for (int i = 0; i < 3; i++) 
    {
        GSOut.normal = GSIn[i].normal;
        GSOut.worldPos = GSIn[i].worldPos + vec4(faceNormal*GSIn[0].explode,0);//explodeVec;

        GSOut.color = GSIn[i].color;

        gl_Position = m_matProj*m_matView*localMatModel*GSOut.worldPos;

        EmitVertex();
    }

    EndPrimitive();
}


/*[maxvertexcount(3)]
void GS_EXPLODE(triangle VSOut input[3],uint primID:SV_PrimitiveID,inout TriangleStream<VSOut> TriStream)
{
	VSOut output;
    
    //
    // Calculate the face normal
    //
    float3 faceEdgeA = input[1].PosL - input[0].PosL;
    float3 faceEdgeB = input[2].PosL - input[0].PosL;
    float3 faceNormal = normalize( cross(faceEdgeA, faceEdgeB) );
    float3 ExplodeAmt = faceNormal*mExplodeTimePassed;
    
    //
    // Calculate the face center
    //
    float3 centerPos = (input[0].PosL.xyz + input[1].PosL.xyz + input[2].PosL.xyz)/3.0;
    float2 centerTex = (input[0].Tex + input[1].Tex + input[2].Tex)/3.0;
    centerPos += faceNormal*mExplodeTimePassed;
    
	output.Diffuse = float4(0,0,0,0);//input[0].Diffuse;
	output.Specular = float4(1,1,1,1);//input[0].Specular;
	output.TextureInfo = float4(input[0].TextureInfo.xyz + input[1].TextureInfo.xyz + input[2].TextureInfo.xyz,1.0)/3.0;
	output.ShadowPosH = float4(input[0].ShadowPosH.xyz + input[1].ShadowPosH.xyz + input[2].ShadowPosH.xyz,1.0)/3.0;
	output.SsaoPosH = float4(input[0].SsaoPosH.xyz + input[1].SsaoPosH.xyz + input[2].SsaoPosH.xyz,1.0)/3.0;

	float4 gravity = float4(0,-1.05,0,0)*mExplodeTimePassed;

    for( int i=0; i<3; i++ )
    {
        output.PosL = input[0].PosL + float4(ExplodeAmt,0)+gravity;
		output.PosH = mul(float4(output.PosL,1.0), gWVP );
        output.NormalH = input[0].NormalH;
        output.Tex = input[0].Tex;
        TriStream.Append( output );

		output.PosL = input[1].PosL + float4(ExplodeAmt,0)+gravity;
		output.PosH = mul(float4(output.PosL,1.0), gWVP );
        output.NormalH = input[1].NormalH;
        output.Tex = input[1].Tex;
        TriStream.Append( output );

		output.PosL = input[2].PosL + float4(ExplodeAmt,0)+gravity;
		output.PosH = mul(float4(output.PosL,1.0), gWVP );
        output.NormalH = input[2].NormalH;
        output.Tex = input[2].Tex;
        TriStream.Append( output );
        
        TriStream.RestartStrip();
    }
}*/

