#ifndef GLTEXTUREMANAGER_H_
#define GLTEXTUREMANAGER_H_
#include "../../stdafx.h"
#include "../../../../External/include/SOIL.h"
#define GLEW_STATIC
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
     std::map<std::string, GLuint> m_TextureMap;
    
public:
    
    GLuint GetTexture(std::string _name)
    {
        return m_TextureMap[_name];
    }
    void Free()
    {
        m_TextureMap.clear();
    }

void Load2DTexture(std::string _name, GLenum _textureSlot)
{
    if(m_TextureMap.find(_name) == m_TextureMap.end())
    {
        GLuint texHandle = LoadTexture(_name, _textureSlot);
    
        m_TextureMap[_name] = texHandle;
    }
    else
    {
        printf("Texture already loaded: %s \n", _name.c_str());
    }
}

GLuint LoadTexture(std::string _name, GLenum _textureSlot)
{
    // LOAD THE TEXTURE
    
    std::string filename = GetFile(_name, TEXTURE_ROOT);
    
    printf("Filename: %s \n", filename.c_str());
    
    int width, height, channels;
    // Load texture file and convert to openGL format
    unsigned char* imgData = SOIL_load_image((const char*)filename.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA );
    
    if(!imgData)
        printf("Texture data NULL when loading texture: %s \n", _name.c_str());

    GLuint texHandle;
    // Copy file to OpenGL
    glActiveTexture(_textureSlot);
    glGenTextures(1, &texHandle);
    glBindTexture(GL_TEXTURE_2D, texHandle);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //gluBuild2DMipmaps(GL_TEXTURE_2D, channels, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
    
        
    int err = glGetError();
    
    printf("LoadTexture ");
    if(err)
    printf("\033[31m with error: %d", err);
    printf("\n\033[30m");
    
    return texHandle;
}

};

#endif