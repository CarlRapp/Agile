#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include "../ClassTypeID.h"

class IComponent
{
private:

public:
	virtual ~IComponent() {}

	TypeID m_ID;
};

template<typename T>
class Component
	: public IComponent
{
private:

public:

	static TypeID GetTypeId()
	{
		return ClassTypeID<IComponent>::GetTypeId<T>();
	}

};


#endif