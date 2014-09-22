#version 420

uniform vec3 m_testLight;

varying mat4 modelView;

in VERTEX
{
    vec3    worldPos;
    vec3    normal;
    vec4    color;

} vertex;


const int nrOfLights = 2;

struct LightInfo{
	vec4 Position; // Light position world coords.
	vec3 Intensity; // La, Ld and Ls intensity
	vec3 Color;
	float Distance;
};
LightInfo Lights[nrOfLights];

struct MaterialInfo{
	vec3 Ka;			// Ambient reflectivity
	vec3 Kd;		    // Diffuse reflectivity
	vec3 Ks;			// Specular reflectivity
	float Shininess;	// Specular shininess factor
};
MaterialInfo Material;

vec3 phongModelDiffAndSpec(int index, vec3 lightPos) {

                //tmp material
                Material.Ka = vec3(0.3);
                Material.Kd = vec3(0.6);
                Material.Ks = vec3(1.0);
                Material.Shininess = 190.0;

		vec3 s = normalize(vec3(lightPos - vertex.worldPos));
		vec3 v = normalize(-vertex.worldPos.xyz);	
		vec3 h = normalize(v + s);

		float sDotN = max( dot(s, vertex.normal), 0.0 );
	
		//Diffuse light
		vec3 diffuse = vec3(Lights[index].Intensity.y) * Lights[index].Color * Material.Kd * sDotN;

		vec3 Ks = Material.Ks;

		vec3 specular = vec3(0.0);
	
		//Specular light
		if( sDotN > 0.0 )
			specular = vec3(Lights[index].Intensity.z) * Lights[index].Color * Ks * pow( max( dot(h,v), 0.0 ), Material.Shininess );
		
		return vec3(diffuse + specular);
}

void main(void) 
{   
    vec3 ambient = vec3(0.2f,0.2f,0.2f);
    vec3 diffAndSpec = vec3(0.0);

    Lights[0].Position = vec4(5.0, 5.0, 3.0, 1.0);
    Lights[0].Intensity = vec3(0.2, 0.8, 0.8);
    Lights[0].Color = vec3(0.5, 0.5, 1.0);

    Lights[1].Position = vec4(-5.0, 5.0, 3.0, 1.0);
    Lights[1].Intensity = vec3(0.2, 0.8, 0.8);
    Lights[1].Color = vec3(1.0, 0.5, 0.5);

    for(int index = 0; index < nrOfLights; index++)
    {
        //för varje ljus
	diffAndSpec += phongModelDiffAndSpec(index, vec3(Lights[index].Position)); //, lightStrength);
    }

        //vec4 A = modelView*vec4(vertex.normal,1.0);
        //float b = dot(normalize(m_testLight),vec3(A));
        //b = pow(b,2);
        //gl_FragColor = vec4(vertex.normal.x, vertex.normal.y, vertex.normal.z, 1.0);

    gl_FragColor = vec4(diffAndSpec + ambient, 1.0);
}
