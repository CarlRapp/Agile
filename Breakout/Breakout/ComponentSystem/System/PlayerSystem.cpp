#include "PlayerSystem.h"
#include "../Component/VelocityComponent.h"
#include "../Component/InputComponent.h"
#include "../World.h"

PlayerSystem::PlayerSystem(World* _world)
: Base(ComponentFilter().Requires<InputComponent, VelocityComponent>(), _world)
{
}

PlayerSystem::~PlayerSystem()
{

}

//int min = 0; // Window min X
//int max = 1000; // Window max X
//int scaleMin = 0; // Gameboard min X
//int scaleMax = 15; // Gameboard max X

//x = x < 0 ? x = 0 : x = x;
//x = x > 1000 ? x = 1000 : x = x;

//float upper = (scaleMax - scaleMin) * (x - min);
//float under = max + min;
//float resualt = (upper / under) + scaleMin;


void PlayerSystem::Update(float _dt)
{
	InputComponent* input;
	VelocityComponent* velocity;



	
	for (int i = 0; i < m_entities.size(); ++i)
	{
		if ((m_entities[i]->GetState() == Entity::LIMBO) || m_entities[i]->GetState() == Entity::DEACTIVATED)
			continue;

		input = m_entities[i]->GetComponent<InputComponent>();
		velocity = m_entities[i]->GetComponent<VelocityComponent>();

		velocity->m_velocity.x = input->m_controls.MouseDX;

		/*if (input->m_controls.MouseDY != 0)
			velocity->m_velocity.y = input->m_controls.MouseDY;*/
	}

}