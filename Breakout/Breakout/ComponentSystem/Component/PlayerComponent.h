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
        std::string m_expString;
        int m_level;
        
	PlayerComponent() : m_name("_"),m_damage(1),m_maxSpeedBall(20.0f),m_minSpeedBall(10.0f),m_size(1.0f),m_expString("EXP <          > LEVEL: 0"),m_level(0) { }
	PlayerComponent(int _playerLife) : m_name("_"),m_damage(1),m_maxSpeedBall(20.0f),m_minSpeedBall(10.0f),m_size(1.0f),m_expString("EXP <          > LEVEL: 0"),m_level(0) { }
        
        void AddLevel(int _levelUp)
        {
            m_level += _levelUp;
        }
};

#endif