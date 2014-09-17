#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include "IComponent.h"
#include "../../Storage/FileManager.h"

struct ModelComponent : Component<ModelComponent>
{

	std::string m_modelPath;
	Vector3 m_offset;

	ModelComponent()
	{
		m_offset = Vector3(0, 0, 0);
		m_modelPath = "";
	}

};

#endif