#include "InputSystem.h"
#include "../Component/InputComponent.h"
#include "../Component/PositionComponent.h"
#include "../World.h"
#include "../EntityFactory.h"

InputSystem::InputSystem(World* _world)
: Base(ComponentFilter().Requires<InputComponent, PositionComponent>(), _world)
{
	m_mouse = InputManager::GetInstance()->getInputDevices()->GetMouse();
	m_keyboard = InputManager::GetInstance()->getInputDevices()->GetKeyboard();
}

InputSystem::~InputSystem()
{

}


void InputSystem::Update(float _dt)
{
	InputComponent* input;

	for (int i = 0; i < m_entities.size(); ++i)
	{
		if ((m_entities[i]->GetState() == Entity::LIMBO) || m_entities[i]->GetState() == Entity::DEACTIVATED)
			continue;

		input = m_entities[i]->GetComponent<InputComponent>();

		input->m_controls.MouseDX = m_mouse->getdX();
		input->m_controls.MouseDY = m_mouse->getdY();

		if (m_keyboard->GetKeyState(32) == InputState::Pressed)
		{
			Entity* e;
			e = m_world->CreateEntity();
			EntityFactory::GetInstance()->CreateEntity(e, EntityFactory::BLOCK);
			e->GetComponent<PositionComponent>()->m_position = m_entities[i]->GetComponent<PositionComponent>()->m_position;
			e->GetComponent<PositionComponent>()->m_position.y += 2;
			m_world->AddEntity(e);
		}


	}
}