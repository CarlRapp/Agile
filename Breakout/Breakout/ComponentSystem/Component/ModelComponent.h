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
        
        // TODO: Flytta till Explosionskomponent eller effektkomponent
        
        ///-------------------------___EXPLOSIONS___-------------------------///
        
        float m_explosion = 0.0f;
        float m_maxExplosionTime = 1.2f;
        float m_expansionRate = 20.0f;
        
        enum ExplosionState
        {
            NONE = 0,
            EXPLODING,
            DONE
        };
        
        ExplosionState m_explosionState = NONE;

        ExplosionState IsExploding(float _dt)
        {
            if(m_explosionState == EXPLODING)
            {
                m_explosion += _dt * m_expansionRate;
                
				if (m_explosion > m_maxExplosionTime * m_expansionRate)
                {
                    m_explosionState = DONE;
                    printf("Entity done exploding, remove\n");
                }
            }
            
            return m_explosionState;
        }
        
        void Explode()
        {
            m_explosionState = EXPLODING;
            printf("Explode Entity\n");
        }

        ///-------------------------___EXPLOSIONS___-------------------------///


};

#endif