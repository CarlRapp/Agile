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
	float midVelocityX;
	float midPositionX;
	float scaleX;

	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;

		auto mouse = e->GetComponent<MouseInputComponent>();
		auto position = e->GetComponent<PositionComponent>();
		auto velocity = e->GetComponent<VelocityComponent>();
		auto scale = e->GetComponent<ScaleComponent>();

		velocity->m_velocity = VECTOR3(mouse->m_controls.MouseDX, 0, 0);
		if (position->GetPosition().x + (velocity->m_velocity.x * _dt) - scale->GetScale().x / 2 < -48.5f)
		{
			position->SetPosition(VECTOR3(-48.5f + scale->GetScale().x / 2, position->GetPosition().y, position->GetPosition().z));
			velocity->m_velocity = VECTOR3(0, 0, 0);
		}
		else if (position->GetPosition().x + (velocity->m_velocity.x * _dt) + scale->GetScale().x / 2 > 48.5f)
		{
			position->SetPosition(VECTOR3(48.5f - scale->GetScale().x / 2, position->GetPosition().y, position->GetPosition().z));
			velocity->m_velocity = VECTOR3(0, 0, 0);
		}
	}
}