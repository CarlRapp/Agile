#version 430
in vec2 TexCoord;

uniform sampler2D m_texture;

void main(void) 
{   
    //result = g_Texture.Sample(g_Sampler, input.Tex);
    vec4 texColor = texture( m_texture, TexCoord );

    gl_FragColor = texColor;
}
