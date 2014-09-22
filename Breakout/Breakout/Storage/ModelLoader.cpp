#include "ModelLoader.h"

ModelLoader::ModelLoader()
{
	m_positions = vector<VECTOR3>();
	m_normals = vector<VECTOR3>();
	m_texCoords = vector<VECTOR2>();
	m_vertices = vector<Vertex>();

	m_newGroupNameCounter = 0;
	m_materialAfterGroup = true;
}

ModelLoader::~ModelLoader()
{
	
}


ModelData* ModelLoader::LoadModelFile(std::string filePath)
{
	ifstream file;
	file.open(filePath + ".obj");

	if (!file)
		return 0;
	string str;

	while (!file.eof())
	{
		file >> str;
                
		if (str == "#" || str == "s")           ParseComment(file);
		else if (str == "v")			ParsePosition(file);	//position
		else if (str == "vn")			ParseNormal(file);		//normal
		else if (str == "vt")			ParseTexCoord(file);	//texturkoordinat
		else if (str == "f")			ParseFace(file);		//face
		else if (str == "usemtl")		ParseMaterial(file);	//material
		else if (str == "g")			ParseGroup(file);		//group

		else if (str == "mtllib")								//materialfile
		{
			ParseMaterialFile(file, MODEL_ROOT);
		}
                str = "";
	}
	//ParseFace2(file);

	ModelData* model = new ModelData();
	for (auto it = m_groups.begin(); it != m_groups.end(); ++it)
		model->Groups.push_back(it->second);
        
        
	return model;
}



void ModelLoader::ParseGroup(std::ifstream& file)
{
	string str;

	std::getline(file, str);
	Btrim(str);

	auto mIter = m_groups.find(str);

	//check if group is already there
	if (mIter == m_groups.end())
	{
		m_currentGroup = new Group;
		m_currentGroup->name = str;
		m_currentGroup->material = NULL;
		m_groups[str] = m_currentGroup;
	}
	else
	{
		m_currentGroup = mIter->second;
	}

	m_materialAfterGroup = false;
}

void ModelLoader::ParseMaterial(std::ifstream& file)
{
	string str;
	if (m_materialAfterGroup == true)
	{
		map<string, Group*>::iterator mIter;
		do
		{
			char tmpGName[20];
#ifdef WINDOWS
			sprintf_s(tmpGName, sizeof(tmpGName), "%d", m_newGroupNameCounter++);
#else
                        snprintf(tmpGName, sizeof(tmpGName), "%d", m_newGroupNameCounter++);
#endif
			std::string groupName = tmpGName;
			mIter = m_groups.find(groupName);

			//check if group is already there
			if (mIter == m_groups.end())
			{
				m_currentGroup = new Group;
				m_currentGroup->name = groupName;
				m_groups[groupName] = m_currentGroup;
			}
		} while (mIter != m_groups.end());
	}
	else
	{
		m_materialAfterGroup = true;
	}

	std::getline(file, str);
	Btrim(str);

	m_currentGroup->material = m_materials[str];
}

void ModelLoader::ParseMaterialFile(std::ifstream& file, string dir)
{
	Material* material = NULL;
	string str;

	//get material filename
	std::getline(file, str);
	Btrim(str);

	str = dir + str;

	//append directory in front of filename
	//str = dir + str;
	ifstream mfile;
	mfile.open(str.c_str());

	if (!mfile)
	{
		return;
	}

	while (!mfile.eof())
	{
		mfile >> str;

		if (str == "newmtl")
		{
			//mf >> str;
			std::getline(mfile, str);
			Btrim(str);


			map<string, Material*>::const_iterator mIter = m_materials.find(str);

			//check if material is already there
			if (mIter == m_materials.end())
			{
				material = new Material;
				material->Name = str;
				m_materials[str] = material;
			}
			else
			{
				material = mIter->second;
			}
		}
		else if (str == "illum")
		{
			mfile >> material->Illum;
		}
		else if (str == "Ka")
		{
			for (unsigned int i = 0; i < 3; ++i)
				mfile >> material->Ka[i];
		}
		else if (str == "Kd")
		{
			for (unsigned int i = 0; i < 3; ++i)
				mfile >> material->Kd[i];
		}
		else if (str == "Ks")
		{
			for (unsigned int i = 0; i < 3; ++i)
				mfile >> material->Ks[i];
		}
		else if (str == "Ns")
		{
			mfile >> material->Ns;
		}
		else if (str == "Tf")
		{
			for (unsigned int i = 0; i < 3; ++i)
				mfile >> material->Tf[i];
		}
		else if (str == "Ni")
		{
			mfile >> material->Ni;
		}
		else if (str == "map_Ka")
		{
			getline(mfile, str);
			Btrim(str);
			material->Map_Ka = dir + str;
		}
		else if (str == "map_Kd")
		{
			getline(mfile, str);
			Btrim(str);
			material->Map_Kd = dir + str;
		}
		else if (str == "map_Ks")
		{
			getline(mfile, str);
			Btrim(str);
			material->Map_Ks = dir + str;
		}
		else if (str == "map_Ns")
		{
			getline(mfile, str);
			Btrim(str);
			material->Map_Ns = dir + str;
		}
		else if (str == "map_Tf")
		{
			getline(mfile, str);
			Btrim(str);
			material->Map_Tf = dir + str;
		}
		else if (str == "map_bump")
		{
			getline(mfile, str);
			Btrim(str);
			material->Map_bump = dir + str;
		}
	}
}

void ModelLoader::ParseComment(std::ifstream& file)
{
	string str;
	getline(file, str);
}

void ModelLoader::ParsePosition(std::ifstream& file)
{
	float x, y, z;
	file >> x;
	file >> y;
	file >> z;

	VECTOR3 pos(x, y, -z);

	m_positions.push_back(pos);
}
void ModelLoader::ParseNormal(std::ifstream& file)
{
	float x, y, z;
	file >> x;
	file >> y;
	file >> z;

	VECTOR3 normal(x, y, -z);

	m_normals.push_back(normal);
}
void ModelLoader::ParseTexCoord(std::ifstream& file)
{
	float x, y, z;
	file >> x;
	file >> y;

	VECTOR2 texCoord(x, 1 - y);

	m_texCoords.push_back(texCoord);
}

void ModelLoader::ParseFace(std::ifstream& file)
{
	Triangle triangle;
	string triPart;
	string sub;

	string str;
	getline(file, str);
	Btrim(str);

	for (unsigned int t = 0; t < 3; ++t)
	{
		Vertex vertex;
		triPart = str.substr(0, str.find(" "));

		unsigned int off = (unsigned int)str.find(" ");
		if (off > 0)
			str = str.substr(off + 1);

		for (unsigned int i = 0; i < 3; ++i)
		{
			sub = triPart.substr(0, triPart.find("/"));

			unsigned int off = (unsigned int)triPart.find("/");
			if (off > 0)
				triPart = triPart.substr(off + 1);

			if (sub.length() != 0)
			{
				unsigned int index = atoi(sub.c_str()) - 1;
				switch (i)
				{
				case 0:
					vertex.Position = m_positions[index];
					break;
				case 1:
					vertex.Texture = m_texCoords[index];
					break;
				case 2:
					vertex.Normal = m_normals[index];
					break;
				}
			}
		}

		triangle.Vertices[t] = vertex;
	}
	m_currentGroup->triangles.push_back(triangle);
}

void ModelLoader::ParseFace2(std::ifstream& file)
{
	string triPart;
	string sub;

	for (unsigned int t = 0; t < 3; ++t)
	{
		Vertex vertex;
		file >> triPart;
		for (unsigned int i = 0; i < 3; ++i)
		{

			sub = triPart.substr(0, triPart.find("/"));

			unsigned int off = (unsigned int)triPart.find("/");
			if (off > 0)
				triPart = triPart.substr(off + 1);

			unsigned int index = atoi(sub.c_str()) - 1;

			switch (i)
			{
			case 0:
				vertex.Position = m_positions[index];
				break;
			case 1:
				vertex.Texture = m_texCoords[index];
				break;
			case 2:
				vertex.Normal = m_normals[index];
				break;
			}

		}
		m_vertices.push_back(vertex);
	}

}




void ModelLoader::Ltrim(std::string& str)
{
	string::size_type pos = 0;

	while (pos < str.size() && isspace(str[pos]))
		pos++;

	str.erase(0, pos);
}

void ModelLoader::Rtrim(std::string& str)
{
	string::size_type pos = str.size();

	while (pos > 0 && isspace(str[pos - 1]))
		pos--;

	str.erase(pos);
}

void ModelLoader::Btrim(std::string& str)
{
	Ltrim(str);
	Rtrim(str);
}


