#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include "IComponent.h"
#include "../../BoundingBox2D.h"

struct TextComponent : Component<TextComponent>
{
private:


public:
        std::string* m_text=NULL;
        float m_scale;
        VECTOR3 m_color;
        float m_x;
        float m_y;
        float m_effect;
        float m_effectStop;
        float m_effectSpeed;
        bool m_useEffect;
        
	TextComponent(){ }

        void Initialize(std::string* _text,float _x, float _y, float _scale, VECTOR3 _color, float _effect)
        {
            m_text  =_text,
            m_scale =_scale;
            m_color =_color;
            m_x     =_x;
            m_y     =_y;
            m_effect = _effect;
            m_effectStop = 1.0f;
            m_effectSpeed = 20.0f;
        }
        
        void SetText(std::string* _text)
        {
            m_text = _text;
        }


		BoundingBox2D GetBoundingBox2D()
		{
			VECTOR2 Min(m_x, m_y);
			float maxX = (m_x + 0.00625 * m_text->size()) * m_effect + 0.00625;
			VECTOR2 Max(maxX, m_y + 0.01111f);
			return BoundingBox2D(Min, Max);
		}

        
	void Reset()
	{

	}
};

#endif