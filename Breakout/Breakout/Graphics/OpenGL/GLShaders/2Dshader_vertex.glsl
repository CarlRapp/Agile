#version 430
layout (location = 0) in vec2 m_position;
layout (location = 1) in vec2 m_texCoord;

out vec2 TexCoord;

void main(void) 
{
    TexCoord = m_texCoord;
    gl_Position = vec4(m_position, 0.0, 1.0);
}
