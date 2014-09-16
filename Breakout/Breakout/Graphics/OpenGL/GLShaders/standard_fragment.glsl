varying vec3 f_normal;
uniform mat4 m_matModel;
uniform vec3 m_testLight;

void main(void) 
{   
    vec4 A = m_matModel*vec4(f_normal,1.0);

    float b = dot(normalize(m_testLight),vec3(A));

    b = pow(b,2);
    
    gl_FragColor = vec4(b, b, b, 1.0);
    
}