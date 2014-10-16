#include <time.h>
#include "SpawnPowerUpSystem.h"
#include "PhysicsSystem.h"
#include "../World.h"
#include "../Component/BlockComponent.h"
#include "../Component/MultiBallComponent.h"
#include "../Component/LaserComponent.h"
#include "../../Audio/AudioManager.h"
SpawnPowerUpSystem::SpawnPowerUpSystem(World* _world)
: Base(ComponentFilter().Requires<BlockComponent>(), _world)
{
}

SpawnPowerUpSystem::~SpawnPowerUpSystem()
{
}

void SpawnPowerUpSystem::Update(float _dt)
{

}

void SpawnPowerUpSystem::OnEntityRemoved(Entity* _block)
{
	int spawnPowerUp = rand() % 100;

	if (spawnPowerUp <= 2)
	{
		spawnPowerUp = rand() % 2;
		Entity* _newPowerUp = 0;
		switch (spawnPowerUp)
		{
		case 0:
			_newPowerUp = CreatePowerUp(SHOOTLASER);
			break;
		case 1:
			_newPowerUp = CreatePowerUp(MULTIBALL);
			break;
		}

		_newPowerUp->GetComponent<PositionComponent>()->SetPosition(_block->GetComponent<PositionComponent>()->GetPosition());
		_newPowerUp->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(rand() % 60 - 30, 15, 0);
		m_world->AddEntity(_newPowerUp);
		AudioManager::GetInstance()->PlaySoundEffect("PowerUp_Spawn.wav");
	}
}

Entity* SpawnPowerUpSystem::CreatePowerUp(PowerUpType _powerUp)
{
	Entity* _entity = m_world->CreateEntity();
	b2BodyDef* bodyDef = new b2BodyDef();
	std::vector<b2FixtureDef*> fixDefs = std::vector<b2FixtureDef*>();

	_entity->AddComponent<PositionComponent>();
	_entity->AddComponent<RotationComponent>();
	_entity->AddComponent<ScaleComponent>();
	_entity->AddComponent<ModelComponent>().m_modelPath = "sphere";
	_entity->AddComponent<VelocityComponent>();
	PhysicsSystem::GenerateBody(EntityFactory::POWERUP, bodyDef, fixDefs);
	_entity->AddComponent<CollisionComponent>(bodyDef, fixDefs);

	switch (_powerUp)
	{
	case SpawnPowerUpSystem::MULTIBALL:
		_entity->AddComponent<MultiBallComponent>();
		break;
	case SpawnPowerUpSystem::SHOOTLASER:
		_entity->AddComponent<LaserComponent>();
		break;
	default:
		break;
	}
	return _entity;
}