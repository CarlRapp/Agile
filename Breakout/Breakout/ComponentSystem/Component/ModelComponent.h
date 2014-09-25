#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include "IComponent.h"
#include "../../Storage/FileManager.h"

struct ModelComponent : Component<ModelComponent>
{
	std::string m_modelPath;
	MATRIX4 m_worldMatrix;

	ModelComponent()
	{
		m_modelPath = "";

        m_worldMatrix = GETIMATRIX();
	}

};

#endif