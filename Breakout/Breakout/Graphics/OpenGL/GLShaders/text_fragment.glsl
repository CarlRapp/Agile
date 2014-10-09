#version 430
in vec2 texCoord;

uniform sampler2D m_texture;

uniform vec3 m_color;

void main(void) 
{   
    vec4 texColor = texture( m_texture, texCoord );

    gl_FragColor = vec4(texColor.x*m_color.x,texColor.x*m_color.y,texColor.x*m_color.z,texColor.x);
}
