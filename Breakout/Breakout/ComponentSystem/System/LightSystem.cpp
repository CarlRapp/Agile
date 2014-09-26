#include "LightSystem.h"


LightSystem::LightSystem(World* _world)
: Base(ComponentFilter().Requires<PositionComponent, LightComponent>(), _world), m_lightId(0)
{
	m_graphicsManager = GraphicsManager::GetInstance();
}

LightSystem::~LightSystem()
{

}

bool LightSystem::Add(Entity* _entity)
{
	if (Base::Add(_entity))
	{
		VECTOR3* position	= &_entity->GetComponent<PositionComponent>()->GetPosition();
                VECTOR3* intensity      = &_entity->GetComponent<LightComponent>()->GetIntensity();
		VECTOR3* color		= &_entity->GetComponent<LightComponent>()->GetColor();
		float* range		= &_entity->GetComponent<LightComponent>()->GetRange();

		m_graphicsManager->AddPointLight(m_lightId++, position, intensity, color, range);
		return true;
	}

	return false;
}


void LightSystem::Update(float _dt)
{
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;

	}

}