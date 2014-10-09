#ifndef BLOCKSYSTEM_H
#define BLOCKSYSTEM_H

#include <stdlib.h>
#include "ISystem.h"
#include "../Component/BlockComponent.h"
#include <queue>

class BlockSystem : public System<BlockSystem>
{
private:
	typedef std::pair<int, int> GridPosition;
	typedef Entity*** BlockGrid;

	enum Direction
	{
		Up = 0,
		Down,
		Right,
		Left,
		DIRECTIONS
	};
	struct CompareY
	{
		bool operator()(GridPosition const & A, GridPosition const & B) {
			// return "true" if "p1" is ordered before "p2", for example:
			return A.second < B.second;
		}
	};
	typedef std::priority_queue<GridPosition, std::vector<GridPosition>, CompareY> GridQueue;

	int m_dimensionX, m_dimensionY;
	int m_topCenterY, m_topCenterX;
	BlockGrid m_blockGrid;

	void OnEntityAdded(Entity* _e);
	void OnEntityRemoved(Entity* _e);
	void FindBlock(Entity* _e, int& _x, int& _y);

	void SpawnBlockAt(Entity* _block, int _x, int _y);

	void PushDown(int _x, int _y);
	void GetBlocksAttachedTo(int _x, int _y, std::map<GridPosition, bool>* _closedList);
	bool GroupCanReachRoot(std::map<GridPosition, bool>* _blockGroup);
	void CheckIndividualBlock(int _x, int _y);
	void MoveToWorldPosition(Entity* _block, int _x, int _y);
	bool HasCollisionComponent(Entity* _block);
public:
	BlockSystem(World* _world);
	~BlockSystem();

	void Update(float _dt);
	void SetSettings(int _sizeX, int _sizeY, int _topCenterY = 20, int _topCenterX = 0);
};

#endif