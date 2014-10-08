#ifndef GLTEXTUREMANAGER_H_
#define GLTEXTUREMANAGER_H_
#include "../../stdafx.h"
#include "../../../../External/include/SOIL.h"
#include <GL/glew.h> 
#include <map>
#include "GLText.h"
#include <vector>

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

class GLTextureManager
{
    
private:
    std::map<std::string, GLuint> m_TextureMap;
    std::vector<GLbyte(*)[64]> m_letters64;
    //std::vector<GLbyte(*)[256]> m_letters256;
    
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
        GLuint texHandle = -1;
        
        if(_name == "TEXT")
        {
            texHandle = LoadText(_textureSlot);
        }
        else
            texHandle = LoadTexture(_name, _textureSlot);
    
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

GLuint LoadText(GLenum _textureSlot)
{
    // Load texture file and convert to openGL format
    GLbyte imgData[64*m_letters64.size()];
    int k = 0;
    int x = 0;
    for(int i=0; i < 8;i++)
    {
        for(int j = 0;j < m_letters64.size();j++)
        {
            for(int g=0; g < 8;g++)
            {
                imgData[k] = (*m_letters64[j])[i*8+g];//[j];
                k++;
                x++;
//                printf("%c",(*m_letters64[j])[i*8+g]);
//                if(x > 8)
//                {
//                    printf("\n");
//                    x=0;
//                }
            }
        }
        //printf("\n");
    }
//    k=0;
//    for(int i =0 ; i < 8;i++)
//    {
//        for(int j = 0; j < 4*8;j++)
//        {
//            printf("%c",imgData[k]);
//            k++;
//        }
//        k+= (m_letters64.size()-4) * 8;
//        printf("\n");
//    }
//    exit(0);
    GLuint texHandle;
    // Copy file to OpenGL
    glActiveTexture(_textureSlot);
    glGenTextures(1, &texHandle);
    glBindTexture(GL_TEXTURE_2D, texHandle);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    printf("%d\n",glGetError());
    //gluBuild2DMipmaps(GL_TEXTURE_2D, channels, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 8*m_letters64.size(), 8, 0, GL_RED, GL_BYTE, imgData);

    
    glActiveTexture(0);    
    int err = glGetError();
    
    printf("LoadText ");
    if(err)
    printf("\033[31m with error: %d", err);
    printf("\n\033[30m");
    
    return texHandle;
}

void LoadLetters()
{
    m_letters64.push_back(&_space);
    m_letters64.push_back(&_exclamation);
    m_letters64.push_back(&_quote);
    m_letters64.push_back(&_number);
    m_letters64.push_back(&_dollar);
    m_letters64.push_back(&_percent);
    m_letters64.push_back(&_ampersand);
    m_letters64.push_back(&_apostrophe);
    m_letters64.push_back(&_leftbrace);
    m_letters64.push_back(&_rightbrace);
    m_letters64.push_back(&_asterisk);
    m_letters64.push_back(&_plus);
    m_letters64.push_back(&_comma);
    m_letters64.push_back(&_minus);
    m_letters64.push_back(&_dot);
    m_letters64.push_back(&_slash);

    m_letters64.push_back(&_0);
    m_letters64.push_back(&_1);
    m_letters64.push_back(&_2);
    m_letters64.push_back(&_3);
    m_letters64.push_back(&_4);
    m_letters64.push_back(&_5);
    m_letters64.push_back(&_6);
    m_letters64.push_back(&_7);
    m_letters64.push_back(&_8);
    m_letters64.push_back(&_9);

    m_letters64.push_back(&_colon);
    m_letters64.push_back(&_semicolon);
    m_letters64.push_back(&_lessthan);
    m_letters64.push_back(&_equal);
    m_letters64.push_back(&_morethan);
    m_letters64.push_back(&_question);
    m_letters64.push_back(&_at);

    m_letters64.push_back(&A);
    m_letters64.push_back(&B);
    m_letters64.push_back(&C);
    m_letters64.push_back(&D);
    m_letters64.push_back(&E);
    m_letters64.push_back(&F);
    m_letters64.push_back(&G);
    m_letters64.push_back(&H);
    m_letters64.push_back(&I);
    m_letters64.push_back(&J);
    m_letters64.push_back(&K);
    m_letters64.push_back(&L);
    m_letters64.push_back(&M);
    m_letters64.push_back(&N);
    m_letters64.push_back(&O);
    m_letters64.push_back(&P);
    m_letters64.push_back(&Q);
    m_letters64.push_back(&R);
    m_letters64.push_back(&S);
    m_letters64.push_back(&T);
    m_letters64.push_back(&U);
    m_letters64.push_back(&V);
    m_letters64.push_back(&W);
    m_letters64.push_back(&X);
    m_letters64.push_back(&Y);
    m_letters64.push_back(&Z);
    
//    m_letters256.resize(m_letters64.size());
//    
//    for(int i=0; i < m_letters64.size();i++)
//    {
////        GLbyte* f;
////        f = (GLbyte*)malloc(sizeof(GLbyte[256]));
//
//        m_letters256[i] = (GLbyte(*)[256])malloc(256);
//    }
   

    GLbyte temp;
    
    GLbyte shade = 'm';
    
    for(int i = 0; i < m_letters64.size();i++)
    {
        for(int j = 0; j < 64; j++)
        {
            temp = (*m_letters64[i])[j];
            if(temp == '~')
            {
//                if(j-8 > 0)
//                {
//                    if((*m_letters[i])[j-8] != '~')
//                    (*m_letters[i])[j-8] += 40;  
//                }
                
                if(j+1 < 64)
                {
                    if((*m_letters64[i])[j+1] == '~')
                    {
                        if(j+8 < 64)
                            if((*m_letters64[i])[j-8] == '~' && !((*m_letters64[i])[j-1] == '~'))
                                (*m_letters64[i])[j] -= 40;
                    }
                }
            }
            
            if(temp == 0)
            {
                int a =0;
                
                if(j-8 > 0)
                {
                    if((*m_letters64[i])[j-8] == '~')
                        a++;
                }
                
                if(j+8 < 64)
                {
                    if((*m_letters64[i])[j+8] == '~')
                        a++;
                }
                
                if(j-1 > 0)
                {
                    if((*m_letters64[i])[j-1] == '~')
                        a++;
                }
                
                if(j+1 < 64)
                {
                    if((*m_letters64[i])[j+1] == '~')
                        a++;
                }
                
                if(a>0)
                    (*m_letters64[i])[j] = ' '*a;
            }
        }
    }

}

};

#endif