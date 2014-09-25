
#include "GLWindow.h"
#include <ctime> 
#include <iostream>
#include <stdio.h>

#include <SDL/SDL.h>
#include <GL/glew.h>


GLWindow::GLWindow(void)
{
	m_width = 0;
	m_height = 0;
}

GLWindow::~GLWindow(void)
{

}

bool GLWindow::InitWindow(int _width, int _height)
{
	m_width = _width;
	m_height = _height;
        
	return InitWnd();
}

bool GLWindow::InitWnd()
{
 //Initialize SDL 
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) 
    { return false; } 
    //Create Window 
    if( SDL_SetVideoMode( m_width, m_height, 32, SDL_OPENGL ) == NULL ) 
    { return false; } 
    //Set caption 
    
    
    std::cout << "\033[32mSDL Window Success\n\033[30m";;
    
    return true; 
}

bool GLWindow::InitGL()
{
    //Initialize GLEW
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        printf( "\033[31mError initializing GLEW! %s\n\033[30m", glewGetErrorString( glewError ) );
        return false;
    }

    //Make sure OpenGL 2.1 is supported
    if( !GLEW_VERSION_2_1 )
    {
        printf( "\033[31mOpenGL 2.1 not supported!\n\033[30m" );
        return false;
    }

    //Set the viewport
    glViewport( 0.f, 0.f, m_width, m_height );

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, m_width, m_height, 0.0, 1.0, -1.0 );
    //glFrustum();
    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Enable texturing
    glEnable( GL_TEXTURE_2D );

    //Set blending
    glEnable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "\033[31mError initializing OpenGL!\033[30m\n");
        return false;
    }
    
    std::cout << "\033[32mOpenGL Initialize Success\n\033[30m";

    return true;
}

bool GLWindow::InitWndApp()
{

	return true;
}


