attribute vec3 coord3d;
attribute vec3 v_color;

uniform mat4 m_matModel;
uniform mat4 m_matView;
uniform mat4 m_matProj;

varying vec3 f_color;

void main(void) 
{
        gl_Position = m_matProj*m_matView*m_matModel*vec4(coord3d, 1.0);
        f_color = v_color;
}