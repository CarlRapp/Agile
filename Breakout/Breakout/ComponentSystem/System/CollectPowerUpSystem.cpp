#include "CollectPowerUpSystem.h"
#include "../Component/CollisionComponent.h"
#include "PhysicsSystem.h"
#include "ShootLaserSystem.h"
#include "../World.h"

#include "../EntityFactory.h"
#include "../Component/PositionComponent.h"
#include "../Component/VelocityComponent.h"
#include "../Component/BallComponent.h"
#include "../Component/MultiBallComponent.h"
#include "../Component/LaserComponent.h"

CollectPowerUpSystem::CollectPowerUpSystem(World* _world)
: Base(ComponentFilter().Requires<CollisionComponent>(), _world)
{
}

CollectPowerUpSystem::~CollectPowerUpSystem()
{

}

void CollectPowerUpSystem::Update(float _dt)
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
			CollisionContact contact = collisions.at(i);
			int bitA = contact.m_fixture->GetFilterData().categoryBits;
			int bitB = contact.m_otherFixture->GetFilterData().categoryBits;
			
			if ((bitA == CollisionCategory::POWERUP && bitB == CollisionCategory::PAD) || (bitA == CollisionCategory::PAD && bitB == CollisionCategory::POWERUP))
			{
				Entity* powerUp = (bitA == CollisionCategory::POWERUP) ? e : m_world->GetEntity(collisions[i].m_otherId);
				if (powerUp->GetState() == Entity::ALIVE)
					TriggerPowerUp(powerUp);
			}

		}
	}
}
void CollectPowerUpSystem::TriggerPowerUp(Entity* _powerUp)
{
	auto isMultiBall = _powerUp->GetComponent<MultiBallComponent>();
	auto isLaser = _powerUp->GetComponent<LaserComponent>();

	if (isMultiBall)
		SpawnMultiBalls();
	if (isLaser)
	{
		ShootLaserSystem* shootLaserSystem = m_world->GetSystem<ShootLaserSystem>();
		if (shootLaserSystem)
			shootLaserSystem->ResetDuration();
		else
			m_world->AddSystem<ShootLaserSystem>();
	}


	_powerUp->SetState(Entity::SOON_DEAD);
}

void CollectPowerUpSystem::SpawnMultiBalls()
{
	std::vector<Entity*>* allBalls = m_world->GetEntities<BallComponent>();
	int tSize = allBalls->size();
	for (int i = 0; i < tSize; ++i)
	{
		Entity* tBall = allBalls->at(i);
		if (tBall->GetState() != Entity::ALIVE)
			continue;

		PositionComponent* POS = tBall->GetComponent<PositionComponent>();
		VelocityComponent* VEL = tBall->GetComponent<VelocityComponent>();
		Entity* newBall = m_world->CreateEntity();
		EntityFactory::GetInstance()->CreateEntity(newBall, EntityFactory::BALL);
		newBall->GetComponent<ScaleComponent>()->SetScale(VECTOR3(0.8f, 0.8f, 0.8f));
		newBall->GetComponent<EffectComponent>()->m_effects.OnAdded = EffectFlags::TRAIL;

		VECTOR3 originalVel = NORMALIZE(VEL->m_velocity);
		float velLength = sqrtf(originalVel.x * originalVel.x + originalVel.y * originalVel.y);
		float velAngle = atan2f(originalVel.y, originalVel.x);
		VECTOR2 aVelocity = VECTOR2
			(
			cos(velAngle + PI*0.25f)*velLength,
			sin(velAngle + PI*0.25f)*velLength
			);
		VECTOR2 bVelocity = VECTOR2
			(
			cos(velAngle - PI*0.25f)*velLength,
			sin(velAngle - PI*0.25f)*velLength
			);

		newBall->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(bVelocity.x, bVelocity.y, 0);
		newBall->GetComponent<PositionComponent>()->SetPosition(POS->GetPosition() + VECTOR3(2.f*bVelocity.x, 2.f*bVelocity.y, 0));
		tBall->GetComponent<VelocityComponent>()->m_velocity = VECTOR3(aVelocity.x, aVelocity.y, 0);
		tBall->GetComponent<PositionComponent>()->SetPosition(POS->GetPosition() + VECTOR3(2.f*aVelocity.x, 2.f*aVelocity.y, 0));

		m_world->AddEntity(newBall);
	}

}