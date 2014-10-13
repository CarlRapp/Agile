#version 420
subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexStartTime;

out vec3 Position; // To transform feedback
out vec3 Velocity; // To transform feedback
out float StartTime; // To transform feedback
out float Transp; // To fragment shader

uniform float Time; // Simulation time (elapsed time)
uniform float DeltaTime; // Elapsed time between frames dt
uniform float ParticleLifetime; // Particle lifespan

uniform float Size;
uniform vec3 BallPos;

uniform mat4 ProjectionMatrix;
uniform mat4 Model;
uniform mat4 View;

uniform float Rate;

mat4 ViewProjection;

subroutine (RenderPassType)
void update() 
{
	Position = VertexPosition;
	Velocity = VertexVelocity;
	StartTime = VertexStartTime;

	if( Time >= StartTime ) {
		float age = Time - StartTime;
		
		if( age >ParticleLifetime) {
			// The particle is past its lifetime, recycle.
			Position = BallPos;
			Velocity = vec3(0.0);
			StartTime = Time-(age-ParticleLifetime);
		} 
		else {
			// The particle is alive, update.
			//Position += Velocity * DeltaTime;
		}
	}

}
subroutine (RenderPassType)
void render() 
{
	float age = Time - VertexStartTime;
	
	gl_Position = ViewProjection * vec4(VertexPosition, 1.0);

	vec4 eyep = View * Model * vec4(VertexPosition, 1.0);

	float sizeFactor = 1.0;
	Transp = 0.0;

	if(Time >= VertexStartTime)
	{
            sizeFactor = 1.0-(age/ParticleLifetime)*0.8;
            Transp = 1.0 - age / ParticleLifetime;
	}

	gl_PointSize = 200 * Size * sizeFactor /-eyep.z;
}

void main()
{
	ViewProjection = ProjectionMatrix * View;
	// This will call either render() or update()
	RenderPass();
}