#version 430
layout (location = 0) in vec2 in_position;
layout (location = 1) in vec2 in_texCoord;

uniform int m_letter;

out vec2 texCoord;

void main(void) 
{
    texCoord = in_texCoord;

    if(in_texCoord.x == 0.0)
        texCoord.x = m_letter/59.0;
    else
        texCoord.x = (m_letter+1)/59.0;

    
    texCoord.y = 1-in_texCoord.y;

    gl_Position = vec4(in_position, 0.0, 1.0);
}
