#ifndef MODELLOADER_H
#define MODELLOADER_H
#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "../stdafx.h"
#include "ModelData.h"

class ModelLoader
{
	
	map<string, Material*>	m_materials;
	map<string, Group*>		m_groups;

	vector<VECTOR3>			m_positions;
	vector<VECTOR3>			m_normals;
	vector<VECTOR2>			m_texCoords;
	vector<Vertex>			m_vertices;

	Group*					m_currentGroup;
	int						m_newGroupNameCounter;
	bool					m_materialAfterGroup;

	void ParseComment(std::ifstream& file);
	void ParseFace(std::ifstream& file);
	void ParseFace2(std::ifstream& file);
	void ParseMaterial(std::ifstream& file);
	void ParsePosition(std::ifstream& file);
	void ParseNormal(std::ifstream& file);
	void ParseTexCoord(std::ifstream& file);
	void ParseGroup(std::ifstream& file);
	void ParseMaterialFile(std::ifstream& file, string dir);

	void Ltrim(std::string& str);
	void Rtrim(std::string& str);
	void Btrim(std::string& str);

public:
	ModelLoader();
	~ModelLoader();

	ModelData* LoadModelFile(std::string filePath);
};
#endif