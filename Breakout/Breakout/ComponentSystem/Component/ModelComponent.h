#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include "IComponent.h"
#include "../../Storage/FileManager.h"

struct ModelComponent : Component<ModelComponent>
{
	std::string m_modelPath= "";
	MATRIX4 m_worldMatrix= GETIMATRIX();
        
        ModelComponent()
	{

	}
        
        float m_explosion = 0.0f;
        float m_maxExplosionTime = 3.0f;
        
        enum Explosion
        {
            NONE,
            EXPLODING,
            DONE
        };
        
        Explosion m_explosionState = NONE;


        
        Explosion IsExploding()
        {
            return m_explosionState;
        }

};

#endif