#include "ModelSystem.h"

#include "../../Graphics/GraphicsManager.h"
#include "../Component/ModelComponent.h"
#include "../Component/PositionComponent.h"
#include "../Component/RotationComponent.h"
#include "../Component/ScaleComponent.h"


ModelSystem::ModelSystem(World* _world)
: Base(ComponentFilter().Requires<ModelComponent, PositionComponent, RotationComponent, ScaleComponent>(), _world)
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

	EntityMap::iterator it;

	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() != Entity::ALIVE)
		{
			GraphicsManager::GetInstance()->RemoveObject(e->GetId());
			continue;
		}
			

		bool change = false;

		position = e->GetComponent<PositionComponent>();
		rotation = e->GetComponent<RotationComponent>();
		scale = e->GetComponent<ScaleComponent>();
		model = e->GetComponent<ModelComponent>();


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
			GraphicsManager::GetInstance()->AddObject(e->GetId(), model->m_modelPath, &model->m_worldMatrix, &model->m_worldMatrix);
		}
	}

}