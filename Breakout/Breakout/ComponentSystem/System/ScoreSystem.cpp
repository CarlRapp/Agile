#include "ScoreSystem.h"
#include "../Component/LifeComponent.h"
#include "../Component/ScoreComponent.h"
#include "../../Input/InputManager.h"
#include "../World.h"

ScoreSystem::ScoreSystem(World* _world)
: Base(ComponentFilter().Requires<ScoreComponent>().Excludes<LifeComponent>(), _world)
{
}

ScoreSystem::~ScoreSystem()
{

}

void ScoreSystem::Update(float _dt)
{
}