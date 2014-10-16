#include "ShootLaserSystem.h"
#include "PhysicsSystem.h"
#include "../Component/MouseInputComponent.h"
#include "../Component/BallComponent.h"
#include "../../Audio/AudioManager.h"

ShootLaserSystem::ShootLaserSystem(World* _world)
: Base(ComponentFilter().Requires<MouseInputComponent, PositionComponent, ScaleComponent>(), _world)
{
	m_duration = m_defaultDuration;
}

ShootLaserSystem::~ShootLaserSystem()
{
}

void ShootLaserSystem::Update(float _dt)
{
	m_duration -= _dt;

	if (m_duration > 0)
		for (auto it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
		{
			Entity* e = it->second;
			if (e->GetState() != Entity::ALIVE)
				continue;

			if (e->GetComponent<MouseInputComponent>()->m_controls.LeftButton == InputState::Pressed)
			{
				auto position = it->second->GetComponent<PositionComponent>()->GetPosition();
				auto scale = it->second->GetComponent<ScaleComponent>()->GetScale();

				Entity* laser = m_world->CreateEntity();
				EntityFactory::GetInstance()->CreateEntity(laser, EntityFactory::LASER);
				laser->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(0, 50, 0);
				laser->GetComponent<ScaleComponent>()->SetScale(VECTOR3(0.2f, 3, 0.2f));
				laser->GetComponent<PositionComponent>()->SetPosition(VECTOR3(position.x + (scale.x * 0.5f) - (laser->GetComponent<ScaleComponent>()->GetScale().x * 0.5f), position.y + (scale.y * 0.5f) + (laser->GetComponent<ScaleComponent>()->GetScale().y * 0.5f) + 1, 0));
				m_world->AddEntity(laser);

				laser = m_world->CreateEntity();
				EntityFactory::GetInstance()->CreateEntity(laser, EntityFactory::LASER);
				laser->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(0, 50, 0);
				laser->GetComponent<ScaleComponent>()->SetScale(VECTOR3(0.2f, 3, 0.2f));
				laser->GetComponent<PositionComponent>()->SetPosition(VECTOR3(position.x - (scale.x * 0.5f) + (laser->GetComponent<ScaleComponent>()->GetScale().x * 0.5f), position.y + (scale.y * 0.5f) + (laser->GetComponent<ScaleComponent>()->GetScale().y * 0.5f) + 1, 0));
				m_world->AddEntity(laser);
				AudioManager::GetInstance()->PlaySoundEffect("Laser.wav", 0);
				
			}
		}
	else
		m_world->RemoveSystem<ShootLaserSystem>();
}

void ShootLaserSystem::ResetDuration()
{
	m_duration = m_defaultDuration;
}