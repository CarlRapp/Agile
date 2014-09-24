#include "MovementSystem.h"
#include "../Component/VelocityComponent.h"
#include "../Component/PositionComponent.h"
#include "../Component/MouseInputComponent.h"

MovementSystem::MovementSystem(World* _world)
: Base(ComponentFilter().Requires<PositionComponent, MouseInputComponent>(), _world)
{
}

MovementSystem::~MovementSystem()
{

}


void MovementSystem::Update(float _dt)
{
	/*
	PositionComponent* position;
	VelocityComponent* velocity;
	EntityMap::iterator it;

	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() != Entity::ALIVE)
			continue;

		velocity = e->GetComponent<VelocityComponent>();

		if (e->HasComponent<MouseInputComponent>())
		{
			MouseInputComponent* mouse = e->GetComponent<MouseInputComponent>();
			velocity->m_velocity.x = mouse->m_controls.MouseDX * 5.f;
		}

		if (ISZERO(velocity->m_velocity))
			continue;

		position = e->GetComponent<PositionComponent>()->SetPosition(;

		position->m_position.x += velocity->m_velocity.x * _dt;
		position->m_position.y += velocity->m_velocity.y * _dt;
		//position->m_position.z += velocity->m_velocity.z * _dt;

		position->m_deltaPosition.x = velocity->m_velocity.x * _dt;
		position->m_deltaPosition.y = velocity->m_velocity.y * _dt;
		//position->m_deltaPosition.z = velocity->m_velocity.z * _dt;

		position->m_position.x = position->m_position.x < 1 ? 1 : position->m_position.x;
		position->m_position.x = position->m_position.x > 24.5f ? 24.5f : position->m_position.x;

	}
	*/
	/*
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;

		auto mouse = e->GetComponent<MouseInputComponent>();
		auto position = e->GetComponent<PositionComponent>();
		auto velocity->m_velocity.x = mouse->m_controls.MouseDX * 5.f;

		
	}
	*/
}