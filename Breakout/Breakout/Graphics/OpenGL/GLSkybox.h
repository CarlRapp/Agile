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


public:
	GLSkybox(std::string mapName);
	GLSkybox();
	~GLSkybox();

	void CreateBuffers();
	void Draw(GLuint *shaderProgHandle, ICamera *_cam);
};

#endif	/* GLSKYBOX_H */

