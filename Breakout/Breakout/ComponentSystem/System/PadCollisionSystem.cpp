#include "PadCollisionSystem.h"


PadCollisionSystem::PadCollisionSystem(World* _world)
: Base(ComponentFilter().Requires<PadCollisionComponent, CollisionComponent>(), _world)
{
}

PadCollisionSystem::~PadCollisionSystem()
{

}


void PadCollisionSystem::Update(float _dt)
{
	for (auto entityPair : m_entityMap)
	{
		Entity* e = entityPair.second;
		auto collision = e->GetComponent<CollisionComponent>();
		auto position = e->GetComponent<PositionComponent>()->GetPosition();
		auto scale = e->GetComponent<ScaleComponent>()->GetScale();
		
		std::vector<CollisionContact> collisions = collision->GetCollisions();
		for (unsigned int i = 0; i < collisions.size(); ++i)
		{
			CollisionContact* contact = &collisions.at(i);
			Entity* otherEntity = m_world->GetEntity(collisions[i].m_otherId);
			auto otherVelocityC = otherEntity->GetComponent<VelocityComponent>();
			b2WorldManifold manifold;
			contact->m_contact->GetWorldManifold(&manifold);
			if (manifold.normal.x == 0 && manifold.normal.y == 1)
			{
				float normalizedPointX = 1 - (manifold.points[0].x - position.x + scale.x / 2) / (scale.x);
				float angle = (0.1 * PI + 0.8 * PI * normalizedPointX);
				VECTOR3 otherVelocity = otherVelocityC->m_velocity;
				float speed = sqrtf(otherVelocity.x * otherVelocity.x + otherVelocity.y * otherVelocity.y);
				otherVelocityC->m_velocity = VECTOR3(cos(angle) * speed, sin(angle) * speed, otherVelocity.z);
			}
		}
	}
}