#version 420

layout (location = 0) in vec3 VertexPoint;
out vec3 TexCoord;

uniform mat4 PVM;

void main()
{
	gl_Position = PVM * vec4(VertexPoint, 1.0);
	TexCoord = VertexPoint;
}