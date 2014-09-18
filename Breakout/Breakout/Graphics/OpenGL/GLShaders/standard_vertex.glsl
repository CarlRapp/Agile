#version 420
layout (location = 0) in vec3 m_position;
layout (location = 1) in vec3 m_normal;

uniform mat4 m_matModel;
uniform mat4 m_matView;
uniform mat4 m_matProj;

varying vec3 f_normal;
varying mat4 PVM;

void main(void) 
{
        PVM = m_matProj*m_matView*m_matModel;
        gl_Position = PVM*vec4(m_position, 1.0);
        f_normal = m_normal;
}