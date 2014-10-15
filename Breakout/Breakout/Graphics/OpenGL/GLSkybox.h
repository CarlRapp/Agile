#ifndef GLSKYBOX_H
#define	GLSKYBOX_H
#include "../../stdafx.h"
#include "../ICamera.h"
#include <GL/glew.h> 
using glm::mat4;

class GLSkybox {
private:
	GLuint mTextureHandle;
	GLuint mVAOHandle;
        GLuint vboCubeVertices;
        GLuint iboCubeIndices;

public:
	GLSkybox(std::string mapName);
	GLSkybox();
	~GLSkybox();
        void Free();
	void CreateBuffers();
	void Draw(GLuint *shaderProgHandle, ICamera *_cam);
};

#endif	/* GLSKYBOX_H */

