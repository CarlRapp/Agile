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
	m_currentTime = m_currentTime >= m_maxTime ? 0 : m_currentTime + _dt;
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;
		auto flags = e->GetComponent<EffectComponent>()->m_effects;

		//OnEveryFrame

		//OnEverySecond
		if (m_currentTime >= m_maxTime)
		{
			if ((flags & EffectFlags::INVISIBLE) == EffectFlags::INVISIBLE)
			{
				auto model = e->GetComponent<ModelComponent>();
				if (model)
				{
					if (!model->m_render)
					{
						auto shatter = e->GetComponent<ShatterComponent>();
						//model->m_gpuId = GetMemoryID(e);

						if (shatter)
							GraphicsManager::GetInstance()->AddObject(GetMemoryID(e), model->m_modelPath, &model->m_worldMatrix, &model->m_worldMatrix, &shatter->m_explosion);
						else
							GraphicsManager::GetInstance()->AddObject(GetMemoryID(e), model->m_modelPath, &model->m_worldMatrix, &model->m_worldMatrix, 0);

						model->m_render = true;
					}
					else
					{
						GraphicsManager::GetInstance()->RemoveObject(GetMemoryID(e));
						model->m_render = false;
					}
				}
			}

			
		}

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