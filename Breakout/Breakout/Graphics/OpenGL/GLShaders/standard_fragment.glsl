#version 420

uniform vec3 m_testLight;

varying mat4 modelView;

in VERTEX
{
    vec3    normal;
    vec4    color;

} vertex;

void main(void) 
{   
    vec4 A = modelView*vec4(vertex.normal,1.0);

    float b = dot(normalize(m_testLight),vec3(A));

    b = pow(b,2);
    
    gl_FragColor = vec4(vertex.normal.x, vertex.normal.y, vertex.normal.z, 1.0);
    
}
