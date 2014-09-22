#version 420
layout (location = 0) in vec3 m_position;
layout (location = 1) in float pad1;
layout (location = 2) in vec3 m_normal;
layout (location = 3) in float pad2;
layout (location = 4) in vec4 m_color;
layout (location = 5) in mat4 m_matModel;


uniform mat4 m_matView;
uniform mat4 m_matProj;

varying mat4 modelView;

out VERTEX
{
   vec3    worldPos;
   vec3    normal;
   vec4    color;
} vertex;


void main(void) 
{
    mat3 normalMatrix = transpose(inverse(mat3(m_matModel)));
    vertex.worldPos = mat3(m_matModel) * m_position;
    vertex.normal = normalize(normalMatrix * m_normal);

    //modelView = m_matView * m_matModel;

    gl_Position = m_matProj*m_matView*m_matModel* vec4(m_position, 1.0);

    // Transform the normal by the upper-left-3x3-submatrix of the
    // model-view matrix
    //vertex.normal = mat3(modelView) * m_normal; //fel normalberäkning??

    // Pass the per-instance color through to the fragment shader.
    vertex.color = m_color;
}
