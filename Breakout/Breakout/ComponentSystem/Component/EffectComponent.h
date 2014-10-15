#ifndef EFFECTCOMPONENT_H
#define EFFECTCOMPONENT_H

#include "IComponent.h"


enum EffectFlags
{
	SHATTER				= 1 << 0,
	EXPLODE				= 1 << 1,
	INVISIBLE			= 1 << 2,
	TRAIL				= 1 << 3,
	SCALE_MIN_TO_MAX	= 1 << 4,
	CHANGE_MODEL    = 1 << 5,

	NO_EFFECT = 1 << 10,
};

struct EffectEvents
{
	EffectFlags OnEveryFrame;
	EffectFlags OnCollide;
	EffectFlags OnRemoved;
	EffectFlags OnAdded;
};


// OR operator
inline EffectFlags operator|(EffectFlags a, EffectFlags b)
{
	return EffectFlags(((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type)a) | ((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type)b));
}

inline EffectFlags &operator |= (EffectFlags &a, EffectFlags b)
{ 
	return (EffectFlags &)(((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type &)a) |= ((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type)b));
}

// AND operator
inline EffectFlags operator & (EffectFlags a, EffectFlags b)
{ 
	return EffectFlags(((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type)a) & ((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type)b));
}
inline EffectFlags &operator &= (EffectFlags &a, EffectFlags b)
{ 
	return (EffectFlags &)(((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type &)a) &= ((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type)b));
}

// NOT operator (invert)
inline EffectFlags operator ~ (EffectFlags a) 
{
	return EffectFlags(~((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type)a)); 
}

// XOR operator
inline EffectFlags operator ^ (EffectFlags a, EffectFlags b) 
{ 
	return EffectFlags(((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type)a) ^ ((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type)b)); 
}
inline EffectFlags &operator ^= (EffectFlags &a, EffectFlags b) 
{ 
	return (EffectFlags &)(((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type &)a) ^= ((_ENUM_FLAG_SIZED_INTEGER<EffectFlags>::type)b)); 
}

struct EffectComponent : Component<EffectComponent>
{
	EffectEvents m_effects;

	EffectComponent()
	{
		m_effects.OnEveryFrame	= NO_EFFECT;
		m_effects.OnCollide		= NO_EFFECT;
		m_effects.OnRemoved		= NO_EFFECT;
		m_effects.OnAdded		= NO_EFFECT;
	}

};


#endif