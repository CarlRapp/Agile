#include "BlockSystem.h"
#include "../Component/PositionComponent.h"
#include "../Component/ScaleComponent.h"
#include "../Component/CollisionComponent.h"
#include "../World.h"


BlockSystem::BlockSystem(World* _world)
: Base(ComponentFilter().Requires<BlockComponent, CollisionComponent>(), _world)
{
	m_blockGrid = 0;
	SetSettings(20, 15, 20, 0);
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

	//	Check LEFT
	if (X - 1 >= 0)
		for (int n = 0; n < mDimension.y; ++n)
			CheckIndividualBlock(X - 1, Y + n);

	//	Check RIGHT
	if (X + mDimension.x < m_dimensionX)
		for (int n = 0; n < mDimension.y; ++n)
			CheckIndividualBlock(X + mDimension.x, Y + n);

	//	Check DOWN
	if (Y + mDimension.y < m_dimensionY)
		for (int n = 0; n < mDimension.x; ++n)
			CheckIndividualBlock(X + n, Y + mDimension.y);

	//	Check UP
	if (Y - 1 >= 0)
		for (int n = 0; n < mDimension.x; ++n)
			CheckIndividualBlock(X + n, Y - 1);
	
}
void BlockSystem::CheckIndividualBlock(int _x, int _y)
{
	Entity* block = m_blockGrid[_y][_x];
	if (block)
	{
		std::map<GridPosition, bool> blockGroup = std::map<GridPosition, bool>();
		GetBlocksAttachedTo(_x, _y, &blockGroup);

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

void BlockSystem::SpawnBlockAt(Entity* _block, int _x, int _y)
{
	Entity* existingBlock = m_blockGrid[_y][_x];

	if (existingBlock)
		PushDown(_x, _y);

	m_blockGrid[_y][_x] = _block;
	//MoveToWorldPosition(_block, _x, _y);
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
		return _block->GetComponent<CollisionComponent>();
	else
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