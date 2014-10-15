#include "InputSystem.h"
#include "../Component/MouseInputComponent.h"
#include "../Component/KeyboardInputComponent.h"
#include "../Component/PositionComponent.h"
#include "../World.h"
#include "../EntityFactory.h"

InputSystem::InputSystem(World* _world)
: Base(ComponentFilter().RequiresOneOf<MouseInputComponent, KeyboardInputComponent>(), _world)
{
	m_mouse = InputManager::GetInstance()->getInputDevices()->GetMouse();
	m_keyboard = InputManager::GetInstance()->getInputDevices()->GetKeyboard();
}

InputSystem::~InputSystem()
{

}


void InputSystem::Update(float _dt)
{
	float dx = m_mouse->GetdX() * 20.f * 1280.0f;
	float dy = m_mouse->GetdY() * 20.f * 720.0f;
	InputState lButton = m_mouse->GetButtonState(MouseButton::LeftMB);
	InputState rButton = m_mouse->GetButtonState(MouseButton::RightMB);
	InputState mButton = m_mouse->GetButtonState(MouseButton::MiddleMB);


	EntityMap::iterator it;
	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() != Entity::ALIVE)
			continue;

		if (e->HasComponent<MouseInputComponent>())
		{
			MouseInputComponent* c = e->GetComponent<MouseInputComponent>();
			c->m_controls.LeftButton = lButton;
			c->m_controls.RightButton = rButton;
			c->m_controls.MiddleButton = mButton;

			c->m_controls.MouseDX = dx;
			c->m_controls.MouseDY = dy;
		}
		if (e->HasComponent <KeyboardInputComponent>())
		{
			KeyboardInputComponent* c = e->GetComponent<KeyboardInputComponent>();

			for (std::map<int, InputState>::iterator i = c->m_keys.begin(); i != c->m_keys.end(); ++i)
				i->second = m_keyboard->GetKeyState(i->first);

		}


	}


}