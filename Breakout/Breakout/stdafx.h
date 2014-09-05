#ifndef _STDAFX_H_
#define _STDAFX_H_

struct VECTOR2
{
private:
	float m_x, m_y;
public:
	VECTOR2(float x, float y) : m_x(x), m_y(y) {}
};

struct VECTOR3
{
private:
	float m_x, m_y, m_z;
public:
	VECTOR3(float x, float y) : m_x(x), m_y(y) {}
};

#endif