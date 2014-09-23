#include "ProjectileSystem.h"
#include "../Component/PositionComponent.h"
#include "../Component/MouseInputComponent.h"

#include "../World.h"
#include "../EntityFactory.h"

ProjectileSystem::ProjectileSystem(World* _world)
: Base(ComponentFilter().Requires<PositionComponent, MouseInputComponent>(), _world)
{
}

ProjectileSystem::~ProjectileSystem()
{

}


void ProjectileSystem::Update(float _dt)
{
	MouseInputComponent* mouse;
	PositionComponent* position;
	EntityMap::iterator it;

	for (int i = 0; i < m_entites.size(); ++i)
	{
		if (m_entites[i]->GetState() != Entity::ALIVE)
		{
			m_entites.erase(m_entites.begin() + i);
			continue;
		}

		position = m_entites[i]->GetComponent<PositionComponent>();
		if (position->m_position.y > 25)
		{
			m_entites[i]->SetState(Entity::DEAD);
			m_entites.erase(m_entites.begin() + i);
		}
	}




	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() != Entity::ALIVE)
			continue;

		mouse = it->second->GetComponent<MouseInputComponent>();
		if (mouse->m_controls.LeftButton == InputState::Pressed)
		{
			position = it->second->GetComponent<PositionComponent>();

			Entity* e = m_world->CreateEntity();
			EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BLOCK);
			e->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(0, 10, 0);
			e->GetComponent<PositionComponent>()->m_position = VECTOR3(position->m_position.x, position->m_position.y + 1, 0);

			m_world->AddEntity(e);
			m_entites.emplace_back(e);
		}


	}
}