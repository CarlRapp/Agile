#ifndef GLTEXTUREMANAGER_H_
#define GLTEXTUREMANAGER_H_
#include "../../stdafx.h"
#include "SOIL.h"
#include <GL/glew.h> 
#include <map>

    struct TextureInfo
{
public:
    GLuint TexHandle;
    float *X, *Y, *Width, *Height;
    
    TextureInfo(GLuint texHandle, float *_x, float *_y, float *_width, float *_height)
    {
        TexHandle = texHandle;
        X = _x;
        Y = _y;
        Width = _width;
        Height = _height;
    }
    
};

class GLTextureManager{
    
private:
     std::map<std::string, GLuint> mTextureMap;
    
public:
    
    GLuint GetTexture(std::string _name)
    {
        return mTextureMap[_name];
    }

void Load2DTexture(std::string _name, GLenum textureSlot)
{
//    if(mTextureMap.find(_name) == mTextureMap.end())
//    {
//        
//        GLuint texHandle = LoadTexture(_name, textureSlot);
//    
//        mTextureMap[_name] = texHandle;
//    }
//    else
//    {
//        printf("Texture already loaded");
//        //printf(_name);
//    }
}

GLuint LoadTexture(std::string name, GLenum textureSlot)
{
    // LOAD THE TEXTURE
//    std::string filename = TEXTURE_ROOT + name;
//    int width, height, channels;
//            // Load texture file and convert to openGL format
//    unsigned char* imgData = SOIL_load_image(filename.c_str(), &width, &height, &channels, 4 );
//
//    GLuint texHandle;
//    // Copy file to OpenGL
//    glActiveTexture(textureSlot);
//    glGenTextures(1, &texHandle);
//    glBindTexture(GL_TEXTURE_2D, texHandle);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//    //gluBuild2DMipmaps(GL_TEXTURE_2D, channels, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
//    
//    return texHandle;
}

};

#endif