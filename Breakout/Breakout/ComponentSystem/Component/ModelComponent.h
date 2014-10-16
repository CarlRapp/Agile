#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include "IComponent.h"
#include "../../Storage/FileManager.h"

struct ModelComponent : Component<ModelComponent>
{
	std::string m_modelPath = "";
	MATRIX4 m_worldMatrix = GETIMATRIX();
	bool m_render;
        
        ModelComponent()
	{
			m_render = true;
	}

};

#endif