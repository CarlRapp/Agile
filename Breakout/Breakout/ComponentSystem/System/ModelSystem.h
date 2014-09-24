#ifndef MODELSYSTEM_H
#define MODELSYSTEM_H

#include "ISystem.h"

class ModelSystem : public System<ModelSystem>
{
private:
public:
    
	ModelSystem(World* _world);
	~ModelSystem();

	void Update(float _dt);
        

        
        //loads models into graphics
        //load all models at startup
        void LoadModels();
//        //load one model
       void LoadModel(int entityID);
        
        //runs methods depending on what's in m_nextEvent
        void RunEvents();
        
        
};

#endif