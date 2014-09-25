#include "ProjectileSystem.h"
#include "../Component/PositionComponent.h"
#include "../Component/MouseInputComponent.h"

#include "../World.h"
#include "../EntityFactory.h"
#include "../../Graphics/GraphicsManager.h"

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


	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() != Entity::ALIVE)
			continue;

		mouse = it->second->GetComponent<MouseInputComponent>();
		if (mouse->m_controls.LeftButton == InputState::Down)
		{
			position = it->second->GetComponent<PositionComponent>();

			Entity* e = m_world->CreateEntity();
			EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PROJECTILE);
			e->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(0, 10, 0);
			e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(position->GetPosition().x - 2.5f, position->GetPosition().y + 2.0f, 0));
			
			m_world->AddEntity(e);


			e = m_world->CreateEntity();
			EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::PROJECTILE);
			e->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(0, 10, 0);
			e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(position->GetPosition().x + 2.5f, position->GetPosition().y + 2.0f, 0));

			m_world->AddEntity(e);


		}


	}
}