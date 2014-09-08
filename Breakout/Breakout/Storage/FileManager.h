#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <string>
#include <unordered_map>

#include <typeinfo>

#include "../Graphics/ModelData.h"

class FileManager
{
private:
	static FileManager* instance;
	//FileManager() {};
	//~FileManager() {};

	std::unordered_map<std::string, ModelData*> m_modelMap;

	ModelData* LoadModel(std::string filePath);

public:
	static FileManager* GetInstance()
	{ 
		if (!instance) 
			instance = new FileManager(); 
		return instance;
	}
	FileManager() {};
	~FileManager() {};

	// Use / between dictionaries in the filePath
	template <class T> T* LoadFile(std::string filePath)
	{
		std::string tName = typeid(T).name();
		if (tName == "ModelData")
		{
			return LoadModel(filePath);
		}
		else
			return 0;
	}
};

#endif