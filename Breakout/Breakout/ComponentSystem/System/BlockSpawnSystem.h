#ifndef BLOCKSPAWNSYSTEM_H
#define BLOCKSPAWNSYSTEM_H

#include "ISystem.h"
#include "PhysicsSystem.h"
#include <queue>

enum BlockType
{
	RED_SMALL,
	BLUE_SMALL,
	BLACK_SMALL,
	GREEN_SMALL,
	TNT_SMALL,
	BLOCKS_1x1x1_END,
	RED_MEDIUM,
	BLUE_MEDIUM,
	GREEN_MEDIUM,
	BLOCKS_2x1x1_END,
	BLOCK_NULL
};
struct BlockPool
{
private:
	std::vector<BlockType> m_blocks;
	int m_poolSpawnChance;
public:
	BlockPool(){ m_blocks = std::vector<BlockType>(); m_poolSpawnChance = 0; }
	~BlockPool(){ m_blocks.clear(); }

	void SetPoolChance(int _chance){ m_poolSpawnChance = _chance; }
	int GetPoolChance(){ return m_poolSpawnChance; }
	void ClearPool(){ m_blocks.clear(); }
	void AddBlockToPool(BlockType _block)
	{
		m_blocks.push_back(_block);
	}
	BlockType GetRandomBlock()
	{
		if (m_blocks.size() == 0)
			return BLOCK_NULL;

		int randIndex = rand() % m_blocks.size();
		return m_blocks[randIndex];
	}
};

class BlockSpawnSystem : public System<BlockSpawnSystem>
{
private:
	float m_spawnTimer;
	float m_elapsedTime;
	std::vector<BlockPool*> m_blockPools;

	void SpawnBlock();
	Entity* GetBlock(BlockType _block);

	void CreateBody(BlockType _block, b2BodyDef* _b2BodyDef, vector<b2FixtureDef*>& _b2FixtureDefs);

public:
	BlockSpawnSystem(World* _world);
	~BlockSpawnSystem();

	void Update(float _dt);
	void AddBlockPool(BlockPool* _pool);

	void ForceSpawn(int _numberOfBlocks);
};

#endif