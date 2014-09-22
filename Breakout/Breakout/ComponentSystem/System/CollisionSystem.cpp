#include "CollisionSystem.h"
#include "../Component/VelocityComponent.h"
#include "../Component/ModelComponent.h"

CollisionSystem::CollisionSystem(World* _world)
: Base(ComponentFilter().Requires<PositionComponent, CollisionComponent, VelocityComponent>(), _world)
{

}
CollisionSystem::~CollisionSystem()
{

}

void CollisionSystem::Update(float _dt)
{
	PositionComponent* position;
	CollisionComponent* collision;
	std::vector<Entity*> movingObjects = std::vector<Entity*>();

	EntityMap::iterator it;

	for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	{
		Entity* e = it->second;

		if (e->GetState() != Entity::ALIVE)
			continue;

		position = e->GetComponent<PositionComponent>();
		collision = e->GetComponent<CollisionComponent>();

		collision->m_position.x = position->m_position.x;
		collision->m_position.y = position->m_position.y;

		VelocityComponent* velocity = e->GetComponent<VelocityComponent>();
		if (!ISZERO(velocity->m_velocity))
			movingObjects.push_back(e);
	}

	CollisionComponent* ACol, *BCol;
	VelocityComponent* AVel, *BVel;
	for(Entity* A : movingObjects)
	{
		
		AVel = A->GetComponent<VelocityComponent>();
		ACol = A->GetComponent<CollisionComponent>();
		for (it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
		{
			Entity* B = it->second;
			if (A->GetId() == B->GetId())
				continue;

			BCol = B->GetComponent<CollisionComponent>();

			if (ACol->CollidesWith(BCol))
			{
				B->SetState(Entity::DEAD);
				A->SetState(Entity::DEAD);
				printf("LOL!");
				PositionComponent* bajs = A->GetComponent<PositionComponent>();
				bajs->m_position.y -= bajs->m_deltaPosition.y;
				AVel->m_velocity.y *= -1;
				//AVel->m_velocity.y *= -1.f;
				//bajs->m_position.y -= AVel->m_velocity.y;
				break;
			}
		}

	}
	movingObjects.clear();
}