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
	EffectFlags effectFlags;

	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;

		auto effectFlags = e->GetComponent<EffectComponent>()->m_effects;

		if ((effectFlags & EffectFlags::NO_EFFECT) == EffectFlags::NO_EFFECT)
			continue;


		// Every effect which trigger at a collision is checked here
		auto collision = e->GetComponent<CollisionComponent>();
		if (collision)
		{
			std::vector<int> collisions = collision->GetCollisions();
			for (unsigned int i = 0; i < collisions.size(); ++i)
			{
				auto effect = e->GetComponent<EffectComponent>()->m_effects;

				if (((effect & EffectFlags::SHATTER) == EffectFlags::SHATTER))
				{
					e->GetComponent<ShatterComponent>()->m_explosionState = ShatterComponent::EXPLODING;
				}

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
			_e->SetState(Entity::DEAD);
	}
}