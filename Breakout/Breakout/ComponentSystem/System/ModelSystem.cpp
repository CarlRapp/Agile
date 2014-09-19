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
float lol = 0;
void ModelSystem::Update(float _dt)
{
	GraphicsManager* manager = GraphicsManager::GetInstance();

	PositionComponent* position;
	RotationComponent* rotation;
	ScaleComponent* scale;
	ModelComponent* model;

	lol += _dt;

	for (int i = 0; i < m_entities.size(); ++i)
	{
		if ((m_entities[i]->GetState() == Entity::LIMBO) || m_entities[i]->GetState() == Entity::DEACTIVATED)
			continue;

		bool change = false;

		position = m_entities[i]->GetComponent<PositionComponent>();
		rotation = m_entities[i]->GetComponent<RotationComponent>();
		scale	 = m_entities[i]->GetComponent<ScaleComponent>();
		model	 = m_entities[i]->GetComponent<ModelComponent>();


		if (!ISZERO(position->m_deltaPosition))
			change = true;
		else if (!ISZERO(rotation->m_deltaRotation))
			change = true;
		else if (!ISZERO(scale->m_deltaScale))
			change = true;

		//TRANSLATE(model->m_worldMatrix,position->
		if (change)
		{
			model->m_worldMatrix = TRANSLATE(model->m_worldMatrix, position->m_position.x, position->m_position.y, position->m_position.z);
			GraphicsManager::GetInstance()->AddObject(m_entities[i]->GetId(), model->m_modelPath, &model->m_worldMatrix, &model->m_worldMatrix);
		}
		
		//model->m_worldMatrix = ROTATE(model->m_worldMatrix, 40, VECTOR3(0, 1, 0));

		

	}

}
