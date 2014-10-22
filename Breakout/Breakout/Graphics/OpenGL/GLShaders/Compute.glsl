#version 430

writeonly uniform image2D destTex;

uniform sampler2D   m_objectTex;
uniform mat4        m_matView;
uniform mat4        m_matProj;
uniform vec3        camPos;

layout (local_size_x = 16, local_size_y = 16) in;

struct LightInfo
{
	vec4 Position; // Light position world coords.
	vec3 Intensity; // La, Ld and Ls intensity
	vec3 Color;
	float Range;
};
const int nrOfLights = 10;
uniform LightInfo   Lights[nrOfLights];

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
uniform Model       Models[1];


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
const mat4 modelMatrix[2] = {
                                    mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1),
                                    mat4(10,0,0,0,0,10,0,0,0,0,10,0,0,0,0,1)
                            };
Sphere spheres[10]  =   {
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

                            


const float widthHalf = 256.0;
const float heightHalf = 256.0;

const int BOUNCE_LIMIT = 4;

//METHODS
vec4 traceColor(Vertex vertex);
float intersect(Ray ray,Vertex vx1,Vertex vx2,Vertex vx3);
vec3 phongModelDiffAndSpec(int index, vec3 lightPos, float strength, Vertex vertex);
vec4 trace(Ray ray);
vec4 hitSphere(Sphere s, Ray r);
bool hitBox(Ray r, vec3 _max, vec3 _min, mat4 _scale);
float shadowTrace(Vertex hit);

void main() 
{
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;

    //send ray from camera eye (camPos) with directions in worldspace
    vec4 rayPos;

    rayPos.x    = x/widthHalf -1.0;
    rayPos.y    = y/heightHalf -1.0;
    rayPos.z    = 1;
    rayPos.w    = 1;
    
    rayPos *= m_matProj;
    rayPos *= m_matView;

    vec3 rayDir = normalize(vec3(rayPos));

    Ray ray = {camPos, rayDir,0,vec4(0,0,0,0)};   

    spheres[0].position.xyz = camPos;

    for(int i =2; i < 9;i++)
    {
        spheres[i].position = Lights[i].Position;
    }

    vec4 color = trace(ray);

    //barrier();
    imageStore(destTex, ivec2(x,y), color);
}

vec4 trace(Ray ray)
{
    bool invert = false;

    for(uint k=0; ray.bounce < BOUNCE_LIMIT;ray.bounce++)
    {
        Vertex hit;
        Triangle saveTri;
        float   saveT=10000;

        //MODELS
        for(uint j=0; j < 2;j++)
        {
            //testa mot minmax AABB
            if(hitBox(ray,Models[0].maximum,Models[0].minimum,modelMatrix[j]))
            {
                for(uint i=0; i < 132;i +=3)
                {
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

                        //Kör cross på alla för att få ut totala arean, därefter ta fram 3 subtrianglar och jämför deras areor med totala.
                        //Då får man en ratio och vet hur mycket av varje texcoord som ska användas.
                        //http://answers.unity3d.com/questions/383804/calculate-uv-coordinates-of-3d-point-on-plane-of-m.html
                        float a = 1 / length(cross(vx1.worldPos.xyz-vx2.worldPos.xyz, vx1.worldPos.xyz-vx3.worldPos.xyz)); 
                        float a1 = length(cross(vx2.worldPos.xyz-hit.worldPos.xyz, vx3.worldPos.xyz-hit.worldPos.xyz)) * a; 
                        float a2 = length(cross(vx3.worldPos.xyz-hit.worldPos.xyz, vx1.worldPos.xyz-hit.worldPos.xyz)) * a; 
                        float a3 = length(cross(vx1.worldPos.xyz-hit.worldPos.xyz, vx2.worldPos.xyz-hit.worldPos.xyz)) * a; 

                        hit.texCoord = vx1.texCoord * a1 + vx2.texCoord * a2 + vx3.texCoord * a3;

                    }
                }
            }
            //fulhax för att man befinner sig i model[1], stor kub
            invert = true; 
        }

        //SPHERES
        for(int j=0;j < 10;j++)
        {
            vec4 sData = hitSphere(spheres[j],ray);

            if(sData.w > 0.01 && sData.w < saveT)
            {
                saveT = sData.w;
                hit.worldPos = vec4(vec3(ray.dir*sData.w+ray.origin),1);
                hit.normal = sData.xyz;
                hit.texCoord = vec2(1,0);
            }
        }

        if(saveT > 0 && saveT != 10000)
        {
            ray.color += traceColor(hit);
            ray.color *= shadowTrace(hit);

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

    return vec3(diffuse +specular);
}

vec4 traceColor(Vertex vertex)
{
    vec3 diffAndSpec = vec3(0.0);

    for(int index = 0; index < nrOfLights; index++)
    {
        float lightDist = abs(length(vertex.worldPos.xyz - vec3(Lights[index].Position)));
        float lightStrength = 0;
        if( lightDist < Lights[index].Range )
			lightStrength = 1.0-(lightDist / Lights[index].Range);

	diffAndSpec += phongModelDiffAndSpec(index, vec3(Lights[index].Position), lightStrength,vertex);
    }
    vec4 texColor = vec4(1,1,1,1);

    texColor = texture( m_objectTex, vertex.texCoord );
    
    return vec4(diffAndSpec, 1.0) * texColor;
}

float intersect(Ray ray,Vertex vx1,Vertex vx2,Vertex vx3)
{
    vec3 e1 = vx2.worldPos.xyz-vx1.worldPos.xyz;
    vec3 e2 = vx3.worldPos.xyz-vx1.worldPos.xyz;
    vec3 q = cross(ray.dir.xyz, e2);
    float a = dot(e1, q);

    //parallel
    //if(a > -0.1 && a < 0.1)
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

//http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
bool hitBox(Ray r, vec3 _max, vec3 _min, mat4 scale)
{
    _min *= scale[0][0];//pow(scale[0][0],3); //(scale*scale*vec4(_min,1)).xyz;
    _max *= scale[0][0];//pow(scale[0][0],3); //(scale*scale*vec4(_max,1)).xyz;

    // r.dir is unit direction vector of ray
    vec3 dirfrac;
    dirfrac.x = 1.0f / r.dir.x;
    dirfrac.y = 1.0f / r.dir.y;
    dirfrac.z = 1.0f / r.dir.z;
    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
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

float shadowTrace(Vertex hit)
{
    Ray ray;

    ray.origin = hit.worldPos.xyz;
    ray.color = vec4(0,0,0,0);
    ray.dir = normalize(Lights[0].Position.xyz-ray.origin);

    //MODELS
    for(uint j=0; j < 2;j++)
    {
        //testa mot minmax AABB
        if(hitBox(ray,Models[0].maximum,Models[0].minimum,modelMatrix[j]))
        {
            for(uint i=0; i < 132;i +=3)
            {
                Vertex vx1;
                vx1.worldPos = modelMatrix[j]*vec4(Models[0].pos[i],1);

                Vertex vx2;
                vx2.worldPos = modelMatrix[j]*vec4(Models[0].pos[i+1],1);

                Vertex vx3;
                vx3.worldPos = modelMatrix[j]*vec4(Models[0].pos[i+2],1);

                float r = intersect(ray,vx1,vx2,vx3);

                if(r > 0.1)
                {
                    if(distance(ray.dir*r+ray.origin,ray.origin) < distance(ray.origin,Lights[0].Position.xyz))
                    {
                        return 0.7;
                    }
                }
            }
        }
    }

    //SPHERES
    for(int j=0;j < 10;j++)
    {
        vec4 sData = hitSphere(spheres[j],ray);

        if(sData.w > 0.01)
        {
            if(distance(ray.dir*sData.w+ray.origin,ray.origin) < distance(ray.origin,Lights[0].Position.xyz))
            {
                /*float lightDist = abs(length(ray.origin - vec3(Lights[0].Position)));
                float lightStrength = 0;
                if( lightDist < Lights[0].Range )
                    lightStrength = 1.0-(lightDist / Lights[0].Range);

                ray.color.xyz -= vec3(0.1,0.1,0.1);//;phongModelDiffAndSpec(0, vec3(Lights[0].Position), lightStrength,hit);
*/
                return 0.7;
            }
        }
    }

    return 1.0; 
}