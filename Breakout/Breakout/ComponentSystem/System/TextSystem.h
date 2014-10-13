#ifndef TEXTSYSTEM_H
#define TEXTSYSTEM_H

#include "ISystem.h"

class TextSystem : public System<TextSystem>
{
private:
	void OnEntityAdded(Entity* _entity);
	void OnEntityRemove(Entity* _entity);

public:

	TextSystem(World* _world);
	~TextSystem();
	void Update(float _dt);
};
#endif