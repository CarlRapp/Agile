#include "EffectSystem.h"
#include "../Component/CollisionComponent.h"
#include "../Component/EffectComponent.h"
#include "../Component/ShatterComponent.h"
#include "../World.h"
#include "../EntityFactory.h"
#include "../../Graphics/GraphicsManager.h"

EffectSystem::EffectSystem(World* _world)
: Base(ComponentFilter().Requires<EffectComponent>(), _world)
{
	m_currentTime = 0.f;
	m_maxTime = 1.f;
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

		if ((e->GetComponent<EffectComponent>()->m_effects & EffectFlags::NO_EFFECT) == EffectFlags::NO_EFFECT)
			continue;

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

		// OnRemove
		if (e->GetState() == Entity::SOON_DEAD)
			OnRemove(e, _dt);

	}
}

void EffectSystem::UpdateComponents(Entity* _e, float _dt)
{
	// Shatter
	auto shatter = _e->GetComponent<ShatterComponent>();
	if (shatter)
	{
		if (shatter->IsExploding(_dt) == ShatterComponent::DONE)
			_e->SetState(Entity::SOON_DEAD);
	}
}



void EffectSystem::OnEntityAdded(Entity* _e)
{
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
void EffectSystem::OnRemove(Entity* _e, float _dt)
{
}