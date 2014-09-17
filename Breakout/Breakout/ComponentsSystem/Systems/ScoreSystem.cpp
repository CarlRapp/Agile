#include "ScoreSystem.h"
#include "../Components/LifeComponent.h"
#include "../Components/ScoreComponent.h"
#include "../../Input/InputManager.h"

ScoreSystem::ScoreSystem()
: Base(ComponentFilter().Requires<ScoreComponent>().Excludes<LifeComponent>())
{
}

ScoreSystem::~ScoreSystem()
{

}

void ScoreSystem::AddPlayer(Entity* _player)
{
	m_player = _player;
}


void ScoreSystem::Update(double _dt)
{
	if (m_player == 0)
		return;

	Mouse* mouse = InputManager::GetInstance()->getInputDevices()->GetMouse();
	InputManager::GetInstance()->Update();

	if (mouse->GetButtonState(VK_LBUTTON) == InputState::Pressed)
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			if ((m_entities[i]->GetState() == Entity::LIMBO) || m_entities[i]->GetState() == Entity::DEACTIVATED)
				continue;

			m_entities[i]->GetComponent<ScoreComponent>()->wasHit = true;
		}
	}


	for (int i = 0; i < m_entities.size(); ++i)
	{
		if ((m_entities[i]->GetState() == Entity::LIMBO) || m_entities[i]->GetState() == Entity::DEACTIVATED)
			continue;

		m_player->GetComponent<ScoreComponent>()->m_score += m_entities[i]->GetComponent<ScoreComponent>()->m_score;

	}

	printf("Score: %i\n", m_player->GetComponent<ScoreComponent>()->m_score);
}