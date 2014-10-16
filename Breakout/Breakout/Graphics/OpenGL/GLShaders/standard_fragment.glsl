#version 430

//uniform vec3 m_testLight;

varying mat4 modelView;

uniform sampler2D m_standardTex;
uniform vec4 EyePosition;

layout( location = 0 ) out vec4 FragColor;

in GS_FS
{
    vec4    worldPos;
    vec4    color;
    vec3    normal;
    float   pad1;
    vec2    texCoord;
    vec2    pad2;

} vertex;


const int nrOfLights = 5;

struct LightInfo{
	vec4 Position; // Light position world coords.
	vec3 Intensity; // La, Ld and Ls intensity
	vec3 Color;
	float Range;
};
uniform LightInfo Lights[nrOfLights];

struct MaterialInfo{
	//vec3 Ka;			// Ambient reflectivity
	//vec3 Kd;		    // Diffuse reflectivity
	float Ks;			// Specular reflectivity
	float Ns;	// Specular shininess factor
};
uniform MaterialInfo Material;

void phongModel(int index, vec3 toEye, out vec3 ambient, out vec3 diffuse, out vec3 spec) {

        //tmp material
    //    Material.Ka = vec3(0.2);
    //    Material.Kd = vec3(0.9);
    //    Material.Ks = vec3(1.0);
    //    Material.Shininess = 100.0;

        ambient = Lights[index].Color * Lights[index].Intensity.x;
        diffuse = vec3(0.0);
        spec    = vec3(0.0);

        vec3 lightVec = Lights[index].Position.xyz - vertex.worldPos.xyz;

        float d = length(lightVec);

        if(d > Lights[index].Range)
            return;

        lightVec /= d; //normlizing
        float diffuseFactor = dot( lightVec, vertex.normal );

        if(diffuseFactor > 0)
        {
            vec3 v = reflect( -lightVec, vertex.normal );
            float specFactor = pow( max( dot(v, toEye), 0.0 ), Material.Ns );

            diffuse = diffuseFactor * Lights[index].Color * Lights[index].Intensity.y;
            spec = specFactor * Lights[index].Color * Lights[index].Intensity.z * Material.Ks;
        }

        float att = 1 - pow((d/Lights[index].Range), 1.0f);

        ambient *= att;
        diffuse *= att;
        spec    *= att;

        return;
}

void main(void) 
{   
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 spec    = vec3(0.0);

    vec3 toEye   = normalize(EyePosition.xyz - vertex.worldPos.xyz);

    for(int index = 0; index < nrOfLights; index++)
    {
        vec3 a,d,s;

        //för varje ljus
        phongModel(index, toEye, a, d, s);
        ambient += a;
        diffuse += d;
        spec    += s;

        
    }

    vec4 texColor = texture( m_standardTex, vertex.texCoord );
    
    FragColor = vec4(ambient + diffuse, 1.0) * texColor + vec4(spec, 0.0f);

    //FragColor = vec4((vertex.normal + vec3(1.0))*0.5, 1.0);
}
