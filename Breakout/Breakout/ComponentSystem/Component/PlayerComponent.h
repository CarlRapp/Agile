#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "IComponent.h"

struct PlayerComponent : Component<PlayerComponent>
{
public:
	std::string m_name;

	PlayerComponent() : m_name("Default") { }
	PlayerComponent(int _playerLife) : m_name("Default") { }

};

#endif