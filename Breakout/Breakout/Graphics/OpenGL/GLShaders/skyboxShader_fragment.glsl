#version 420

in vec3 TexCoord;
out vec4 FragColor;
uniform samplerCube cubemap;

void main()
{
	FragColor = texture(cubemap, TexCoord);//vec4(1.0,1.0,1.0,1.0);//
}