#ifndef SPAWNENTITYCOMPONENT_H
#define SPAWNENTITYCOMPONENT_H

#include "../../stdafx.h"
#include "IComponent.h"

struct SpawnEntityComponent : Component<SpawnEntityComponent>
{
public:
	int m_entityType;
	VECTOR3 m_posFromEntity;

	SpawnEntityComponent() : m_entityType(0), m_posFromEntity(VECTOR3(0, 0, 0)) { }
	SpawnEntityComponent(int _entityType, VECTOR3 _posFromEntity) : m_entityType(_entityType), m_posFromEntity(_posFromEntity) { }

};

#endif