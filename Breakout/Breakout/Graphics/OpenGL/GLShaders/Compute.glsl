#version 430

writeonly uniform image2D destTex;

uniform vec3 m_testLight;

varying mat4 modelView;

uniform sampler2D m_standardTex;

const int nrOfLights = 5;

struct LightInfo
{
	vec4 Position; // Light position world coords.
	vec3 Intensity; // La, Ld and Ls intensity
	vec3 Color;
	float Range;
};
uniform LightInfo Lights[nrOfLights];

struct MaterialInfo{
	vec3 Ka;			// Ambient reflectivity
	vec3 Kd;		    // Diffuse reflectivity
	vec3 Ks;			// Specular reflectivity
	float Shininess;	// Specular shininess factor
};
MaterialInfo Material;

struct Model
{
    vec3[132]    pos;
    //vec4[132]    color;
    vec3[132]    normal;
    vec2[132]    texCoord;

    mat4 modelMatrix;

};

uniform vec3[132] testArray;

uniform Model Models[5];

uniform mat4 m_matView;
uniform mat4 m_matProj;

layout (local_size_x = 32, local_size_y = 32) in;

struct Sphere{
    vec4    position;
    float   radius;
};

struct Ray{
    vec3    origin;
    vec3    dir;
};

struct Vertex
{
    vec4    worldPos;
    vec4    color;
    vec3    normal;
    float   pad;
    vec2    texCoord;

};

struct Triangle
{
    Vertex vx1;
    Vertex vx2;
    Vertex vx3;
};

vec4 traceColor(Vertex vertex);
vec4 intersect(Ray r, Triangle t,mat4 modelMatrix);
vec3 phongModelDiffAndSpec(int index, vec3 lightPos, float strength, Vertex vertex);
vec4 traceColor(Vertex vertex);

void main() 
{
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;

    Ray ray = {vec3(0,0,0), vec3(x,y, 1000)};   

    uint i = 0;

    float r = 0;
    float g = 0;
    float b = 0;
    float a = 1.0;

for(i = 0; i < 132;i++)
{
    Triangle tri;
    Vertex vx1;
    vx1.worldPos = vec4(Models[0].pos[i],1);
    vx1.normal = Models[0].normal[i];
    vx1.texCoord = Models[0].texCoord[i];

    Vertex vx2;
    vx2.worldPos = vec4(Models[0].pos[i+1],1);
    vx2.normal = Models[0].normal[i+1];
    vx2.texCoord = Models[0].texCoord[i+1];

    Vertex vx3;
    vx3.worldPos = vec4(Models[0].pos[i+2],1);
    vx3.normal = Models[0].normal[i+2];
    vx3.texCoord = Models[0].texCoord[i+2];

    tri.vx1 = vx1;
    tri.vx2 = vx2;
    tri.vx3 = vx3;

    vec4 t = intersect(ray,tri,Models[0].modelMatrix);

    r = r+ t.x;
    g = g+t.y;
    b = b+t.z;
}
/*
    float r = testArray[1].x;
    float g = testArray[2].y;
    float b = testArray[3].z;*/

    //vec2 coord = vec2(x/512,y/512);

    //Sphere sp ={vec4(128, 128, 512, 1), 25.0f};

    //float t = hitSphere(ray, sp);
    
    // = hitSphere(ray, sp);
/*
for(int i = 0; i < 36;i+=3)
    t += intersect( ray,
                    Models[0].vertex[i],
                    Models[0].vertex[i+1],
                    Models[0].vertex[i+2]);

    r = t.x;
    g = t.y;
    b = t.z;
    a = t.w;*/
    

    imageStore(destTex, ivec2(x,y), vec4(r, g, b, a));
}

vec3 phongModelDiffAndSpec(int index, vec3 lightPos, float strength, Vertex vertex) 
{

    //tmp material
    Material.Ka = vec3(0.2);
    Material.Kd = vec3(0.9);
    Material.Ks = vec3(0.9);
    Material.Shininess = 30.0;

    vec3 s = normalize(vec3(lightPos - vertex.worldPos.xyz));
    vec3 v = normalize(-vertex.worldPos.xyz);	
    //vec3 h = normalize(v + s);
    vec3 h = reflect( s, vertex.normal);

    float sDotN = max( dot(s, vertex.normal), 0.0 );

    //Diffuse light
    vec3 diffuse = vec3(Lights[index].Intensity.y) * Lights[index].Color * Material.Kd * sDotN * strength;

    vec3 Ks = Material.Ks;

    vec3 specular = vec3(0.0);

    //Specular light
    if( sDotN > 0.0 )
            specular = vec3(Lights[index].Intensity.z) * Lights[index].Color * Ks * pow( max( dot(h,v), 0.0 ), Material.Shininess ) * strength;

    return vec3(diffuse);// + specular);
}

vec4 traceColor(Vertex vertex)
{
       vec3 ambient = vec3(0.0f); // = vec3(0.2f,0.2f,0.2f);
    vec3 diffAndSpec = vec3(0.0);

    for(int index = 0; index < nrOfLights; index++)
    {
        float lightDist = abs(length(vertex.worldPos.xyz - vec3(Lights[index].Position)));
        float lightStrength = 0;
        if( lightDist < Lights[index].Range )
			lightStrength = 1.0-(lightDist / Lights[index].Range);
        //för varje ljus
	diffAndSpec += phongModelDiffAndSpec(index, vec3(Lights[index].Position), lightStrength,vertex);
    }

    //vec4 texColor = texture( m_standardTex, vertex.texCoord );
    
    return vec4(diffAndSpec + ambient, 1.0);// * texColor;
    //return vec4(0.5,0.5,0.5, 1.0);
}

vec4 intersect(Ray r, Triangle tri,mat4 modelMatrix)
{
       vec3 v0 = vec3(m_matProj*m_matView*modelMatrix* tri.vx1.worldPos);
       vec3 v1 = vec3(m_matProj*m_matView*modelMatrix* tri.vx2.worldPos);
       vec3 v2 = vec3(m_matProj*m_matView*modelMatrix* tri.vx3.worldPos);

        vec3 v0v1 = v1 - v0;
        vec3 v0v2 = v2 - v0;
        vec3 N = cross(v0v1, v0v2);
        float nDotRay = dot(N, r.dir);
       if (dot(N, r.dir) == 0) 
            return vec4(0,0,0,0); 
        // ray parallel to triangle 
        float d = dot(N, v0);
        float t = -(dot(N, r.origin) + d) / nDotRay;
        // inside-out test 
        vec3 Phit = r.dir*t + r.origin;
        // inside-out test edge0 
        vec3 v0p = Phit - v0;
        float v = dot(N, cross(v0v1, v0p));
        if (v < 0) 
           return vec4(0,0,0,0); 
         // P outside triangle 

        // inside-out test edge1 
        vec3 v1p = Phit - v1; 
        vec3 v1v2 = v2 - v1; 
        float w = dot(N, cross(v1v2, v1p)); 
        if (w < 0) 
            return vec4(0,0,0,0); 
        // P outside triangle 

        // inside-out test edge2 
        vec3 v2p = Phit - v2; 
        vec3 v2v0 = v0 - v2; 
        float u = dot(N, cross(v2v0, v2p)); 
        if (u < 0) 
            return vec4(0,0,0,0);
         //P outside triangle 

        vec4 tc = vec4(1,1,1,1);//traceColor(tri.vx1) + traceColor(tri.vx2) + traceColor(tri.vx3);

    return tc;
} 
