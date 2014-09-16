#ifndef SCENE_H
#define SCENE_H
#include "../stdafx.h"
#include "../ComponentsSystem/ClassTypeID.h"
#include <map>


namespace SceneSystem
{
	class BaseScene
	{
	public:
		virtual	~BaseScene(){};
		virtual void Initialize(){};
		virtual void LoadContent(){};

		virtual void Update(float _dt){};
		virtual void Render(){};

		virtual void Reset(){};
	};

	template <typename C>
	class Scene : public BaseScene
	{
	public:
		static TypeID GetSceneID()
		{
			return ClassTypeID<BaseScene>::GetTypeId<C>();
		}
	};

	typedef std::map<TypeID, BaseScene*> SceneMap;
}

#endif