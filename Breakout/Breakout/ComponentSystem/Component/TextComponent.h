#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include "IComponent.h"

struct TextComponent : Component<TextComponent>
{
private:


public:
        std::string* m_text;
        float m_scale;
        VECTOR3 m_color;
        float m_x;
		float m_y;
		float m_effect;
    
	TextComponent(){ }

        void Initialize(std::string* _text,float _x, float _y, float _scale, VECTOR3 _color, float _effect)
        {
            m_text  =_text,
            m_scale =_scale;
            m_color =_color;
            m_x     =_x;
            m_y     =_y;
			m_effect = _effect;
        }
        
        void SetText(std::string* _text)
        {
            m_text = _text;
        }
        
	void Reset()
	{

	}
};

#endif