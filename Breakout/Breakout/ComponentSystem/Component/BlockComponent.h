#ifndef BLOCKCOMPONENT_H
#define BLOCKCOMPONENT_H

#include "IComponent.h"
#include "../Entity/Entity.h"
struct BlockComponent : Component<BlockComponent>
{
private:
	//	All IDs in this map is adjacent blocks
	//	to this block, the bool indicates if
	//	the block is attached to the adjacent
	//	block or not.
	//	True = It depends on the block
	//	False = It does not depend on it, just a neighbour
	std::map<TypeID, bool> m_adjacentBlocks;

public:
	BlockComponent()
	{
		m_adjacentBlocks = std::map<TypeID, bool>();
	}

	void AddToAdjacentList(TypeID _i, bool _dependency = false)
	{
		if (m_adjacentBlocks.find(_i) != m_adjacentBlocks.end())
			return;

		m_adjacentBlocks[_i] = _dependency;
	}
	void ChangeDependency(TypeID _i, bool _dependency)
	{
		if (m_adjacentBlocks.find(_i) != m_adjacentBlocks.end())
			return;

		m_adjacentBlocks[_i] = _dependency;
	}
	void RemoveDependency(TypeID _i)
	{
		if (m_adjacentBlocks.find(_i) != m_adjacentBlocks.end())
			return;

		m_adjacentBlocks.erase(_i);
	}

	int GetSize()
	{
		return m_adjacentBlocks.size();
	}

};

#endif