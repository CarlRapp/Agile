#include "MovementSystem.h"


MovementSystem::MovementSystem(World* _world)
: Base(ComponentFilter().Requires<PositionComponent, VelocityComponent, CollisionComponent, MouseInputComponent>(), _world)
{
}

MovementSystem::~MovementSystem()
{

}


void MovementSystem::Update(float _dt)
{
	bool collidingWithLWall = false;
	bool collidingWithRWall = false;

	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;

		
		auto mouse = e->GetComponent<MouseInputComponent>();
		auto position = e->GetComponent<PositionComponent>();
		auto velocity = e->GetComponent<VelocityComponent>();
		auto scale = e->GetComponent<ScaleComponent>();
		auto isMid = e->GetComponent<SpawnEntityComponent>();

		velocity->m_velocity = VECTOR3(mouse->m_controls.MouseDX, 0, 0);

		if (isMid)
		{
			if (position->GetPosition().x - scale->GetScale().x * 0.5f - 0.25f < -46.5f)
				position->SetPosition(VECTOR3(-46.5f + scale->GetScale().x * 0.5f + 0.25f, position->GetPosition().y, position->GetPosition().z));
			if (position->GetPosition().x + scale->GetScale().x * 0.5f + 0.25f > 46.5f)
				position->SetPosition(VECTOR3(46.5f - scale->GetScale().x * 0.5f + 0.25f, position->GetPosition().y, position->GetPosition().z));
		}
		else
		{
			if (position->GetPosition().x - 0.25f < -46.5f)
				position->SetPosition(VECTOR3(-46.5f + 0.25f, position->GetPosition().y, position->GetPosition().z));
			if (position->GetPosition().x + scale->GetScale().x * 0.5f + 0.25f > 46.5f)
				position->SetPosition(VECTOR3(46.5f - scale->GetScale().x * 0.5f + 0.25f, position->GetPosition().y, position->GetPosition().z));
		}
	}
}