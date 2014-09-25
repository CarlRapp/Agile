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
	EntityMap::iterator it;
	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;
		if (e->GetState() != Entity::ALIVE)
			continue;


		auto collision = e->GetComponent<CollisionComponent>();
		

		if (collision->IsAdded())
		{
			for (b2ContactEdge* contactEdge = collision->GetBody()->GetContactList(); contactEdge; contactEdge = contactEdge->next)
			{
				b2Contact* contact = contactEdge->contact;
				if (!contact->IsTouching())
				{
					const char* audioPath = e->GetComponent<AudioComponent>()->m_audioPath.c_str();
					m_audioManager->PlaySoundEffect("Wowpulse.wav");
				}
			}
		}


	}


}