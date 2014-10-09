#include "TextSystem.h"
#include "../Component/TextComponent.h"

TextSystem::TextSystem(World* _world)
: Base(ComponentFilter().Requires<TextComponent>(), _world)
{
    
}

TextSystem::~TextSystem()
{

}

void TextSystem::OnEntityAdded(Entity* _entity)
{
    
}

void TextSystem::OnEntityRemove(Entity* _entity)
{
    
}


void TextSystem::Update(float _dt)
{
	for (auto entityPair : m_entityMap)
	{
            Entity* e = entityPair.second;
            auto TC = e->GetComponent<TextComponent>();
            
            //fulhax 9999 = ingen effekt
            if(TC->m_effect != 9999.0f)
            {
                if(TC->m_effect > TC->m_effectStop)
                {
                    TC->m_effect -= _dt*TC->m_effectSpeed;
                }
                else
                    TC->m_effect = 1.0f;
            }
	}

}