#include "ScoreSystem.h"
#include "../Component/CollisionComponent.h"
#include "../Component/ScoreComponent.h"
#include "../World.h"
#include "../EntityFactory.h"
#include "../../Scenes/GameScene.h"
#include "../../Scene/SceneManager.h"
using namespace SceneSystem;

ScoreSystem::ScoreSystem(World* _world)
: Base(ComponentFilter().Requires<ScoreComponent>(), _world)
{
    m_frameScore = 0;
}

ScoreSystem::~ScoreSystem()
{

}

void ScoreSystem::OnEntityAdded(Entity* _e)
{

}

void ScoreSystem::OnEntityRemoved(Entity* _e)
{
	ScoreComponent* SC = _e->GetComponent<ScoreComponent>();
	int a = 2;

	m_frameScore += SC->m_score;
}
void ScoreSystem::Update(float _dt)
{
	std::vector<Entity*>* player = m_world->GetEntities<PlayerComponent>();
	if (m_frameScore > 0)
	{
		auto SC = player->at(0)->GetComponent<ScoreComponent>();
		SC->AddScore(m_frameScore);
                
		SC->SetString();
		m_frameScore = 0;

                m_levelUp = SC->CheckLevelUp();
                
                if(m_levelUp)
                {
                    GameScene* gScene = SceneManager::GetInstance()->GetScene<GameScene>();
                    gScene->LevelUp(m_levelUp,true);
                    player->at(0)->GetComponent<PlayerComponent>()->AddLevel(m_levelUp/STATPOINTS_PER_LEVEL);
                    m_levelUp = 0;
                    
                }
                
                player->at(0)->GetComponent<PlayerComponent>()->m_expString = ExpToString(
                        SC->m_nextLevel,
                        SC->m_score,player->at(0)->GetComponent<PlayerComponent>()->m_level,
                        SC->m_increase);
	}
}

std::string ScoreSystem::ExpToString(int _nextLevel, int _score,int _level, float _increase)
{
    std::string str = "EXP <";
    int blocks = 0;
    int div = (_increase)/10;
    blocks =  (_nextLevel - _score)/(div);

    for(int i =0; i < (10-blocks);i++)
    {
        str += "[";
    }
    for(int i = 0; i < blocks;i++)
    {
        str += " ";
    }
    str += ">";
    
    str += " LEVEL: ";
    str += std::to_string(_level);

    return str;
}