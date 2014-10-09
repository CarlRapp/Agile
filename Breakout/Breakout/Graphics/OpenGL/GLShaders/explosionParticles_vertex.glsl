#version 420
subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexStartTime;
layout (location = 3) in vec3 VertexInitialVelocity;
layout (location = 4) in vec3 VertexInitialPosition;

out vec3 Position; // To transform feedback
out vec3 Velocity; // To transform feedback
out float StartTime; // To transform feedback
out float Transp; // To fragment shader

uniform float Time; // Simulation time (elapsed time)
uniform float DeltaTime; // Elapsed time between frames dt
uniform vec3 Accel; // Particle acceleration
uniform float ParticleLifetime; // Particle lifespan

uniform float Size;
uniform int Type;

uniform mat4 ProjectionMatrix;
uniform mat4 Model;
uniform mat4 View;

mat4 MVP;

subroutine (RenderPassType)
void update() 
{
	Position = VertexPosition;
	Velocity = VertexVelocity;
	StartTime = VertexStartTime;

	if( Time >= StartTime ) {
		float age = Time - StartTime;
		
		if( age >ParticleLifetime ) {
			// The particle is past its lifetime, recycle.
			Position = VertexInitialPosition; //vec3(0.0);
			Velocity = VertexInitialVelocity;
			StartTime = Time;
		} 
		else {
			// The particle is alive, update.
			Position += Velocity * DeltaTime;
			Velocity += Accel * DeltaTime;			
		}
	}

}
subroutine (RenderPassType)
void render() 
{
	float age = Time - VertexStartTime;
	
	gl_Position = MVP * vec4(VertexPosition, 1.0);

	vec4 eyep = View * Model * vec4(VertexPosition, 1.0);

	float sizeFactor = 1.0;
	Transp = 0.0;
	if(Time >= VertexStartTime)
	{
		if(Type == 0)
			sizeFactor = 1.0-(age/ParticleLifetime)*0.8;
		else if(Type == 1)
			sizeFactor = 1.0+(age/ParticleLifetime)*0.7;
		Transp = 1.0 - age / ParticleLifetime;
	}

	gl_PointSize = 200 * Size * sizeFactor /-eyep.z;
}

void main()
{
	//Accel = vec3(0.0); //vec3(0.01,0.0,0.0);
	MVP = ProjectionMatrix * View * Model;
	// This will call either render() or update()
	RenderPass();
}