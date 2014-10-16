#ifndef CLASSTYPEID_H
#define CLASSTYPEID_H

#include <stdio.h>
#include <iostream>

typedef std::size_t TypeID;

template<typename TBase>
class ClassTypeID
{
private:
	static TypeID m_nextTypeID;

public:
	template<typename T>
	static TypeID GetTypeId()
	{
		static const TypeID ID = m_nextTypeID++;
		return ID;
	}
};
template<typename TBase>
TypeID ClassTypeID<TBase>::m_nextTypeID = 0;

#endif