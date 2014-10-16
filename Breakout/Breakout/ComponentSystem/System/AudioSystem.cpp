#include "AudioSystem.h"
#include "../Component/CollisionComponent.h"
#include "../Component/AudioComponent.h"
#include "../World.h"
#include "../EntityFactory.h"

AudioSystem::AudioSystem(World* _world)
: Base(ComponentFilter().Requires<AudioComponent, CollisionComponent>(), _world)
{
	m_audioManager = AudioManager::GetInstance();
}

AudioSystem::~AudioSystem()
{
}


void AudioSystem::Update(float _dt)
{
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;

		if ((e->GetState() != Entity::ALIVE))
			continue;

		auto collision = e->GetComponent<CollisionComponent>();
		std::vector<CollisionContact> collisions = collision->GetCollisions();
		for (unsigned int i = 0; i < collisions.size(); ++i)
		{
			const char* audioPath = e->GetComponent<AudioComponent>()->m_audioPath.c_str();
			m_audioManager->PlaySoundEffect(audioPath);
		}
	}

}