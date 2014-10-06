#ifndef EFFECTCOMPONENT_H
#define EFFECTCOMPONENT_H

#include "IComponent.h"

enum EffectFlags
{
	SHATTER = 1 << 0,

	EXPLORE = 1 << 1,
	INVISIBLE = 1 << 2,
	NO_EFFECT = 1 << 10,
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
	EffectFlags m_effects;

	EffectComponent() : m_effects(NO_EFFECT) { }

};


#endif