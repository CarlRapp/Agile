#version 430
in vec2 TexCoord;

uniform sampler2D m_texture;

void main(void) 
{   
    vec4 texColor = texture( m_texture, vec2(TexCoord.x,TexCoord.y) );

    gl_FragColor = texColor;
}
