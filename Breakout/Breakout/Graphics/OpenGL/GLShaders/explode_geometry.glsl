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
    vec2    texCoord;

} GSIn[];

out GS_FS
{
    vec4    worldPos;
    vec4    color;
    vec3    normal;
    float   pad;
    vec2    texCoord;

} GSOut;

void main() 
{
    mat4 localMatModel = GSIn[0].matModel;

    vec3 faceEdgeA = vec3(GSIn[1].worldPos - GSIn[0].worldPos);
    vec3 faceEdgeB = vec3(GSIn[2].worldPos - GSIn[0].worldPos);
    vec3 faceNormal = normalize( cross(faceEdgeA, faceEdgeB) );
    vec3 explodeVec = faceNormal;

    for (int i = 0; i < 3; i++) 
    {
        GSOut.normal = GSIn[i].normal;
        GSOut.texCoord = GSIn[i].texCoord;
        GSOut.worldPos = GSIn[i].worldPos + vec4(faceNormal*GSIn[0].explode,0);//explodeVec;

        GSOut.color = GSIn[i].color;

        gl_Position = m_matProj*m_matView*GSOut.worldPos;

        EmitVertex();
    }

    EndPrimitive();
}
