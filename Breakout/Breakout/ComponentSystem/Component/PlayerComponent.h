#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "IComponent.h"

struct PlayerComponent : Component<PlayerComponent>
{
public:
	std::string m_name;
        float m_damage;
        float m_maxSpeedBall;
        float m_minSpeedBall;
        float m_size;
        
	PlayerComponent() : m_name("Default"),m_damage(1),m_maxSpeedBall(20.0f),m_minSpeedBall(10.0f),m_size(1.0f) { }
	PlayerComponent(int _playerLife) : m_name("Default"),m_damage(1),m_maxSpeedBall(20.0f),m_minSpeedBall(10.0f),m_size(1.0f) { }

};

#endif