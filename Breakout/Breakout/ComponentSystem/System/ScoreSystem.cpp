#include "ScoreSystem.h"
#include "../Component/LifeComponent.h"
#include "../Component/ScoreComponent.h"
#include "../../Input/InputManager.h"

ScoreSystem::ScoreSystem()
: Base(ComponentFilter().Requires<ScoreComponent>().Excludes<LifeComponent>())
{
}

ScoreSystem::~ScoreSystem()
{

}

void ScoreSystem::Update(float _dt)
{
}