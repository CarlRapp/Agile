#version 430

writeonly uniform image2D destTex;

uniform vec3 m_testLight;

varying mat4 modelView;

uniform sampler2D m_standardTex;

const int nrOfLights = 2;

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
};

uniform Model Models[5];

uniform mat4 m_matView;
uniform mat4 m_matProj;

layout (local_size_x = 16, local_size_y = 16) in;

struct Sphere
{
    vec4    position;
    float   radius;
};

struct SplitRay
{
    vec3    origin;
    vec3    dir;
    uint    bounce;
    vec4    color;
};

struct Ray
{
    vec3    origin;
    vec3    dir;
    uint    bounce;
    vec4    color;
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

struct BounceData
{
    float t;
    vec4 color;
};

//VARIABLES
float scale = 10;
mat4 modelMatrix[2] =   {
                                mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1),
                                mat4(scale,0,0,0,0,scale,0,0,0,0,scale,0,0,0,0,1)
                        };


const float widthHalf = 256.0;
const float heightHalf = 256.0;
uniform vec3 camPos;
const vec3 ambient = vec3(0.0,0.0,0.0);
const int BOUNCE_LIMIT = 2;

//METHODS
vec4 traceColor(Vertex vertex);
float intersect(Ray r, Triangle t);
vec3 phongModelDiffAndSpec(int index, vec3 lightPos, float strength, Vertex vertex);
vec4 traceColor(Vertex vertex);
Ray trace(Ray ray);

void main() 
{
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;

    //send ray from camera eye (camPos) with directions x,y,1
    vec4 rayPos;

    //
    //
    //vs_out.ray_direction = (viewmat * vec4(pos.xyz * direction_scale * vec3(1.0, aspect, 2.0) + direction_bias, 0.0)).xyz;
    //camPos = vec3(m_matView[3][0],m_matView[3][1],m_matView[3][2]);

    rayPos.x    = x/widthHalf -1.0;
    rayPos.y    = y/heightHalf -1.0;
    rayPos.z    = 1;
    rayPos.w    = 1;
    
    rayPos *= m_matProj;//*inverse(m_matProj)*rayPos;
    //rayPos /= rayPos.w;
    rayPos *= m_matView;
    //rayPos = vec4(camPos - rayPos.xyz,1);

    vec3 rayDir = normalize(vec3(rayPos));


    Ray ray = {camPos, rayDir,0,vec4(0,0,0,0)};   

    Ray rayOut = trace(ray);
    
    vec4 color = vec4(0,0,0,1);
    
    color += rayOut.color;

    //color = vec4(camPos,1);

    /*if(rayOut.isSplit)
    {
        Ray splitRay = {rayOut.split.origin,rayOut.split.dir,0,rayOut.split.color,sr,true};

        color += trace(splitRay).color;
    }*/



   // imageStore(destTex, m_matProj*m_matView*vec4(x,y,1,1)ivec2(x,y), color);
    imageStore(destTex, ivec2(x,y), color);
}

Ray trace(Ray ray)
{
    uint i=0;
    uint j=0;
    float t = 10000;

    Vertex hit;

    for(int k=0; ray.bounce < BOUNCE_LIMIT;ray.bounce++)
    {
        for(j=0; j < 2;j++)
        {
            for(i=0; i < 132;i +=3)
            {
                Triangle tri;
                Vertex vx1;
                vx1.worldPos = modelMatrix[j]*vec4(Models[0].pos[i],1);
                vx1.normal = Models[0].normal[i];
                vx1.texCoord = Models[0].texCoord[i];

                Vertex vx2;
                vx2.worldPos = modelMatrix[j]*vec4(Models[0].pos[i+1],1);
                vx2.normal = Models[0].normal[i+1];
                vx2.texCoord = Models[0].texCoord[i+1];

                Vertex vx3;
                vx3.worldPos = modelMatrix[j]*vec4(Models[0].pos[i+2],1);
                vx3.normal = Models[0].normal[i+2];
                vx3.texCoord = Models[0].texCoord[i+2];

                tri.vx1 = vx1;
                tri.vx2 = vx2;
                tri.vx3 = vx3;

                float r = intersect(ray,tri);

                if(r > 0 && r < t)
                {
                    t = r;
                    hit.worldPos = vec4(vec3(ray.dir*t+ray.origin),1);
                    hit.normal = vx1.normal;
                    
                }
            }
        }

        if(t > 0 && t != 10000)
        {
            ray.color += traceColor(hit);

            ray.origin = hit.worldPos.xyz;
            ray.dir = reflect(ray.dir,hit.normal);
        }
        else
            break;
    }

    return ray; 
}

vec3 phongModelDiffAndSpec(int index, vec3 lightPos, float strength, Vertex vertex) 
{

    //tmp material
    Material.Ka = vec3(0.5);
    Material.Kd = vec3(0.6);
    Material.Ks = vec3(0.9);
    Material.Shininess = 50.0;

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

    return vec3(diffuse +specular+ ambient);
}

vec4 traceColor(Vertex vertex)
{
    vec3 diffAndSpec = vec3(0.0);

    for(int index = 0; index < nrOfLights; index++)
    {
        float lightDist = abs(length(vertex.worldPos.xyz - vec3(Lights[index].Position)));
        float lightStrength = 0;
        //if( lightDist < Lights[index].Range )
			lightStrength = 1.0;//-(lightDist / Lights[index].Range);

	diffAndSpec += phongModelDiffAndSpec(index, vec3(Lights[index].Position), lightStrength,vertex);
    }

    //vec4 texColor = texture( m_standardTex, vertex.texCoord );
    
    return vec4(diffAndSpec + ambient, 1.0);// * texColor;
}

float intersect(Ray ray, Triangle tri)
{
    Vertex vertex1 = tri.vx1;
    Vertex vertex2 = tri.vx2;
    Vertex vertex3 = tri.vx3;

    vec3 e1 = vertex2.worldPos.xyz-vertex1.worldPos.xyz;
    vec3 e2 = vertex3.worldPos.xyz-vertex1.worldPos.xyz;
    vec3 q = cross(ray.dir.xyz, e2);
    float a = dot(e1, q);

    if(a > -0.000001 && a < 0.000001)
    {
        return 0;
    }

    float f = 1.0 / a;
    vec3 s = ray.origin.xyz - vertex1.worldPos.xyz;
    float u = f*(dot(s, q));

    if(u < 0 || u > 1.0f)
    {
        return 0;
    }
    vec3 r = cross(s, e1);
    float v = f*(dot(ray.dir.xyz, r));

    if(v < 0 || (u + v) > 1) 
    {
        return 0;
    }
    float t = f*(dot(e2, r));

    if(t < 0)
    {
        return 0;
    }

    return t;
    //return vec4(vertex1.normal,1);
} 


