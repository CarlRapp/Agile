#version 430
uniform float roll;

writeonly uniform image2D destTex;

layout (local_size_x = 16, local_size_y = 16) in;

struct Sphere{
    vec4    position;
    float   radius;
};

struct Ray{
    vec3    origin;
    vec3    dir;
};

float hitSphere(Ray r, Sphere s);

void main() 
{
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;

    Ray ray = {vec3(0,0,0), vec3(x,y, 1000)};   

    float r = 0.0;
    float g = 0.0;
    float b = 0.2;

    //vec2 coord = vec2(x/512,y/512);

    Sphere sp ={vec4(128, 128, 512, 1), 25.0f};

    //float t = hitSphere(ray, sp);
    
    float t = hitSphere(ray, sp);
    r = t;
    g = t;
    b = t;

    imageStore(destTex, ivec2(x,y), vec4(r, g, b, 1.0));


}

float hitSphere(Ray r, Sphere s)
{

    float s_vv = dot(r.dir, r.dir);
    float s_ov = dot(r.origin, r.dir);
    float s_mv = dot(s.position.xyz, r.dir);
    float s_mm = dot(s.position.xyz, s.position.xyz);
    float s_mo = dot(s.position.xyz, r.origin);
    float s_oo = dot(r.origin, r.origin);

    float d = s_ov*s_ov-2*s_ov*s_mv+s_mv*s_mv-s_vv*(s_mm-2*s_mo*s_oo-s.radius*s.radius);

    if(d < 0){
        return -1.0f;
    } else if(d == 0){
        return (s_mv-s_ov)/s_vv;
    } else {
        float t1 = 0, t2 = 0;
        t1 = s_mv-s_ov;

        t2 = (t1-sqrt(d))/s_vv;
        t1 = (t1+sqrt(d))/s_vv;

        return t1>t2? t2 : t1 ; 
    }
}


