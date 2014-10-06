#include "ModelSystem.h"

#include "../../Graphics/GraphicsManager.h"
#include "../Component/ModelComponent.h"
#include "../Component/PositionComponent.h"
#include "../Component/RotationComponent.h"
#include "../Component/ScaleComponent.h"
#include "../Component/ShatterComponent.h"


ModelSystem::ModelSystem(World* _world)
: Base(ComponentFilter().Requires<ModelComponent, PositionComponent, RotationComponent, ScaleComponent>(), _world)
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

	EntityMap::iterator it;

	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() != Entity::ALIVE)
		{
			GraphicsManager::GetInstance()->RemoveObject(e->GetId());
			continue;
		}


		if (!e->GetInitialized())
		{
			LoadModel(e->GetId());
		}


		bool change = false;

		position = e->GetComponent<PositionComponent>();
		rotation = e->GetComponent<RotationComponent>();
		scale = e->GetComponent<ScaleComponent>();
		model = e->GetComponent<ModelComponent>();

		if (!ISZERO(position->GetDeltaPosition()))
			change = true;
		else if (!rotation->HasChanged())
			change = true;
		else if (!ISZERO(scale->GetDeltaScale()))
			change = true;
                                
		if (change)
		{
#ifdef OPENGL
			model->m_worldMatrix = TRANSLATE(position->GetPosition()) * ROTATE(rotation->GetRotation())* SCALE(scale->GetScale());
#else
                        model->m_worldMatrix =  SCALE(scale->GetScale()) *ROTATE(rotation->GetRotation())* TRANSLATE(position->GetPosition());
#endif
			//TEMP
			position->Reset();
			rotation->Reset();
			scale->Reset();
		}
	}
        
        RunEvents();

}

void ModelSystem::RunEvents()
{
     //if(m_nextEvent == NONE)
     //       return;
     //
     //if(m_nextEvent == INITIALIZE)
     //{
     //    LoadModels();
     //    printf("YO\n");
     //}
     //
     //m_nextEvent = NONE;
}

//void ModelSystem::LoadModels()
//{
//    EntityMap::iterator it;
//    ModelComponent* model;
//    
//    for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
//    {
//        Entity* e = it->second;
//        model = e->GetComponent<ModelComponent>();
//        GraphicsManager::GetInstance()->AddObject(e->GetId(), model->m_modelPath, &model->m_worldMatrix, &model->m_worldMatrix);
//    }
//}

void ModelSystem::LoadModel(int _entityID)
{
    EntityMap::iterator it;
    ModelComponent* model;
    
    Entity* e = m_entityMap.find(_entityID)->second;

    model = e->GetComponent<ModelComponent>();

	auto shatter = e->GetComponent<ShatterComponent>();
	model->m_gpuId = GetMemoryID(e);

	if(shatter)
		GraphicsManager::GetInstance()->AddObject(model->m_gpuId, model->m_modelPath, &model->m_worldMatrix, &model->m_worldMatrix, &shatter->m_explosion);
	else
		GraphicsManager::GetInstance()->AddObject(model->m_gpuId, model->m_modelPath, &model->m_worldMatrix, &model->m_worldMatrix, 0);
	//GraphicsManager::GetInstance()->RemoveObject(model->m_gpuId);
	e->SetInitialized(true);
}

bool ModelSystem::Remove(Entity* _entity)
{
	GraphicsManager::GetInstance()->RemoveObject(GetMemoryID(_entity));
	return ISystem::Remove(_entity);
}
