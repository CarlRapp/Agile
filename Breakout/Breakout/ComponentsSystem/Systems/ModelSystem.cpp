#include "ModelSystem.h"

#include "../Components/ModelComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/RotationComponent.h"
#include "../Components/ScaleComponent.h"

ModelSystem::ModelSystem(void)
: Base(ComponentFilter().Requires<ModelComponent, PositionComponent, RotationComponent, ScaleComponent>())
{
}

ModelSystem::~ModelSystem()
{
}

void ModelSystem::Update(float _dt)
{
	for (int i = 0; i < m_entities.size(); ++i)
	{
		if ((m_entities[i]->GetState() == Entity::LIMBO) || m_entities[i]->GetState() == Entity::DEACTIVATED)
			continue;

		m_entities[i]->SetState(Entity::LIMBO);

	}

}
