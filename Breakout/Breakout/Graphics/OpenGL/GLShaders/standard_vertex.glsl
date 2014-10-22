#version 430
layout (location = 0) in vec3   m_position;
layout (location = 1) in float  pad1;
layout (location = 2) in vec3   m_normal;
layout (location = 3) in float  m_explosion;
//layout (location = 4) in vec4   m_color;
layout (location = 4) in mat4   m_matModel;

layout (location = 8) in vec2   m_texCoord;
//layout (location = 9) in vec2   pad2;


uniform mat4 m_matView;
uniform mat4 m_matProj;

varying mat4 modelView;

out VS_GS
{
    vec4    worldPos;
    vec3    normal;
    float   explode;
    vec4    color;
    mat4    matModel;
    vec2    texCoord;
    vec2    pad;
} vertex;


void main(void) 
{
    mat3 normalMatrix = transpose(inverse(mat3(m_matModel)));
    vertex.worldPos = m_matModel * vec4(m_position, 1.0);
    vertex.normal = normalize(normalMatrix * m_normal);  //mat3(m_matModel) * m_normal;
    vertex.texCoord = m_texCoord;

    //vertex.normal = mat3(m_matModel) * m_normal;

    gl_Position = m_matProj*m_matView*m_matModel* vec4(m_position, 1.0);

    //vertex.color = m_color;
    vertex.explode = m_explosion;
    vertex.matModel = m_matModel;
}
