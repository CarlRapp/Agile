#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <string>
#include <vector>
#include <fstream>

#include "../Graphics/Vertex.h"
#include "../Graphics/ModelData.h"

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

	/*std::vector<Position>	vertexPositions;
	std::vector<Position>	vertexTexturePos;
	std::vector<Position>	vertexNormals;
	std::vector<VERTEX>		objVertices;
	*/
	
	
	
	ObjToken GetToken(std::string objLine);
	Vector3 ConvertToPosition(std::string line, ObjToken token);
	void GetVertices(std::string line, std::vector<Vector3>& positions, std::vector<Vector2>& texCoords, std::vector<Vector3>& normals, std::vector<Vertex>& vertices);

public:
	ObjLoader();
	~ObjLoader(void);

	ModelData* LoadObjFile(std::string filePath);
};
#endif