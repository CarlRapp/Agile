#include "IGraphics.h"

IGraphics* IGraphics::GetIGraphics()
{
//    
//#ifdef DIRECTX
//	return new DXGraphics();
//#else
//        m_graphics = new GLGraphics();
//        return m_graphics;
//#endif
}

void IGraphics::Render()
{
    
}

//void loop()
//{
// //While there are events to handle 
//    while(true)
//    {
//        while( SDL_PollEvent( &event ) ) 
//        { 
//
//            if( event.type == SDL_QUIT ) 
//            { 
//                quit = true; 
//            } 
//            else if( event.type == SDL_KEYDOWN ) 
//            { 
//                //Handle keypress with current mouse position 
//
//                int x = 0, y = 0; 
//                SDL_GetMouseState( &x, &y ); 
//                handleKeys( event.key.keysym.unicode, x, y ); 
//            } 
//        }
//
//
//        //Run frame update 
//        update(); 
//        //Render frame 
//        render(); 
//        //Cap the frame rate 
//        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) 
//        { 
//            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() ); 
//        }
//    }
//}