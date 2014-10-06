#ifndef ENTITY_H
#define ENTITY_H

#include "../Component/IComponent.h"

#include <iostream>
#include <type_traits>
#include <vector>

class Entity
{

public:

	enum ENTITY_STATE
	{
		ALIVE = 0,
		SOON_DEAD,
		DEAD,
		CHANGED
	};

private:

	void AddComponent(IComponent* component, TypeID _componentTypeId);
	void RemoveComponent(int _componentTypeId);
	IComponent* GetComponent(int _componentTypeId);
	bool HasComponent(int _componentTypeId);

	static int m_counter;

	bool m_initialized;

	int m_id;
	ENTITY_STATE m_state;
	std::vector<IComponent*> m_components;

public:

	Entity();
	Entity(int _id);
	~Entity();

	ENTITY_STATE GetState(void) { return m_state; };
	void SetState(ENTITY_STATE _state) { m_state = _state; };

	template <typename T>
	T& AddComponent(T* component);

	template <typename T, typename... Args>
	T& AddComponent(Args&&... args);

	template <typename T>
	void RemoveComponent();

	template <typename T>
	T* GetComponent();

	template <typename T>
	bool HasComponent();


	std::vector<IComponent*>* GetComponents();
	bool RemoveAllComponents();
	void Reset(void);
	int GetId();

	bool GetInitialized(void);
	void SetInitialized(bool _value);

};

template <typename T>
T& Entity::AddComponent(T* component)
{
	if (std::is_base_of<IComponent, T>())
		AddComponent(component, T::GetTypeId());
	else
		printf("ERROR -> addComponent\n");

	return *component;
}

template <typename T, typename... Args>
T& Entity::AddComponent(Args&&... args)
{
	return AddComponent<T>(new T{ std::forward<Args>(args)... });
}

template <typename T>
void Entity::RemoveComponent()
{
	if (std::is_base_of<IComponent, T>())
		RemoveComponent(T::GetTypeId());
	else
		printf("ERROR -> removeComponent\n");
}

template <typename T>
T* Entity::GetComponent()
{
	if (std::is_base_of<IComponent, T>())
		return static_cast<T*>(GetComponent(T::GetTypeId()));


	printf("ERROR -> getComponent");
	return 0;
}

template <typename T>
bool Entity::HasComponent()
{
	if (std::is_base_of<IComponent, T >())
		return HasComponent(T::GetTypeId());

	printf("ERROR -> hasComponent\n");
	return false;
}


#endif