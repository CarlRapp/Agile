#version 430

writeonly uniform image2D m_destTex;

//textureslots
//0 = kub i mitten
//1 = kub runtom
//2 = sfär
uniform sampler2D   m_objectTex[3];

uniform mat4        m_matView;
uniform mat4        m_matProj;
uniform vec3        m_camPos;
uniform vec3        m_camDir;
uniform bool        m_mouseClick;

//has to be same as GLGraphics.cpp COMPUTE_WORKGROUPS
layout (local_size_x = 16, local_size_y = 16) in;

struct LightInfo
{
	vec4 Position; // Light position world coords.
	vec3 Intensity; // La, Ld and Ls intensity
	vec3 Color;
	float Range;
};
const int LIGHTS = 10;
uniform LightInfo   m_lights[LIGHTS];

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
    vec3[132]    normal;
    vec2[132]    texCoord;
    vec3         minimum;
    vec3         maximum;
};
uniform Model       m_models[1];


struct Sphere
{
    vec4    position;
    float   radius;
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
    vec3    normal;
    vec2    texCoord;
};

struct Triangle
{
    Vertex vx1;
    Vertex vx2;
    Vertex vx3;
};

//VARIABLES
//modelmatrices with only scale, to use small box and big surrounding one.
const mat4 modelMatrix[2] = {
                                    mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1),
                                    mat4(10,0,0,0,0,10,0,0,0,0,10,0,0,0,0,1)
                            };
Sphere m_spheres[10]  =   {
                            //this is the "player"
                            {vec4(0,0,0,0),0.3},
                            //LIGHTS
                            {vec4(0,0,0,0),0.1},
                            {vec4(0,0,0,0),0.1},
                            {vec4(0,0,0,0),0.1},
                            {vec4(0,0,0,0),0.1},
                            {vec4(0,0,0,0),0.1},
                            {vec4(0,0,0,0),0.1},
                            {vec4(0,0,0,0),0.1},
                            {vec4(0,0,0,0),0.1},
                            {vec4(0,0,0,0),0.1},
                        };

                            


uniform float m_width;
uniform float m_height;

shared vec4[gl_WorkGroupSize.x][gl_WorkGroupSize.y] warpColor;
shared vec3[gl_WorkGroupSize.x][gl_WorkGroupSize.y] warpNormal;
shared float[gl_WorkGroupSize.x][gl_WorkGroupSize.y] warpT;

uint m_myX = gl_LocalInvocationID.x;
uint m_myY = gl_LocalInvocationID.y;

//METHODS
vec4 traceColor(Vertex vertex,int texSlot);
float intersect(Ray ray,Vertex vx1,Vertex vx2,Vertex vx3);
vec3 phongModelDiffAndSpec(int index, vec3 lightPos, float strength, Vertex vertex);
vec4 trace(Ray ray);
vec4 hitSphere(Sphere s, Ray r);
bool hitBox(Ray r, vec3 _max, vec3 _min, mat4 _scale);
float shadowTrace(Vertex hit);
float flashLight(Vertex hit,Ray ray);

#define VERTICES 132
#define BOUNCES 1
//#define SAMPLES 3
//#define SUPER_SAMPLE
#define FLASHLIGHT

#define AABB
#define SPHERES
#define MODELS

//define SHADOWS
#define TEXTURES
#define SHADOW_LIGHTS 1
//#define USE_TEXSLOTS

void main() 
{
    //send ray from camera eye (m_camPos) with directions in worldspace
    vec4 rayPos;

    rayPos.x    = gl_GlobalInvocationID.x/(m_width*0.5) -1.0;
    rayPos.y    = gl_GlobalInvocationID.y/(m_height*0.5) -1.0;
    rayPos.z    = 1;
    rayPos.w    = 1;
    
    rayPos *= m_matProj;
    rayPos *= m_matView;

    vec3 rayDir = normalize(vec3(rayPos));

    Ray ray = {m_camPos, rayDir,0,vec4(0,0,0,0)};   

    m_spheres[0].position.xyz = m_camPos;

#ifdef SPHERES
    for(int i =2; i < 10;i++)
    {
        m_spheres[i].position = m_lights[i].Position;
    }
#endif

#ifdef SUPER_SAMPLE

    uint RIGHT = m_myX+1;
    uint LEFT = m_myX-1;
    uint UP = m_myY+1;
    uint DOWN = m_myY-1;

    if(RIGHT >= gl_WorkGroupSize.x)
        RIGHT = gl_WorkGroupSize.x-1;

    //uint kan inte testa mot -1, slår runt och hamnar på maxint, därför ej: if <0
    if(LEFT > gl_WorkGroupSize.x)
        LEFT = 0;
    if(DOWN > gl_WorkGroupSize.y)
        DOWN = 0;

    if(UP >= gl_WorkGroupSize.y)
        UP = gl_WorkGroupSize.y-1;

    warpColor[m_myX][m_myY] = trace(ray);

    barrier();
    
    float b1,b2,b3,b4,b5,b6,b7,b8,b9;

    float bias = 3;
    float p = 20;

    //bias väger in omliggande mer än träffad vertex, p gör att pixlar långt bort från träffen betyder mindre.
    b1=bias/(warpT[RIGHT][m_myY]*p);
    b2=bias/(warpT[RIGHT][UP]*p);
    b3=bias/(warpT[RIGHT][DOWN]*p);
    b4=bias/(warpT[LEFT][m_myY]*p);
    b5=bias/(warpT[LEFT][UP]*p);
    b6=bias/(warpT[LEFT][DOWN]*p);
    b7=bias/(warpT[m_myX][UP]*p);
    b8=bias/(warpT[m_myX][DOWN]*p);
    b9=1.0;

    for(int i=0; i < SAMPLES;i++)
    {
        warpColor[m_myX][m_myY] = ( warpColor[RIGHT][m_myY]*b1+
                                warpColor[RIGHT][UP]*b2+
                                warpColor[RIGHT][DOWN]*b3+

                                warpColor[LEFT][m_myY]*b4+
                                warpColor[LEFT][UP]*b5+
                                warpColor[LEFT][DOWN]*b6+

                                warpColor[m_myX][UP]*b7+
                                warpColor[m_myX][DOWN]*b8+
                                warpColor[m_myX][m_myY]*b9)/(b1+b2+b3+b4+b5+b6+b7+b8+b9);
        barrier();
    }
    imageStore(m_destTex, ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y), warpColor[m_myX][m_myY]);
    #else
    imageStore(m_destTex, ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y), trace(ray));
#endif

    
}

vec4 trace(Ray ray)
{
    bool invert = false;
    int texSlot = 0;

    for(uint k=0; ray.bounce < BOUNCES;ray.bounce++)
    {
        Vertex hit;
        float   saveT=10000;

#ifdef MODELS
        //MODELS
        for(int j=0; j < 2;j++)
        {
#ifdef AABB
            //testa mot minmax AABB
            if(hitBox(ray,m_models[0].maximum,m_models[0].minimum,modelMatrix[j]))
            {
#endif
                for(int i=0; i < VERTICES;i +=3)
                {
                    Vertex vx1;
                    vx1.worldPos = modelMatrix[j]*vec4(m_models[0].pos[i],1);
                    vx1.normal = m_models[0].normal[i];
                    vx1.texCoord = m_models[0].texCoord[i];

                    Vertex vx2;
                    vx2.worldPos = modelMatrix[j]*vec4(m_models[0].pos[i+1],1);
                    vx2.normal = m_models[0].normal[i+1];
                    vx2.texCoord = m_models[0].texCoord[i+1];

                    Vertex vx3;
                    vx3.worldPos = modelMatrix[j]*vec4(m_models[0].pos[i+2],1);
                    vx3.normal = m_models[0].normal[i+2];
                    vx3.texCoord = m_models[0].texCoord[i+2];

                    if(invert)
                    {
                        vx1.normal = -vx1.normal;
                        vx2.normal = -vx2.normal;
                        vx3.normal = -vx3.normal;
                    }

                    float r = intersect(ray,vx1,vx2,vx3);

                    if(r > 0.01 && r < saveT)
                    {
                        saveT = r;

                        hit.worldPos = vec4(ray.dir*saveT+ray.origin,1);
                        hit.normal = vx1.normal;

                        #ifdef TEXTURES
                        //Kör cross på alla för att få ut totala arean, därefter ta fram 3 subtrianglar och jämför deras areor med totala.
                        //Då får man en ratio och vet hur mycket av varje texcoord som ska användas.
                        //http://answers.unity3d.com/questions/383804/calculate-uv-coordinates-of-3d-point-on-plane-of-m.html
                        float a = 1 / length(cross(vx1.worldPos.xyz-vx2.worldPos.xyz, vx1.worldPos.xyz-vx3.worldPos.xyz)); 
                        float a1 = length(cross(vx2.worldPos.xyz-hit.worldPos.xyz, vx3.worldPos.xyz-hit.worldPos.xyz)) * a; 
                        float a2 = length(cross(vx3.worldPos.xyz-hit.worldPos.xyz, vx1.worldPos.xyz-hit.worldPos.xyz)) * a; 
                        float a3 = length(cross(vx1.worldPos.xyz-hit.worldPos.xyz, vx2.worldPos.xyz-hit.worldPos.xyz)) * a; 

                        hit.texCoord = vx1.texCoord * a1 + vx2.texCoord * a2 + vx3.texCoord * a3;
                        #ifdef USE_TEXSLOTS
                        texSlot = j;
                        #endif
                        #endif
                    }
                }
#ifdef AABB
            }
#endif
            //fulhax för att man befinner sig i model[1], stor kub
            invert = true; 
        }
#endif
#ifdef SPHERES
        //SPHERES
        for(int j=0;j < 10;j++)
        {
            vec4 sData = hitSphere(m_spheres[j],ray);

            if(sData.w > 0.01 && sData.w < saveT)
            {
                saveT = sData.w;
                hit.worldPos = vec4(vec3(ray.dir*sData.w+ray.origin),1);
                hit.normal = normalize(sData.xyz);
                #ifdef TEXTURES
                
                    #ifdef USE_TEXSLOTS
                    hit.texCoord = vec2(asin(hit.normal.x)/3.14 + 0.5,asin(hit.normal.y)/3.14 + 0.5);
                    texSlot = 2;
                    #else
                    //istället för annan texturslot, spara bilden i existerande textur och ta fram x,y i paint (0,0)(95,47) i denna
                    //
                    //  arcsin(x)/(pi*storlek/koordinat)+koordinat/storlek/2+index 
                    //
                    hit.texCoord = vec2(asin(hit.normal.x)/(3.14*256.0/95.0) + 95.0/256.0/2.0,-asin(hit.normal.y)/(3.14*256.0/47.0) + 47.0/256.0/2.0);
                    #endif
                
                #endif
            }
        }
#endif

        #ifdef SUPER_SAMPLE
        warpT[m_myX][m_myY] = saveT;
        #endif

        if(saveT > 0 && saveT != 10000)
        {
            ray.color += traceColor(hit,texSlot);

            #ifdef SHADOWS
            ray.color *= shadowTrace(hit);
            #endif

            #ifdef FLASHLIGHT
            if(m_mouseClick)
            ray.color += flashLight(hit,ray);
            #endif

            ray.origin = hit.worldPos.xyz;
            ray.dir = reflect(ray.dir,hit.normal);
        }
    }
    return ray.color; 
}


//http://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
vec4 hitSphere(Sphere s,Ray r)
{
    vec3    C = s.position.xyz;
    vec3    D = r.dir.xyz;
    vec3    O = r.origin.xyz;
    float   R = s.radius;

    float root = dot(D,(O-C))*dot(D,(O-C)) - pow(length(O - C),2)+pow(R,2);

    //no solution exists
    if(root < 0)
        return vec4(0,0,0,0);

    float x = (sqrt(root));

    float a = -dot(D,(O-C));

    float d1 = a + x;
    float d2 = a - x;

    //pick closest intersection
    float t = min(d1,d2);

    vec3 normal= ((O+D*t)-(C))/R;

    return vec4(normal,t);
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
    vec3 h = reflect( s, vertex.normal);

    float sDotN = max( dot(s, vertex.normal), 0.0 );
    //float sDotN = abs(dot(s, vertex.normal));

    //Diffuse light
    vec3 diffuse = vec3(m_lights[index].Intensity.y) * m_lights[index].Color * Material.Kd * sDotN * strength;

    vec3 Ks = Material.Ks;

    vec3 specular = vec3(0.0);

    //Specular light
    if( sDotN > 0.0 )
            specular = vec3(m_lights[index].Intensity.z) * m_lights[index].Color * Ks * pow( max( dot(h,v), 0.0 ), Material.Shininess ) * strength;

    return vec3(diffuse +specular);
}

vec4 traceColor(Vertex vertex,int texSlot)
{
    vec3 diffAndSpec = vec3(0.0);

    for(int index = 0; index < LIGHTS; index++)
    {
        float lightDist = abs(length(vertex.worldPos.xyz - vec3(m_lights[index].Position)));
        float lightStrength = 0;
        if( lightDist < m_lights[index].Range )
			lightStrength = 1.0-(lightDist / m_lights[index].Range);

	diffAndSpec += phongModelDiffAndSpec(index, vec3(m_lights[index].Position), lightStrength,vertex);
    }
    #ifdef TEXTURES
    vec4 texColor = vec4(1,1,1,1);

    texColor = texture( m_objectTex[texSlot], vertex.texCoord );
    //return vec4(vertex.texCoord.xy,0,1);
    return vec4(diffAndSpec, 1.0) * texColor;
    #else
    return vec4(diffAndSpec, 1.0);
    #endif
}

float intersect(Ray ray,Vertex vx1,Vertex vx2,Vertex vx3)
{
    vec3 e1 = vx2.worldPos.xyz-vx1.worldPos.xyz;
    vec3 e2 = vx3.worldPos.xyz-vx1.worldPos.xyz;
    vec3 q = cross(ray.dir.xyz, e2);
    float a = dot(e1, q);

    //parallel
    if(a ==0)
    {
        return 0;
    }

    //instead of 3 divide with a, save f to use multiplication
    float f = 1.0 / a;

    vec3 s = ray.origin.xyz - vx1.worldPos.xyz;
    float u = f*(dot(s, q));

    //side
    if(u < 0 || u > 1.0)
    {
        return 0;
    }
    vec3 r = cross(s, e1);
    float v = f*(dot(ray.dir.xyz, r));

    //side
    if(v < 0 || (u + v) > 1) 
    {
        return 0;
    }
    float t = f*(dot(e2, r));

    //behind
    if(t < 0)
    {
        return 0;
    }

    return t;
} 

//AABB-test
//http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
bool hitBox(Ray r, vec3 _max, vec3 _min, mat4 scale)
{
    _min *= scale[0][0];
    _max *= scale[0][0];

    // r.dir is unit direction vector of ray
    vec3 dirfrac;
    dirfrac.x = 1.0f / r.dir.x;
    dirfrac.y = 1.0f / r.dir.y;
    dirfrac.z = 1.0f / r.dir.z;
    // lb is the corner of AABB with minimal coordinates - LEFT bottom, rt is maximal corner
    // r.org is origin of ray
    float t1 = (_min.x - r.origin.x)*dirfrac.x;
    float t2 = (_max.x - r.origin.x)*dirfrac.x;
    float t3 = (_min.y - r.origin.y)*dirfrac.y;
    float t4 = (_max.y - r.origin.y)*dirfrac.y;
    float t5 = (_min.z - r.origin.z)*dirfrac.z;
    float t6 = (_max.z - r.origin.z)*dirfrac.z;

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
    float t = 0;
    if (tmax < 0)
    {
        t = tmax;
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        t = tmax;
        return false;
    }

    t = tmin;
    return true;
}

//ficklampsljus från kameraposition
float flashLight(Vertex hit,Ray ray)
{

      float dist = distance(hit.worldPos.xyz, m_camPos);

      float angle = 0.7;
      float decay = 0.2;
      float reflexMax = 0.7;

      float scatter = dot(m_camDir, normalize(-ray.dir));
      float dif = 1.0-angle;
      float factor = clamp((scatter-angle)/dif, 0.0, 1.0);

      if(scatter > angle)
        return clamp(factor/(dist*decay),0.0,reflexMax);

      return 0; 
}

// Skuggan är tung för att varje träff måste skjuta en ny stråle mot ljuset och testa kollision,
// därför blir detta nästan dubbelt så dyrt som utan skugga.
float shadowTrace(Vertex hit)
{
    Ray ray;

    ray.origin = hit.worldPos.xyz;
    ray.color = vec4(0,0,0,0);

    float increment = 0.05;
    float shadow = 1.0;
    

#ifdef MODELS
    //MODELS
    for(int f = 0; f < SHADOW_LIGHTS; f++)
    {
        ray.dir = normalize(m_lights[f].Position.xyz-ray.origin);

        for(uint j=0; j < 2;j++)
        {
            #ifdef AABB
            //testa mot minmax AABB
            if(hitBox(ray,m_models[0].maximum,m_models[0].minimum,modelMatrix[j]))
            {
            #endif
                for(uint i=0; i < 132;i +=3)
                {
                    Vertex vx1;
                    vx1.worldPos = modelMatrix[j]*vec4(m_models[0].pos[i],1);

                    Vertex vx2;
                    vx2.worldPos = modelMatrix[j]*vec4(m_models[0].pos[i+1],1);

                    Vertex vx3;
                    vx3.worldPos = modelMatrix[j]*vec4(m_models[0].pos[i+2],1);

                    float r = intersect(ray,vx1,vx2,vx3);

                    if(r > 0.1)
                    {
                        //stråle sänds från Hit, och spåras mot ljuset, om den träffar något innan ljuset står den pixeln i skugga
                        if(distance(ray.dir*r+ray.origin,ray.origin) < distance(ray.origin,m_lights[0].Position.xyz))
                        {
                            shadow -= increment;
                        }
                    }
                }
            #ifdef AABB
            }
            #endif 
        }
    #endif
    #ifdef SPHERES
        //SPHERES
        for(int j=0;j < 10;j++)
        {
            vec4 sData = hitSphere(m_spheres[j],ray);

            if(sData.w > 0.01)
            {
                //stråle sänds från Hit, och spåras mot ljuset, om den träffar något innan ljuset står den pixeln i skugga
                if(distance(ray.dir*sData.w+ray.origin,ray.origin) < distance(ray.origin,m_lights[f].Position.xyz))
                {
                    shadow -= increment;
                }
            }
        }
    }
#endif
    return shadow; 
}