#ifndef COMPONENTFILTER_H
#define COMPONENTFILTER_H

#include <vector>
#include "../ClassTypeID.h"
#include "../Components/IComponent.h"

class ComponentFilter
{
private:
	std::vector<TypeID> m_requiredComponents;
	std::vector<TypeID> m_excludedComponents;

public:

	ComponentFilter() {}
	ComponentFilter(const ComponentFilter& _componentFilter) = default;

	template <typename C1>
	ComponentFilter& Requires()
	{
		if (std::is_base_of<IComponent, C1>())
			m_requiredComponents.push_back(C1::GetTypeId());
		else
			printf("ERROR -> Requires\n");

		return *this;
	}

	template <typename C1, typename C2, typename... Components>
	ComponentFilter& Requires()
	{
		Requires<C1>();
		Requires<C2, Components...>();

		return *this;
	}

	template <typename C1>
	ComponentFilter& Excludes()
	{
		if (std::is_base_of<IComponent, C1>())
			m_excludedComponents.push_back(C1::GetTypeId());
		else
			printf("ERROR -> Excludes\n");

		return *this;
	}
	template <typename C1, typename C2, typename... Components>
	ComponentFilter& Excludes()
	{
		Excludes<C1>();
		Excludes<C2, Components...>();

		return *this;
	}

	bool DoesFilterPass(const std::vector<IComponent*> _components) const;
	void Clear();

};

#endif