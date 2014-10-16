#ifndef BLOCKCOMPONENT_H
#define BLOCKCOMPONENT_H

#include "IComponent.h"
#include "../Entity/Entity.h"
struct BlockComponent : Component<BlockComponent>
{
private:
	VECTOR2 m_blockSize;
	VECTOR2 m_blockDimension;
public:
	BlockComponent() : m_blockSize(VECTOR2(1, 1)), m_blockDimension(VECTOR2(1, 1)){}

	void SetSize(VECTOR2 _size){ m_blockSize = _size; }
	void SetDimension(VECTOR2 _dimension){ m_blockDimension = _dimension; }

	VECTOR2 GetSize(){ return m_blockSize; }
	VECTOR2 GetDimension(){ return m_blockDimension; }
};

#endif