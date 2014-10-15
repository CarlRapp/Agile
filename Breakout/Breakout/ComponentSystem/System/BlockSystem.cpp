#include "BlockSystem.h"
#include "../Component/PositionComponent.h"
#include "../Component/ScaleComponent.h"
#include "../Component/CollisionComponent.h"
#include "../Component/ScoreComponent.h"
#include "../Component/TNTComponent.h"
#include "../Component/EffectComponent.h"
#include "../World.h"


BlockSystem::BlockSystem(World* _world)
: Base(ComponentFilter().Requires<BlockComponent, CollisionComponent>(), _world)
{
	m_blockGrid = 0;
	SetSettings(20, 15, 20, 0);
}

BlockSystem::~BlockSystem()
{
	for (int i = 0; i < m_dimensionY; ++i)
		SafeDeleteArray(m_blockGrid[i]);

	SafeDeleteArray(m_blockGrid);
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

		for (int i = 0; i < m_dimensionY; ++i)
			SafeDeleteArray(m_blockGrid[i]);

		SafeDeleteArray(m_blockGrid);
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

	VECTOR2 blockDimension = _block->GetComponent<BlockComponent>()->GetDimension();
	if (blockDimension.x > 1)
	{
		xPos = (xPos + blockDimension.x < m_dimensionX) ? xPos : xPos - blockDimension.x;
		for (int n = 0; n < blockDimension.x; ++n)
		{
			Entity* tExisting = m_blockGrid[yPos][xPos + n];
			if (tExisting)
			{
				PushDown(xPos + n, yPos);
				m_blockGrid[yPos][xPos + n] = 0;
			}

				
		}
		for (int n = 0; n < blockDimension.x; ++n)
			SpawnBlockAt(_block, xPos + n, yPos);

		MoveToWorldPosition(_block, xPos, yPos);
	}
	else
		SpawnBlockAt(_block, xPos, yPos);

	MoveToWorldPosition(_block, xPos, yPos);
}

void BlockSystem::OnEntityRemoved(Entity* _e)
{
	int X, Y;
	FindBlock(_e, X, Y);
	if (X == -1 || Y == -1)
		return;

	VECTOR2 mDimension = _e->GetComponent<BlockComponent>()->GetDimension();
	int xPos = X;
	int yPos = Y;
	while (xPos - 1 >= 0 && m_blockGrid[yPos][xPos - 1] == _e)
		--xPos;
	while (yPos - 1 >= 0 && m_blockGrid[yPos - 1][xPos] == _e)
		--yPos;

	//	Set all cells for this block to 0
	for (int n = 0; n < mDimension.x; ++n)
		m_blockGrid[Y][X + n] = 0;


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
					|| (_x + X >= m_dimensionX || _y + Y >= m_dimensionY)
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



	std::vector<ScoreComponent> collectedScore = std::vector<ScoreComponent>();
	std::map<GridPosition, bool> blocksToCheck = std::map<GridPosition, bool>();
	//	Check LEFT
	if (X - 1 >= 0)
		for (int n = 0; n < mDimension.y; ++n)
			blocksToCheck[GridPosition(X - 1, Y - n)] = false;
			//CheckIndividualBlock(X - 1, Y + n);

	//	Check RIGHT
	if (X + mDimension.x < m_dimensionX)
		for (int n = 0; n < mDimension.y; ++n)
			blocksToCheck[GridPosition(X + (int)mDimension.x, Y + n)] = false;
			//CheckIndividualBlock(X + mDimension.x, Y + n);

	//	Check DOWN
	if (Y + mDimension.y < m_dimensionY)
		for (int n = 0; n < mDimension.x; ++n)
			blocksToCheck[GridPosition(X + n, Y + (int)mDimension.y)] = false;
			//CheckIndividualBlock(X + n, Y + mDimension.y);

	//	Check UP
	if (Y - 1 >= 0)
		for (int n = 0; n < mDimension.x; ++n)
			blocksToCheck[GridPosition(X + n, Y - 1)] = false;
			//CheckIndividualBlock(X + n, Y - 1);
	
	CheckGroupOfBlocks(&blocksToCheck);
}

void BlockSystem::CheckGroupOfBlocks(std::map<GridPosition, bool>* _blocks)
{

	std::map<GridPosition, bool>::iterator gIT;
	for (gIT = _blocks->begin(); gIT != _blocks->end(); ++gIT)
	{
		if (gIT->second)
			continue;

		int xPos = gIT->first.first;
		int yPos = gIT->first.second;
		if (m_blockGrid[yPos][xPos])
		{
			//	Get all blocks attached to the current block
			std::map<GridPosition, bool> blockGroup = std::map<GridPosition, bool>();
			GetBlocksAttachedTo(xPos, yPos, &blockGroup);

			//	If the group can reach the root
			//	check if any of the other blocks in _blocks
			//	is in this group, which means we don't have tot check those.
			if (GroupCanReachRoot(&blockGroup))
			{
				std::map<GridPosition, bool>::iterator alreadyExists;
				for (alreadyExists = blockGroup.begin(); alreadyExists != blockGroup.end(); ++alreadyExists)
				{
					if (blockGroup.find(alreadyExists->first) != blockGroup.end())
						alreadyExists->second = true;
				}
			}
			else
			{
				//	Remove all the blocks that couldn't reach the root
				std::map<GridPosition, bool>::iterator destroyBlock;
				for (destroyBlock = blockGroup.begin(); destroyBlock != blockGroup.end(); ++destroyBlock)
				{
					m_blockGrid[destroyBlock->first.second][destroyBlock->first.first]->SetState(Entity::SOON_DEAD);
					auto effect = m_blockGrid[destroyBlock->first.second][destroyBlock->first.first]->GetComponent<EffectComponent>();
					if (effect)
						effect->m_effects.OnRemoved = EffectFlags::SHATTER;

					m_blockGrid[destroyBlock->first.second][destroyBlock->first.first] = 0;
				}
			}


		}
		gIT->second = true;
	}
}

void BlockSystem::SpawnBlockAt(Entity* _block, int _x, int _y)
{
	Entity* existingBlock = m_blockGrid[_y][_x];

	if (existingBlock)
		PushDown(_x, _y);

	m_blockGrid[_y][_x] = _block;
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
		if (e && e->GetComponent<BlockComponent>())
		{
			int xPos = GP.first;
			int yPos = GP.second;
			while (xPos - 1 >= 0 && m_blockGrid[yPos][xPos - 1] == e)
				--xPos;

			VECTOR2 blockDimension = e->GetComponent<BlockComponent>()->GetDimension();
			for (int n = 0; n < blockDimension.x; ++n)
			{
				if (yPos + 1 < m_dimensionY)
				{
					m_blockGrid[yPos + 1][xPos + n] = m_blockGrid[yPos][xPos + n];
					m_blockGrid[yPos][xPos + n] = 0;
				}
				else
					e->SetState(Entity::SOON_DEAD);
			}

			MoveToWorldPosition(e, xPos, yPos + 1);
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
	VECTOR2 size = _block->GetComponent<BlockComponent>()->GetSize();
	VECTOR2 dimension = _block->GetComponent<BlockComponent>()->GetDimension();
	position.x += (dimension.x - 1) * 0.50f;

	position.x *= size.x;
	position.y *= size.y;
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