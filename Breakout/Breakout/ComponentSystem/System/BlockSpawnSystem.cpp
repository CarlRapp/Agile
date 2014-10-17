#include "BlockSpawnSystem.h"
#include "../Component/BallComponent.h"
#include "PhysicsSystem.h"
#include "../World.h"

BlockSpawnSystem::BlockSpawnSystem(World* _world)
: Base(ComponentFilter(), _world), m_elapsedTime(0), m_spawnTimer(0)
{
}

BlockSpawnSystem::~BlockSpawnSystem()
{

}

void BlockSpawnSystem::Update(float _dt)
{
	std::vector<Entity*>* activeBalls = m_world->GetEntities<BallComponent>();
	if (!activeBalls || (activeBalls->size() == 0))
		return;

	m_elapsedTime += _dt;

	m_spawnTimer -= _dt;
	if (m_spawnTimer <= 0)
	{
		SpawnBlock();
		m_spawnTimer = 0.3f * exp(-m_elapsedTime / 120);
		printf("Time: %f\n", m_spawnTimer);
	}
}

void BlockSpawnSystem::AddBlockPool(BlockPool* _pool)
{
	for (int i = 0; i < m_blockPools.size(); ++i)
		if (m_blockPools[i] == _pool)
			return;

	m_blockPools.push_back(_pool);
}


Entity* BlockSpawnSystem::GetBlock(BlockType _block)
{
	if (_block == BlockType::BLOCK_NULL || _block == BlockType::BLOCKS_1x1x1_END || _block == BlockType::BLOCKS_2x1x1_END)
		return 0;

	Entity* block = m_world->CreateEntity();

	b2BodyDef* bodyDef;
	std::vector<b2FixtureDef*> fixDefs = std::vector<b2FixtureDef*>();
	b2PolygonShape* polyShapes;

	bodyDef = new b2BodyDef();

	block->AddComponent<PositionComponent>();
	block->AddComponent<RotationComponent>();
	block->AddComponent<ScaleComponent>();
	block->GetComponent<ScaleComponent>()->SetScale(VECTOR3(2, 2, 2));
	CreateBody(_block, bodyDef, fixDefs);
	block->AddComponent<CollisionComponent>(bodyDef, fixDefs);
	block->AddComponent<DeflectionComponent>(30.0f);
	block->AddComponent<HealthComponent>(1);
	block->AddComponent<ScoreComponent>().m_score = 1;
	block->AddComponent<BlockComponent>();
	block->AddComponent<EffectComponent>();
	block->GetComponent<EffectComponent>()->m_effects.OnAdded = EffectFlags::SCALE_MIN_TO_MAX;
	block->GetComponent<EffectComponent>()->m_effects.OnRemoved = EffectFlags::SCALE_MAX_TO_MIN;

	switch (_block)
	{
	case BlockType::BLACK_SMALL:
		block->AddComponent<ModelComponent>().m_modelPath = "Block_Black_Small";
		block->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		block->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		break;

	case BlockType::BLUE_SMALL:
		block->AddComponent<ModelComponent>().m_modelPath = "Block_Blue_Small";
		block->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		block->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		break;

	case BlockType::GREEN_SMALL:
		block->AddComponent<ModelComponent>().m_modelPath = "Block_Green_Small";
		block->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		block->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		break;

	case BlockType::RED_SMALL:
		block->AddComponent<ModelComponent>().m_modelPath = "Block_Red_Small";
		block->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		block->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		break;

	case BlockType::TNT_SMALL:
		block->AddComponent<ModelComponent>().m_modelPath = "Block_TNT_Small";
		block->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		block->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		block->AddComponent<TNTComponent>();
		block->AddComponent<AudioComponent>().m_audioPath = "Explosion.wav";
		block->GetComponent<EffectComponent>()->m_effects.OnRemoved = EffectFlags::EXPLODE;
		break;

	case BlockType::RED_MEDIUM:
		block->AddComponent<ModelComponent>().m_modelPath = "Block_Red_Medium";
		block->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";
		
		block->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		block->GetComponent<BlockComponent>()->SetDimension(VECTOR2(2, 1));
		block->GetComponent<HealthComponent>()->SetHealth(20);
		block->GetComponent<ScoreComponent>()->m_score = 2;
		block->GetComponent<EffectComponent>()->m_effects.OnCollide = EffectFlags::CHANGE_MODEL;
		break;

	case BlockType::BLUE_MEDIUM:
		block->AddComponent<ModelComponent>().m_modelPath = "Block_Blue_Medium";
		block->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";

		block->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		block->GetComponent<BlockComponent>()->SetDimension(VECTOR2(2, 1));
		block->GetComponent<HealthComponent>()->SetHealth(20);
		block->GetComponent<ScoreComponent>()->m_score = 2;
		block->GetComponent<EffectComponent>()->m_effects.OnCollide = EffectFlags::CHANGE_MODEL;
		break;

	case BlockType::GREEN_MEDIUM:
		block->AddComponent<ModelComponent>().m_modelPath = "Block_Green_Medium";
		block->AddComponent<AudioComponent>().m_audioPath = "Wowpulse.wav";

		block->GetComponent<BlockComponent>()->SetSize(VECTOR2(2, 2));
		block->GetComponent<BlockComponent>()->SetDimension(VECTOR2(2, 1));
		block->GetComponent<HealthComponent>()->SetHealth(20);
		block->GetComponent<ScoreComponent>()->m_score = 2;
		block->GetComponent<EffectComponent>()->m_effects.OnCollide = EffectFlags::CHANGE_MODEL;
		break;
	}

	return block;
}
void BlockSpawnSystem::SpawnBlock()
{
	int poolRand = rand() % 100;

	for (int i = m_blockPools.size() - 1; i >= 0; --i)
		if (m_blockPools[i]->GetPoolChance() > poolRand)
		{
			Entity* newBlock = GetBlock(m_blockPools[i]->GetRandomBlock());
			if (newBlock)
				m_world->AddEntity(newBlock);

			break;
		}
}

void BlockSpawnSystem::CreateBody(BlockType _block, b2BodyDef* _b2BodyDef, vector<b2FixtureDef*>& _b2FixtureDefs)
{
	b2PolygonShape* polygonShape = 0;
	b2CircleShape* circleShape = 0;
	b2FixtureDef* fixDef = new b2FixtureDef();

	polygonShape = new b2PolygonShape();
	
	if (_block <= BLOCKS_1x1x1_END)
		polygonShape->SetAsBox(0.5f, 0.5f);
	else if (_block <= BLOCKS_2x1x1_END)
		polygonShape->SetAsBox(1.0f, 0.5f);

	fixDef->shape = polygonShape;
	fixDef->density = 1.0f;
	fixDef->friction = 0.0f;
	fixDef->filter.categoryBits = CollisionCategory::BLOCK;
	_b2FixtureDefs.push_back(fixDef);
	_b2BodyDef->type = b2_staticBody;
}

void BlockSpawnSystem::ForceSpawn(int _numberOfBlocks)
{
	for (int n = 0; n < _numberOfBlocks; ++n)
		SpawnBlock();
}