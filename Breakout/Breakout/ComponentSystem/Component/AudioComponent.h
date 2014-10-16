#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include "IComponent.h"
#include "../../Storage/FileManager.h"

struct AudioComponent : Component<AudioComponent>
{
	std::string m_audioPath;

	AudioComponent()
	{
		m_audioPath = "";
	}

};

#endif