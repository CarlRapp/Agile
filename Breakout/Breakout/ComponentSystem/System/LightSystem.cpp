#include "LightSystem.h"


LightSystem::LightSystem(World* _world)
: Base(ComponentFilter().Requires<PositionComponent, LightComponent>(), _world), m_lightId(0)
{
	m_graphicsManager = GraphicsManager::GetInstance();
}

LightSystem::~LightSystem()
{

}

void LightSystem::OnEntityAdded(Entity* _entity)
{
	VECTOR3* position = &_entity->GetComponent<PositionComponent>()->GetPosition();
	VECTOR3* intensity = &_entity->GetComponent<LightComponent>()->GetIntensity();
	VECTOR3* color = &_entity->GetComponent<LightComponent>()->GetColor();
	float* range = &_entity->GetComponent<LightComponent>()->GetRange();

	m_graphicsManager->AddPointLight(GetMemoryID(_entity), position, intensity, color, range);
}

void LightSystem::OnEntityRemove(Entity* _entity)
{
	m_graphicsManager->RemovePointLight(GetMemoryID(_entity));
}


void LightSystem::Update(float _dt)
{
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;

	}

}