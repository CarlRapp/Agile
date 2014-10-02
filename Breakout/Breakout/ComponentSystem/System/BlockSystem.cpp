#include "BlockSystem.h"
#include "../Component/PositionComponent.h"
#include "../World.h"


BlockSystem::BlockSystem(World* _world)
: Base(ComponentFilter().Requires<BlockComponent>(), _world)
{
	m_blockGrid = 0;
	SetSettings(40, 20, 20, 0);
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

void BlockSystem::OnEntityAdded(Entity* _e)
{
	
	int xPos = rand() % m_dimensionX;
	int yPos = m_topCenterY;

	printf("Block %d added at [%d, %d]!\n", _e->GetId(), yPos, xPos);

	if (m_blockGrid[0][xPos] == 0)
	{
		_e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(2 * (int)(xPos - m_dimensionX*0.5f) + m_topCenterX, yPos, 0));
		m_blockGrid[0][xPos] = _e;

		UpdateBlockDependencies(xPos, 0);
	}
	else
	{
		printf("Position [%d, %d] taken, pushing down!\n", yPos, xPos);
		PushDown(_e, xPos);
	}
}


void BlockSystem::OnEntityRemove(Entity* _e)
{
	printf("Block %d Removed!\n", _e->GetId());
}

void BlockSystem::UpdateBlockDependencies(int _x, int _y)
{
	Entity* tAdjacent[4];
	tAdjacent[Up] = 0; // Up
	tAdjacent[Right] = 0; // Right
	tAdjacent[Down] = 0; // Down
	tAdjacent[Left] = 0; // Left

	if (_x > 0)
		tAdjacent[Left] = m_blockGrid[_y][_x - 1];
	if (_x < m_dimensionX - 1)
		tAdjacent[Right] = m_blockGrid[_y][_x + 1];
	if (_y > 0)
		tAdjacent[Up] = m_blockGrid[_y - 1][_x];
	if (_y < m_dimensionY - 1)
		tAdjacent[Down] = m_blockGrid[_y + 1][_x];

	Entity*	baseBlock = m_blockGrid[_y][_x];
	BlockComponent* baseComponent = baseBlock->GetComponent<BlockComponent>();
	if (_y == 0)
	{
		for (int i = 0; i < DIRECTIONS; ++i)
		{
			Entity* e = tAdjacent[i];
			if (e)
			{
				BlockComponent* bComponent = e->GetComponent<BlockComponent>();
				bComponent->AddToAdjacentList(baseBlock->GetId(), true);
				baseComponent->AddToAdjacentList(e->GetId(), true);
			}
		}
	}
	else
	{
		int a = 2;
	}

}

void BlockSystem::PushDown(Entity* _newBlock, int _x)
{
	std::map<GridPosition, bool> visitedPositions = std::map<GridPosition, bool>();

	//	Start by connecting the new block with the one already in its position
	Entity* A = _newBlock;
	Entity* B = m_blockGrid[0][_x];
	A->GetComponent<BlockComponent>()->AddToAdjacentList(B->GetId(), true);
	B->GetComponent<BlockComponent>()->AddToAdjacentList(A->GetId(), true);
	VisitAdjacentRec(&visitedPositions, _x, 0);

	GridQueue sortedPositions = GridQueue();

	std::map<GridPosition, bool>::iterator gIT;
	for (gIT = visitedPositions.begin(); gIT != visitedPositions.end(); ++gIT)
		sortedPositions.push(gIT->first);

	int tSize = sortedPositions.size();
	for (int i = 0; i < tSize; ++i)
	{
		GridPosition GP = sortedPositions.top();
		printf("[X,Y] (%d %d)\n", GP.first, GP.second);
		m_blockGrid[GP.second + 1][GP.first] = m_blockGrid[GP.second][GP.first];
		m_blockGrid[GP.second][GP.first] = 0;
		sortedPositions.pop();
	}

	for (gIT = visitedPositions.begin(); gIT != visitedPositions.end(); ++gIT)
	{
		GridPosition GP = gIT->first;
		Entity* e = m_blockGrid[GP.second+1][GP.first];
		if (e)
			e->GetComponent<PositionComponent>()->SetPosition(VECTOR3(2 * (int)(GP.first - m_dimensionX*0.5f) + m_topCenterX, m_topCenterY - 2 * (GP.second + 1), 0));
	}
		
	_newBlock->GetComponent<PositionComponent>()->SetPosition(VECTOR3(2 * (int)(_x - m_dimensionX*0.5f) + m_topCenterX, m_topCenterY, 0));
	m_blockGrid[0][_x] = _newBlock;
	
}

void BlockSystem::VisitAdjacentRec(std::map<GridPosition, bool>* _closedList, int _x, int _y)
{
	//	Check if this position already is calculated
	if (_closedList->find(GridPosition(_x, _y)) != _closedList->end())
		return;

	//	Set this positions as visited
	(*_closedList)[GridPosition(_x, _y)] = true;


	if (_x > 0)
		if (m_blockGrid[_y][_x-1])
			VisitAdjacentRec(_closedList, _x - 1, _y);
	if (_x < m_dimensionX - 1)
		if (m_blockGrid[_y][_x+1])
			VisitAdjacentRec(_closedList, _x + 1, _y);
	if (_y > 0)
		if (m_blockGrid[_y-1][_x])
			VisitAdjacentRec(_closedList, _x, _y - 1);
	if (_y < m_dimensionY - 1)
		if (m_blockGrid[_y+1][_x])
			VisitAdjacentRec(_closedList, _x, _y + 1);
}
void BlockSystem::PushDownRec(int _x, int _y)
{
	if (_y < m_dimensionY - 1)
		if (m_blockGrid[_y + 1][_x])
			PushDownRec(_x, _y + 1);

	int a = 2;
}