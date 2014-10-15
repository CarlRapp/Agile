#ifndef SCALECOMPONENT_H
#define SCALECOMPONENT_H

#include "IComponent.h"

struct ScaleComponent : Component<ScaleComponent>
{
private:
	VECTOR3 m_scale;
	VECTOR3 m_deltaScale;
	bool m_bool;

public:
	ScaleComponent() : m_scale(VECTOR3(1, 1, 1)), m_deltaScale(VECTOR3(1, 1, 1)), m_bool(true) { }
	ScaleComponent(VECTOR3 _scale) : m_scale(_scale), m_deltaScale(VECTOR3(1, 1, 1)) { }

	void Reset()
	{
		m_deltaScale = VECTOR3(0, 0, 0);
	}

	VECTOR3& GetScale() { return m_scale; }
	const VECTOR3& GetDeltaScale() const { return m_scale; }
	void SetScale(const VECTOR3& _scale) { m_deltaScale = VECTOR3(_scale.x - m_scale.x, _scale.y - m_scale.y, _scale.z - m_scale.z); m_scale = _scale; }
	const bool& GetBool() const { return m_bool; }
	void SetBool(const bool& _bool) { m_bool = _bool; }
};

#endif