#include "EffectSystem.h"
#include "../Component/CollisionComponent.h"
#include "../Component/EffectComponent.h"
#include "../World.h"
#include "../EntityFactory.h"

EffectSystem::EffectSystem(World* _world)
: Base(ComponentFilter().Requires<EffectComponent>(), _world)
{
}

EffectSystem::~EffectSystem()
{

}

float r = 0.f;
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

				if ( ((effect & EffectFlags::SHATTER) == EffectFlags::SHATTER))
				{
					// Presume the entity has an modelComponent since it should shatter
					//e->SetState(Entity::EXPLODE);
					e->GetComponent<ModelComponent>()->Explode();
					//e->RemoveComponent<CollisionComponent>();
				}

				if ((effectFlags & EffectFlags::INVISIBLE) == EffectFlags::INVISIBLE)
				{
					if (e->HasComponent<ModelComponent>())
					{
						e->RemoveComponent<ModelComponent>();
					}
					else
					{
						e->AddComponent<ModelComponent>().m_modelPath = "Box_1_1x1x1";
					}
				}


			}
		}


	}

}