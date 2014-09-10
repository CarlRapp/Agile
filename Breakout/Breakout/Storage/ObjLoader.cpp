#include "ObjLoader.h"


ObjToken ObjLoader::GetToken(std::string objLine)
{
	std::string tokenStr;

	//	Check what the first section of
	//	the string is. (What token it is)
	for(int i = 0; i < objLine.length(); ++i)
		if(objLine[i] != ' ')
			tokenStr += objLine[i];
		else
			break;

	//	Convert the token to
	//	the correct enum.
	if(tokenStr == TOKEN_MTL_GROUP)
		return MATERIAL_GROUP;

	if(tokenStr == TOKEN_MTL_USE)
		return MATERIAL_USE;

	if(tokenStr == TOKEN_VERTEX_GROUP)
		return VERTEX_GROUP;

	if(tokenStr == TOKEN_VERTEX_POSITION)
		return VERTEX_POSITION;

	if(tokenStr == TOKEN_VERTEX_NORMAL)
		return VERTEX_NORMAL;

	if(tokenStr == TOKEN_VERTEX_TEXPOS)
		return VERTEX_TEXPOS;

	if(tokenStr == TOKEN_FACE_DATA)
		return FACE_DATA;

	//	If there is a undefined token
	//	pass this. (Usually comments/blank rows)
	return UNDEFINED;
}

Vector3 ObjLoader::ConvertToPosition(std::string line, ObjToken token)
{
	float x, y, z;
	x = y = z = 0;

	//	Index for getting the floats 
	int i = 0;
	int nMax = 0;
	std::string tString;

	switch(token)
	{
		case VERTEX_POSITION:
			i = TOKEN_VERTEX_POSITION.length();
			nMax = 3;
			break;

		case VERTEX_TEXPOS:
			i = TOKEN_VERTEX_TEXPOS.length();
			nMax = 2;
			break;

		case VERTEX_NORMAL:
			i = TOKEN_VERTEX_NORMAL.length();
			nMax = 3;
			break;
	}
	i++;

	for(int n = 0; n < nMax; ++n)
	{
		tString = "";
		while(true)
		{
			if(i >= line.length())
				break;

			if(line[i] == ' ')
			{
				++i;
				break;
			}

			tString += line[i];
			++i;
		}

		if(n == 0)
			x = (float)(atof(tString.c_str()));
		if(n == 1)
			y = (float)(atof(tString.c_str()));
		if(n == 2)
			z = (float)(atof(tString.c_str()));
	}



	return Vector3(x, y, z);
}

void ObjLoader::GetVertices(std::string line, std::vector<Vector3>& positions, std::vector<Vector2>& texCoords, std::vector<Vector3>& normals, std::vector<Vertex>& vertices)
{
	int i = TOKEN_FACE_DATA.length() + 1;

	// Ugly variable names, dont follow this example, I did not make this!!! - Carl
	std::string	tempData;

	int**	vData			=	new int*[3];
	int		indexDivider	=	0;

	//	This will divide the string up into
	//	three sub sections, each with its
	//	own vertex data
	for(int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
	{
		vData[vertexIndex]	=	new int[3];

		while(true)
		{
			if(i >= line.length())
			{
				vData[vertexIndex][indexDivider]	=	stoi(tempData);
				break;
			}

			if(line[i] == ' ')
			{
				vData[vertexIndex][indexDivider]	=	stoi(tempData);
				break;
			}

			if(line[i] == '/')
			{
				vData[vertexIndex][indexDivider]	=	stoi(tempData);
				++indexDivider;
				tempData = "";
			}
			else
				tempData	+=	line[i];

			++i;
		}

		++i;
		indexDivider = 0;
		tempData = "";
	}

	
	for(int i = 0; i < 3; ++i)
	{
		vertices.push_back(
			Vertex(
				positions[vData[i][0] - 1], 
				texCoords[vData[i][1] - 1], 
				normals[vData[i][2] - 1]
			)
		);
	}
}

ModelData* ObjLoader::LoadObjFile(std::string filePath)
{
	std::vector<Vector3> vertexPositions = std::vector<Vector3>();
	std::vector<Vector2> texturePositions = std::vector<Vector2>();
	std::vector<Vector3> vertexNormals = std::vector<Vector3>();
	std::vector<Vertex> vertices = std::vector<Vertex>();

	std::ifstream myFile(filePath);
	std::string currentLine;
	ObjToken currentToken;
	
	if (!myFile.bad())
	{
		while (getline(myFile, currentLine))
		{
			int a = 2;
			if (currentLine.length() > 0)
			{
				currentToken = GetToken(currentLine);
				switch (currentToken)
				{
				case VERTEX_POSITION:
					vertexPositions.push_back(ConvertToPosition(currentLine, currentToken));
					break;

				case VERTEX_TEXPOS:
					texturePositions.push_back(ConvertToPosition(currentLine, currentToken).ToVector2());
					break;

				case VERTEX_NORMAL:
					vertexNormals.push_back(ConvertToPosition(currentLine, currentToken));
					break;

				case FACE_DATA:
					GetVertices(currentLine, vertexPositions, texturePositions, vertexNormals, vertices);
					break;
				}
			}
		}
	}

	myFile.close();

	return new ModelData(vertices);
}
/*
ObjLoader::ObjLoader(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, std::string ObjFile)
{
	vertexPositions		=	vector<Position>();
	vertexTexturePos	=	vector<Position>();
	vertexNormals		=	vector<Position>();
	objVertices			=	vector<Vertex>();


	std::ifstream myfile;
	myfile.open (ObjFile);
	std::string sline;
	ObjToken token;

	while (!myfile.eof())
	{
		getline(myfile, sline);

		if(sline.length() > 0)
		{
			token = Gettoken(sline);
			switch(token)
			{
				case VERTEX_POSITION:
					vertexPositions.push_back(ConvertToPosition(sline, token));
				break;

				case VERTEX_TEXPOS:
					vertexTexturePos.push_back(ConvertToPosition(sline, token));
				break;

				case VERTEX_NORMAL:
					vertexNormals.push_back(ConvertToPosition(sline, token));
				break;

				case FACE_DATA:
					GetVertices(sline, vertexPositions, vertexTexturePos, vertexNormals, objVertices);
				break;
			}
		}
	}
	myfile.close();

	Vertex*	vertices	=	new Vertex[objVertices.size()];
	for(int i = 0; i < objVertices.size(); ++i)
		vertices[i]	=	objVertices[i];

}*/