#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include <vector>

#include "../stdafx.h"
#include "../Graphics/ModelHelper.h"

class ModelData
{
private:

public:
	ModelData();
	~ModelData();

	std::vector<Group*> Groups;
};


#endif