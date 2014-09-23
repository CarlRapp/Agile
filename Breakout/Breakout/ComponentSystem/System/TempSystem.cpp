#include "TempSystem.h"
#include "../Component/VelocityComponent.h"
#include "../Component/MouseInputComponent.h"

#include "../World.h"
#include "../EntityFactory.h"

TempSystem::TempSystem(World* _world)
: Base(ComponentFilter().Requires<VelocityComponent, PositionComponent, MouseInputComponent>(), _world)
{
}

TempSystem::~TempSystem()
{

}


void TempSystem::Update(float _dt)
{
	MouseInputComponent* mouse;
	VelocityComponent* velocity;
	PositionComponent* position;
	EntityMap::iterator it;

	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() != Entity::ALIVE)
			continue;

		velocity = it->second->GetComponent<VelocityComponent>();
		mouse = it->second->GetComponent<MouseInputComponent>();
		

		velocity->m_velocity.x = mouse->m_controls.MouseDX * 2.f;



		if (mouse->m_controls.LeftButton == InputState::Pressed)
		{
			position = it->second->GetComponent<PositionComponent>();

			Entity* e = m_world->CreateEntity();
			EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BLOCK);
			e->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(0, 10, 0);
			e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(position->GetPosition().x, position->GetPosition().y + 1, 0));

			m_world->AddEntity(e);
		}


	}
}