#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include "IComponent.h"

struct TextComponent : Component<TextComponent>
{
private:


public:
        std::string m_text;
        float m_scale;
        unsigned int m_color;
        int m_x;
        int m_y;
    
	TextComponent(){ }

        void Initialize(std::string _text,float _scale, unsigned int _color,int _x,int _y)
        {
            m_text  =_text,
            m_scale =_scale;
            m_color =_color;
            m_x     =_x;
            m_y     =_y;
        }
        
        void SetText(std::string _text)
        {
            m_text = _text;
        }
        
	void Reset()
	{

	}
};

#endif