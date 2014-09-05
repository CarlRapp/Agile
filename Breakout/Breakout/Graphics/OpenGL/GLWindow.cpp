
#include "GLWindow.h"
#include <ctime> 
#include <iostream>
#include <stdio.h>
//#include <io.h>
//#include <fcntl.h>


GLWindow::GLWindow(void)
{
	m_width = 0;
	m_height = 0;
}

GLWindow::~GLWindow(void)
{

}

bool GLWindow::InitWindow(int width, int height)
{
	m_width = width;
	m_height = height;
        
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
    //Initialize OpenGL if( initGL() == false ) 
    { return false; } 
    //Set caption 
    SDL_WM_SetCaption( "OpenGL Test", NULL ); 
    
    return true; 
}

bool GLWindow::InitGL()
{
    //Initialize GLEW
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
        return false;
    }

    //Make sure OpenGL 2.1 is supported
    if( !GLEW_VERSION_2_1 )
    {
        printf( "OpenGL 2.1 not supported!\n" );
        return false;
    }

    //Set the viewport
    glViewport( 0.f, 0.f, m_width, m_height );

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, m_width, m_height, 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Enable texturing
    glEnable( GL_TEXTURE_2D );

    //Set blending
    glEnable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", int( error ) );
        return false;
    }

//    //Initialize DevIL and DevILU
//    ilInit();
//    iluInit();
//    ilClearColour( 255, 255, 255, 000 );
//
//    //Check for error
//    ILenum ilError = ilGetError();
//    if( ilError != IL_NO_ERROR )
//    {
//        printf( "Error initializing DevIL! %s", iluErrorString( ilError ) );
//        return false;
//    }

    return true;
}

bool GLWindow::InitWndApp()
{

	return true;
}


