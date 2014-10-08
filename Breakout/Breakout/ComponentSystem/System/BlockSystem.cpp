#include "BlockSystem.h"
#include "../Component/PositionComponent.h"
#include "../Component/ScaleComponent.h"
#include "../Component/CollisionComponent.h"
#include "../Component/TNTComponent.h"
#include "../Component/EffectComponent.h"
#include "../World.h"


BlockSystem::BlockSystem(World* _world)
: Base(ComponentFilter().Requires<BlockComponent, CollisionComponent>(), _world)
{
	m_blockGrid = 0;
	SetSettings(40, 15, 20, 0);
}

BlockSystem::~BlockSystem()
{
}

void BlockSystem::SetSettings(int _sizeX, int _sizeY, int _topCenterY, int _topCenterX)
{
	m_dimensionX = _sizeX;
	m_dimensionY = _sizeY;
	m_topCenterX = _topCenterX;
	m_topCenterY = _topCenterY;

	printf("#################\n");
	printf("Blocksystem Settings\n");
	printf("Dimension: %d x %d\n", m_dimensionX, m_dimensionY);
	printf("TopCenter(%d, %d)\n", m_topCenterX, m_topCenterY);
	printf("#################\n");

	if (m_blockGrid)
	{
		printf("Deleting BlockGRID!\n");
		delete[] m_blockGrid;
		m_blockGrid = 0;
	}

	m_blockGrid = new Entity**[m_dimensionY];
	for (int y = 0; y < m_dimensionY; ++y)
	{
		m_blockGrid[y] = new Entity*[m_dimensionX];
		for (int x = 0; x < m_dimensionX; ++x)
			m_blockGrid[y][x] = 0;
	}
		
}

void BlockSystem::Update(float _dt)
{


}

void BlockSystem::OnEntityAdded(Entity* _block)
{
	int xPos = rand() % m_dimensionX;
	int yPos = 0;

	Entity* existingBlock = m_blockGrid[yPos][xPos];

	if (existingBlock)
		PushDown(xPos, yPos);

	MoveToWorldPosition(_block, xPos, yPos);
	m_blockGrid[yPos][xPos] = _block;
}

void BlockSystem::OnEntityRemoved(Entity* _e)
{
	int X, Y;
	FindBlock(_e, X, Y);
	if (X == -1 || Y == -1)
		return;
	m_blockGrid[Y][X] = 0;


	auto tnt = _e->GetComponent<TNTComponent>();
	if (tnt)
	{
		Entity* block;

		for (int _x = -1; _x < 2; ++_x)
		{
			for (int _y = -1; _y < 2; ++_y)
			{
				//if _x & _y = 0, or if we are outside bounds ( < 0 )
				if (   (_x == 0 && _y == 0)
					|| ( _x + X < 0 || _y + Y < 0)
				   )
					continue;

				block = m_blockGrid[Y + _y][X + _x];
				if (block)
				{
					auto effect = block->GetComponent<EffectComponent>();
					if (effect)
						effect->m_effects.OnRemoved = EffectFlags::EXPLODE;

					block->SetState(Entity::SOON_DEAD);
				}

			}
		}

		return;
	}





	//	Check LEFT
	if (X - 1 >= 0)
	{
		Entity* block = m_blockGrid[Y][X - 1];
		if (block)
		{
			std::map<GridPosition, bool> blockGroup = std::map<GridPosition, bool>();
			GetBlocksAttachedTo(X-1, Y, &blockGroup);

			if (!GroupCanReachRoot(&blockGroup))
			{
				std::map<GridPosition, bool>::iterator gIT;
				for (gIT = blockGroup.begin(); gIT != blockGroup.end(); ++gIT)
				{
					m_blockGrid[gIT->first.second][gIT->first.first]->SetState(Entity::SOON_DEAD);
					m_blockGrid[gIT->first.second][gIT->first.first] = 0;
				}
					
			}
		}
	}
	//	Check RIGHT
	if (X + 1 < m_dimensionX)
	{
		Entity* block = m_blockGrid[Y][X + 1];
		if (block)
		{
			std::map<GridPosition, bool> blockGroup = std::map<GridPosition, bool>();
			GetBlocksAttachedTo(X + 1, Y, &blockGroup);

			if (!GroupCanReachRoot(&blockGroup))
			{
				std::map<GridPosition, bool>::iterator gIT;
				for (gIT = blockGroup.begin(); gIT != blockGroup.end(); ++gIT)
				{
					m_blockGrid[gIT->first.second][gIT->first.first]->SetState(Entity::SOON_DEAD);
					m_blockGrid[gIT->first.second][gIT->first.first] = 0;
				}
					
					
			}
		}
	}
	//	Check DOWN
	if (Y + 1 < m_dimensionY)
	{
		Entity* block = m_blockGrid[Y + 1][X];
		if (block)
		{
			std::map<GridPosition, bool> blockGroup = std::map<GridPosition, bool>();
			GetBlocksAttachedTo(X, Y + 1, &blockGroup);

			if (!GroupCanReachRoot(&blockGroup))
			{
				std::map<GridPosition, bool>::iterator gIT;
				for (gIT = blockGroup.begin(); gIT != blockGroup.end(); ++gIT)
				{
					m_blockGrid[gIT->first.second][gIT->first.first]->SetState(Entity::SOON_DEAD);
					m_blockGrid[gIT->first.second][gIT->first.first] = 0;
				}

			}
		}
	}
	//	Check UP
	if (Y - 1 >= 0)
	{
		Entity* block = m_blockGrid[Y - 1][X];
		if (block)
		{
			std::map<GridPosition, bool> blockGroup = std::map<GridPosition, bool>();
			GetBlocksAttachedTo(X, Y - 1, &blockGroup);

			if (!GroupCanReachRoot(&blockGroup))
			{
				std::map<GridPosition, bool>::iterator gIT;
				for (gIT = blockGroup.begin(); gIT != blockGroup.end(); ++gIT)
				{
					m_blockGrid[gIT->first.second][gIT->first.first]->SetState(Entity::SOON_DEAD);
					m_blockGrid[gIT->first.second][gIT->first.first] = 0;
				}
			}
		}
	}
	
}

void BlockSystem::PushDown(int _x, int _y)
{
	if (!(_x >= 0 && _x < m_dimensionX && _y >= 0 && _y < m_dimensionY))
		return;

	std::map<GridPosition, bool> blockGroup = std::map<GridPosition, bool>();
	GetBlocksAttachedTo(_x, _y, &blockGroup);
	GridQueue sortedPositions = GridQueue();

	std::map<GridPosition, bool>::iterator gIT;
	for (gIT = blockGroup.begin(); gIT != blockGroup.end(); ++gIT)
		sortedPositions.push(gIT->first);

	int tSize = sortedPositions.size();
	for (int i = 0; i < tSize; ++i)
	{
		GridPosition GP = sortedPositions.top();
		Entity* e = m_blockGrid[GP.second][GP.first];
		if (e)
		{
			MoveToWorldPosition(e, GP.first, GP.second + 1);
			if (GP.second + 1 < m_dimensionY)
			{
				m_blockGrid[GP.second + 1][GP.first] = m_blockGrid[GP.second][GP.first];
				m_blockGrid[GP.second][GP.first] = 0;
			}
			else
				e->SetState(Entity::SOON_DEAD);
		}
			
		sortedPositions.pop();
	}
}

bool BlockSystem::GroupCanReachRoot(std::map<GridPosition, bool>* _blockGroup)
{
	std::map<GridPosition, bool>::iterator gIT;
	for (gIT = _blockGroup->begin(); gIT != _blockGroup->end(); ++gIT)
		if (gIT->first.second == 0)
			return true;

	return false;
}

bool BlockSystem::HasCollisionComponent(Entity* _block)
{
	if (_block)
	{
		auto collision = _block->GetComponent<CollisionComponent>();
		if (collision)
			return true;
	}

	return false;
}

void BlockSystem::GetBlocksAttachedTo(int _x, int _y, std::map<GridPosition, bool>* _closedList)
{
	if (_closedList->find(GridPosition(_x, _y)) != _closedList->end())
		return;

	(*_closedList)[GridPosition(_x, _y)] = true;

	if (_x - 1 >= 0)
		if (HasCollisionComponent(m_blockGrid[_y][_x - 1]))
			GetBlocksAttachedTo(_x - 1, _y, _closedList);
	if (_x + 1 < m_dimensionX)
		if (HasCollisionComponent(m_blockGrid[_y][_x + 1]))
			GetBlocksAttachedTo(_x + 1, _y, _closedList);
	if (_y - 1 >= 0)
		if (HasCollisionComponent(m_blockGrid[_y - 1][_x]))
			GetBlocksAttachedTo(_x, _y - 1, _closedList);
	if (_y + 1 < m_dimensionY)
		if (HasCollisionComponent(m_blockGrid[_y + 1][_x]))
			GetBlocksAttachedTo(_x, _y + 1, _closedList);

}

void BlockSystem::MoveToWorldPosition(Entity* _block, int _x, int _y)
{
	PositionComponent* positionC = _block->GetComponent<PositionComponent>();
	if (!positionC)
		return;
	VECTOR3 position = VECTOR3(_x - (m_dimensionX*0.5f), -_y, 0);

	ScaleComponent* scaleC = _block->GetComponent<ScaleComponent>();
	if(scaleC)
	{
		VECTOR3 scale = scaleC->GetScale();
		position.x *= scale.x;
		position.y *= scale.y;
		position.z *= scale.z;
	}

	position.x += m_topCenterX;
	position.y += m_topCenterY;
	positionC->SetPosition(position);

	CollisionComponent* collisionC = _block->GetComponent<CollisionComponent>();
	if (collisionC)
		collisionC->GetBody()->SetTransform(b2Vec2(position.x, position.y), 0);
}

void BlockSystem::FindBlock(Entity* _e, int& _x, int& _y)
{
	_x = -1;
	_y = -1;

	for (int y = 0; y < m_dimensionY; ++y)
		for (int x = 0; x < m_dimensionX; ++x)
			if (m_blockGrid[y][x])
				if (m_blockGrid[y][x]->GetId() == _e->GetId())
				{
					_x = x;
					_y = y;
					return;
				}
}