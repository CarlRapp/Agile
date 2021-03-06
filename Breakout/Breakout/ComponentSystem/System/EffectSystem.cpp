#include "EffectSystem.h"
#include "../Component/CollisionComponent.h"
#include "../Component/ShatterComponent.h"
#include "../World.h"
#include "../EntityFactory.h"
#include "../../Graphics/GraphicsManager.h"

EffectSystem::EffectSystem(World* _world)
: Base(ComponentFilter().Requires<EffectComponent>(), _world)
{
	m_currentTime = 0.f;
	m_maxTime = 1.f;
	m_flags.OnAdded = NO_EFFECT;
	m_flags.OnCollide = NO_EFFECT;
	m_flags.OnEveryFrame = NO_EFFECT;
	m_flags.OnRemoved = NO_EFFECT;
}

EffectSystem::~EffectSystem()
{

}

void EffectSystem::Update(float _dt)
{
	/*
	If currentTime is >= then maxTime, reset to 0, else add _dt
	*/
	m_currentTime = m_currentTime >= m_maxTime ? 0 : m_currentTime + _dt;

	// Go through all entites
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;

		m_flags = e->GetComponent<EffectComponent>()->m_effects;

		// OnEveryFrame
		OnEveryFrame(e, _dt);

		// OneEverySecond
		if (m_currentTime >= m_maxTime)
			OnEverySecond(e, _dt);


		// OnCollision
		auto collide = e->GetComponent<CollisionComponent>();
		if (collide)
		{
			if (collide->GetCollisions().size() > 0)
				OnCollision(e, _dt);
		}
	}

	UpdateEffects(_dt);
}

void EffectSystem::UpdateEffects(float _dt)
{
	for (auto it = m_effects.begin(); it != m_effects.end();)
	{
		auto shatter = it->second->GetComponent<ShatterComponent>();
		if (shatter)
		{
			if (shatter->IsShattering(_dt) == ShatterComponent::DONE)
			{
				it->second->SetState(Entity::SOON_DEAD);
				m_effects.erase(it++);
				continue;
			}
		}

		auto explosion = it->second->GetComponent<ExplosionComponent>();
		if (explosion)
		{

			if (explosion->IsExploding(_dt) == ExplosionComponent::DONE)
			{
				GraphicsManager::GetInstance()->RemoveParticleEffect(GetMemoryID(it->second));
				it->second->SetState(Entity::SOON_DEAD);
				m_effects.erase(it++);
				continue;
			}

		}

		++it;
	}
}

void EffectSystem::OnEntityAdded(Entity* _e)
{
	auto flags = _e->GetComponent<EffectComponent>()->m_effects;

	// Trail
	if ((flags.OnAdded & EffectFlags::TRAIL) == EffectFlags::TRAIL)
	{
		auto position = _e->GetComponent<PositionComponent>();
		if (position)
		{
			GraphicsManager::GetInstance()->AddParticleEffect(GetMemoryID(_e), "trail", &position->GetPosition(), 0);
		}

	}
}
void EffectSystem::OnEntityRemoved(Entity* _e)
{
	auto flags = _e->GetComponent<EffectComponent>()->m_effects;

	//SHATTER
	if ((flags.OnRemoved & EffectFlags::SHATTER) == EffectFlags::SHATTER)
	{
		Entity* e = m_world->CreateEntity();
		EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::SHATTER);
		e->GetComponent<ModelComponent>()->m_modelPath = _e->GetComponent<ModelComponent>()->m_modelPath;
		e->GetComponent<PositionComponent>()->SetPosition(_e->GetComponent<PositionComponent>()->GetPosition());
		m_effects[e->GetId()] = e;

		m_world->AddEntity(e);
	}

	//EXPLODE
	if ((flags.OnRemoved & EffectFlags::EXPLODE) == EffectFlags::EXPLODE)
	{
		Entity* e = m_world->CreateEntity();
		EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::EXPLOSION);
		e->GetComponent<PositionComponent>()->SetPosition(_e->GetComponent<PositionComponent>()->GetPosition());
		m_effects[e->GetId()] = e;

		m_world->AddEntity(e);

		auto position = e->GetComponent<PositionComponent>();
		GraphicsManager::GetInstance()->AddParticleEffect(GetMemoryID(e), "fire", &position->GetPosition(), 0);
	}

	if (EntityContains(flags, EffectFlags::TRAIL))
	{
		GraphicsManager::GetInstance()->RemoveParticleEffect(GetMemoryID(_e));
	}
}

void EffectSystem::OnEveryFrame(Entity* _e, float _dt)
{
}
void EffectSystem::OnEverySecond(Entity* _e, float _dt)
{
}
void EffectSystem::OnCollision(Entity* _e, float _dt)
{
}



bool EffectSystem::EntityContains(EffectEvents& _entityEvents, EffectFlags _flagToCheck)
{
	
	if (((_entityEvents.OnAdded & _flagToCheck) == _flagToCheck)
		|| ((_entityEvents.OnCollide & _flagToCheck) == _flagToCheck)
		|| ((_entityEvents.OnEveryFrame & _flagToCheck) == _flagToCheck)
		|| ((_entityEvents.OnRemoved & _flagToCheck) == _flagToCheck)
		)
		return true;


	return false;

}