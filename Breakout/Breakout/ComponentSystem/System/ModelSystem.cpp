#include "ModelSystem.h"

#include "../../Graphics/GraphicsManager.h"
#include "../Component/ModelComponent.h"
#include "../Component/PositionComponent.h"
#include "../Component/RotationComponent.h"
#include "../Component/ScaleComponent.h"

ModelSystem::ModelSystem(void)
: Base(ComponentFilter().Requires<ModelComponent, PositionComponent, RotationComponent, ScaleComponent>())
{
}

ModelSystem::~ModelSystem()
{
}

void ModelSystem::Update(float _dt)
{
	GraphicsManager* manager = GraphicsManager::GetInstance();

	PositionComponent* position;
	RotationComponent* rotation;
	ScaleComponent* scale;
	ModelComponent* model;


	for (int i = 0; i < m_entities.size(); ++i)
	{
		if ((m_entities[i]->GetState() == Entity::LIMBO) || m_entities[i]->GetState() == Entity::DEACTIVATED)
			continue;

		bool change = false;

		position = m_entities[i]->GetComponent<PositionComponent>();
		rotation = m_entities[i]->GetComponent<RotationComponent>();
		scale	 = m_entities[i]->GetComponent<ScaleComponent>();
		model	 = m_entities[i]->GetComponent<ModelComponent>();


		if (position->m_deltaPosition != Vector3(0, 0, 0))
			change = true;
		else if (rotation->m_deltaRotation != Vector3(0, 0, 0))
			change = true;
		else if (scale->m_deltaScale != Vector3(0, 0, 0))
			change = true;


		if (change)
		{
			//TRANSLATE(model->m_worldMatrix,position->
			// Calc worldmatrix
		}



	}

}
