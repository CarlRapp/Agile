
#include "GLWindow.h"
#include <ctime> 
#include <iostream>
#include <stdio.h>

//#define GL_GLEXT_PROTOTYPES
//#define GLX_GLXEXT_PROTOTYPES

#include <SDL/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>

//#include <X11/Xlib.h>
//#include <GL/gl.h>
//#include <GL/glx.h>
//#include <GL/glu.h>
//#include <X11/Xatom.h>

//#include "GL/glext.h"


#include <string.h>
#include <stdlib.h>

//Display *d_dpy;
//Window d_win;
//GLXContext d_ctx;


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
    if( !GLEW_VERSION_4_2 )
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
    glClearColor( 1.f, 0.f, 1.f, 1.f );

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

bool GLWindow::InitGLX() 
{
//    GLenum glewError = glewInit();
//    
//	if (!(d_dpy = XOpenDisplay(NULL))) {
//		fprintf(stderr, "Couldn't open X11 display\n");
//		exit(10);
//	}
//
//	int attr[] = {
//		GLX_RGBA,
//		GLX_RED_SIZE, 1,
//		GLX_GREEN_SIZE, 1,
//		GLX_BLUE_SIZE, 1,
//		GLX_DOUBLEBUFFER,
//		None
//	};
//
//	int scrnum = DefaultScreen(d_dpy);
//	Window root = RootWindow(d_dpy, scrnum);
//    
//	int elemc;
//	GLXFBConfig *fbcfg = glXChooseFBConfig(d_dpy, scrnum, NULL, &elemc);
//	if (!fbcfg) {
//		fprintf(stderr, "Couldn't get FB configs\n");
//		exit(11);
//	}
//
//	XVisualInfo *visinfo = glXChooseVisual(d_dpy, scrnum, attr);
//
//	if (!visinfo) {
//		fprintf(stderr, "Couldn't get a visual\n");
//		exit(12);
//	}
//
//	// Window parameters
//	XSetWindowAttributes winattr;
//	winattr.background_pixel = 0;
//	winattr.border_pixel = 0;
//	winattr.colormap = XCreateColormap(d_dpy, root, visinfo->visual, AllocNone);
//	winattr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
//	unsigned long mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;
//
//	d_win = XCreateWindow(d_dpy, root, -1, -1, m_width, m_height, 0, 
//			visinfo->depth, InputOutput, visinfo->visual, mask, &winattr);
//
//	// OpenGL version 4.3, forward compatible core profile
//	int gl3attr[] = {
//        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
//        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
//        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
//        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
//		None
//    };
//
//	//d_ctx = glXCreateContextAttribsARB(d_dpy, fbcfg[0], NULL, true, gl3attr);
//
//	if (!d_ctx) {
//		fprintf(stderr, "Couldn't create an OpenGL context\n");
//		exit(13);
//	}
//
//	XFree(visinfo);
//
//	// Setting the window name
//	XTextProperty windowName;
//	windowName.value = (unsigned char *) "OpenGL compute shader demo";
//	windowName.encoding = XA_STRING;
//	windowName.format = 8;
//	windowName.nitems = strlen((char *) windowName.value);
//
//	XSetWMName(d_dpy, d_win, &windowName);
//
//	XMapWindow(d_dpy, d_win);
//	glXMakeCurrent(d_dpy, d_win, d_ctx);
//	
////	printf("OpenGL:\n\tvendor %s\n\trenderer %s\n\tversion %s\n\tshader language %s\n",
////			glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION),
////			glGetString(GL_SHADING_LANGUAGE_VERSION));
//
//	// Finding the compute shader extension
////	int extCount;
////	glGetIntegerv(GL_NUM_EXTENSIONS, &extCount);
////	bool found = false;
////	for (int i = 0; i < extCount; ++i)
////		if (!strcmp((const char*)glGetStringi(GL_EXTENSIONS, i), "GL_ARB_compute_shader")) {
////			printf("Extension \"GL_ARB_compute_shader\" found\n");
////			found = true;
////			break;
////		}
////
////	if (!found) {
////		fprintf(stderr, "Extension \"GL_ARB_compute_shader\" not found\n");
////		exit(14);
////	}
//
//	glViewport(0, 0, m_width, m_height);
//        return 1;
}

void GLWindow::SwapBuffersX() 
{
//	glXSwapBuffers(d_dpy, d_win);
}
