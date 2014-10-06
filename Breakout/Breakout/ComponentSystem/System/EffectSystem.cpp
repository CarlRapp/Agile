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
}

EffectSystem::~EffectSystem()
{

}

void EffectSystem::Update(float _dt)
{
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;
		auto flags = e->GetComponent<EffectComponent>()->m_effects;

		//OnEveryFrame



		//OnCollide
		auto collide = e->GetComponent<CollisionComponent>();
		if (collide)
		{
			if (collide->GetCollisions().size() > 0)
			{

			}
		}

		//OnRemove
		if (e->GetState() == Entity::SOON_DEAD)
		{

			// Shatter
			if ((flags & EffectFlags::SHATTER) == EffectFlags::SHATTER)
			{
				e->GetComponent<ShatterComponent>()->m_explosionState = ShatterComponent::EXPLODING;
				e->RemoveComponent<CollisionComponent>();
			}


		}

		UpdateComponents(e, _dt);
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