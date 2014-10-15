#include "SpawnPowerUpSystem.h"
#include "PhysicsSystem.h"
#include "../World.h"
#include "../Component/BlockComponent.h"
#include "../Component/MultiBallComponent.h"
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

	if (spawnPowerUp < 10)
	{
		Entity* _newPowerUp = CreatePowerUp(MULTIBALL);
		_newPowerUp->GetComponent<PositionComponent>()->SetPosition(_block->GetComponent<PositionComponent>()->GetPosition());
		_newPowerUp->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(rand() % 20 - 10, 5, 0);
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
	_entity->AddComponent<CollisionStatsComponent>(40.0f, 60.0f, 40.0f, 20.0f);
	_entity->AddComponent<HealthComponent>(10);
	_entity->AddComponent<EffectComponent>().m_effects.OnAdded = TRAIL;
	

	switch (_powerUp)
	{
	case SpawnPowerUpSystem::MULTIBALL:
		_entity->AddComponent<MultiBallComponent>();
		break;
	default:
		break;
	}
	return _entity;
}