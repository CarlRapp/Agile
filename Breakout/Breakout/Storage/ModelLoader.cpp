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
	CalculateTangents();
	//ParseFace2(file);

	ModelData* model = new ModelData();
	for (auto it = m_groups.begin(); it != m_groups.end(); ++it)
		model->Groups.push_back(it->second);
        
        
	return model;
}

void ModelLoader::CalculateTangents()
{
	for (auto it = m_groups.begin(); it != m_groups.end(); ++it)
	{
		int vertexCount = it->second->triangles.size() * 3;
		VECTOR3 *tan1 = new VECTOR3[vertexCount * 2];
		VECTOR3 *tan2 = tan1 + vertexCount;
		ZeroMemory(tan1, vertexCount * sizeof(VECTOR3) * 2);

		for (int i = 0; i < it->second->triangles.size(); ++i)
		{
			const VECTOR3& v1 = it->second->triangles[i].Vertices[0].Position;
			const VECTOR3& v2 = it->second->triangles[i].Vertices[1].Position;
			const VECTOR3& v3 = it->second->triangles[i].Vertices[2].Position;

			const VECTOR2& w1 = it->second->triangles[i].Vertices[0].Texture;
			const VECTOR2& w2 = it->second->triangles[i].Vertices[1].Texture;
			const VECTOR2& w3 = it->second->triangles[i].Vertices[2].Texture;

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r = 1.0F / (s1 * t2 - s2 * t1);
			VECTOR3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
				(t2 * z1 - t1 * z2) * r);
			VECTOR3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
				(s1 * z2 - s2 * z1) * r);

			tan1[i * 3 + 0].x += sdir.x; tan1[i * 3 + 0].y += sdir.y; tan1[i * 3 + 0].z += sdir.z;
			tan1[i * 3 + 1].x += sdir.x; tan1[i * 3 + 1].y += sdir.y; tan1[i * 3 + 1].z += sdir.z;
			tan1[i * 3 + 2].x += sdir.x; tan1[i * 3 + 2].y += sdir.y; tan1[i * 3 + 2].z += sdir.z;

			tan2[i * 3 + 0].x += tdir.x; tan2[i * 3 + 0].y += tdir.y; tan2[i * 3 + 0].z += tdir.z;
			tan2[i * 3 + 1].x += tdir.x; tan2[i * 3 + 1].y += tdir.y; tan2[i * 3 + 1].z += tdir.z;
			tan2[i * 3 + 2].x += tdir.x; tan2[i * 3 + 2].y += tdir.y; tan2[i * 3 + 2].z += tdir.z;
			
		}
		
		for (long a = 0; a < vertexCount; a++)
		{
			const VECTOR3& n = it->second->triangles[a / 3].Vertices[a % 3].Normal;
			const VECTOR3& t = tan1[a];

			// Gram-Schmidt orthogonalize
			VECTOR3 tan3 = NORMALIZE((t - n * DOT(n, t)));
			VECTOR4 *tangent = &it->second->triangles[a / 3].Vertices[a % 3].Tangent;
			tangent->x = tan3.x;
			tangent->y = tan3.y;
			tangent->z = tan3.z;

			// Calculate handedness

			tangent->w = (DOT(CROSS(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
		}
	}
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


