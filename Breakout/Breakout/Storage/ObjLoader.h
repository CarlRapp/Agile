#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <string>
#include <vector>
#include <fstream>

#include "../stdafx.h"
#include "../Graphics/Vertex.h"
#include "ModelData.h"

//	Define here what the string values
//	for each token
const	std::string	TOKEN_MTL_GROUP			= "mtllib";
const	std::string	TOKEN_MTL_USE = "usemtl";

const	std::string	TOKEN_VERTEX_GROUP = "g";
const	std::string	TOKEN_VERTEX_POSITION = "v";
const	std::string	TOKEN_VERTEX_NORMAL = "vn";
const	std::string	TOKEN_VERTEX_TEXPOS = "vt";

const	std::string	TOKEN_FACE_DATA = "f";

enum ObjToken
{
	MATERIAL_GROUP, 
	MATERIAL_USE, 

	VERTEX_GROUP,
	VERTEX_POSITION,
	VERTEX_NORMAL,
	VERTEX_TEXPOS,

	FACE_DATA,

	UNDEFINED
};

class ObjLoader
{
	ObjLoader();
	~ObjLoader(void);
	
	static ObjToken GetToken(std::string objLine);
	static Vector3 ConvertToPosition(std::string line, ObjToken token);
	static void GetVertices(std::string line, std::vector<Vector3>& positions, std::vector<Vector2>& texCoords, std::vector<Vector3>& normals, std::vector<Vertex>& vertices);

public:
	static ModelData* LoadObjFile(std::string filePath);
};
#endif