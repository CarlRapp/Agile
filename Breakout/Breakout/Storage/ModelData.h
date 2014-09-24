#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include <vector>

#include "../stdafx.h"
#include "../Graphics/ModelHelper.h"

class ModelData
{
public:
	std::vector<Group*> Groups;

	ModelData();
	~ModelData();
};


#endif