#include "LinInput.h"
#include <stdio.h>
#include <vector>
#include <SDL/SDL.h>

LinInput::LinInput()
{
	m_mousePointer = new Mouse();
	m_keyboardPointer = new Keyboard();
        SDL_EnableKeyRepeat(0,0);
}

LinInput::~LinInput()
{
	delete m_mousePointer;
	delete m_keyboardPointer;
}

void LinInput::Update()
{
    if (m_mousePointer != 0 && m_keyboardPointer != 0)
    {
        m_mousePointer->Update();
        m_keyboardPointer->Update();
        
            
    }

}



Mouse* LinInput::GetMouse()
{
	return m_mousePointer;
}

Keyboard* LinInput::GetKeyboard()
{
	return m_keyboardPointer;
}

/*
	Windows Specific implementation
	of Mouse and Keyboard
*/
#pragma region Keyboard implementation
Keyboard::Keyboard()
{
	for (int n = 0; n < KEYBOARDKEYS; ++n)
	{
		m_lastFrameKeys[n] = 0;
		m_thisFrameKeys[n] = 0;
	}
}

void Keyboard::Update()
{
    for(int i = 0; i < KEYBOARDKEYS; ++i)
        m_lastFrameKeys[i] = m_thisFrameKeys[i];
    
    //std::vector<SDL_Event> sdlEvents = std::vector<SDL_Event>();
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_KEYDOWN:
                    m_thisFrameKeys[e.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                    m_thisFrameKeys[e.key.keysym.sym] = false;
                break;
                
            default:
                //sdlEvents.push_back(e);
                break;
        }
        
    }
       
    /*for(int i = sdlEvents.size() - 1; i >= 0; --i)
        SDL_PushEvent(&sdlEvents[i]);
    
    sdlEvents.clear();*/
}

InputState Keyboard::GetKeyState(char _key)
{
    _key = tolower(_key);
    if (_key < KEYBOARDKEYS)
    {
        bool	tLastFrame = m_lastFrameKeys[_key];
        bool	tThisFrame = m_thisFrameKeys[_key];

        if (!tLastFrame && tThisFrame)
                return InputState::Pressed;

        else if (tLastFrame && tThisFrame)
                return InputState::Down;

        else if (tLastFrame && !tThisFrame)
                return InputState::Released;

    }
    return InputState::Up;
}
#pragma endregion

#pragma region Mouse implementation
Mouse::Mouse()
{
	for (int n = 0; n < MOUSEBUTTONS; ++n)
	{
		m_lastFrameButtons[n] = 0;
		m_thisFrameButtons[n] = 0;
	}

	m_positionX = 0;
	m_positionY = 0;
	m_dX = 0;
	m_dY = 0;
}
void Mouse::Update()
{
    for(int i = 0; i < MOUSEBUTTONS; ++i)
        m_lastFrameButtons[i] = m_thisFrameButtons[i];
    
    std::vector<SDL_Event> sdlEvents = std::vector<SDL_Event>();
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            switch(e.button.button)
            {
                case SDL_BUTTON_LEFT:
                    m_thisFrameButtons[0] = true;
                    break;
                case SDL_BUTTON_RIGHT:
                    m_thisFrameButtons[1] = true;
                    break;
                case SDL_BUTTON_MIDDLE:
                    m_thisFrameButtons[MouseButton::MiddleMB] = true;
                    break;
            }
        }
        else if(e.type == SDL_MOUSEBUTTONUP)
        {
            switch(e.button.button)
            {
                case SDL_BUTTON_LEFT:
                    m_thisFrameButtons[0] = false;
                    break;
                case SDL_BUTTON_RIGHT:
                    m_thisFrameButtons[1] = false;
                    break;
                case SDL_BUTTON_MIDDLE:
                    m_thisFrameButtons[MouseButton::MiddleMB] = false;
                    break;
            } 
        }
        else if(e.type == SDL_MOUSEMOTION)
        {

        }    
        else
            sdlEvents.push_back(e);
        
    }
    
    int tX = m_positionX;
    int tY = m_positionY;
    SDL_GetMouseState(&m_positionX, &m_positionY);
    m_dX = m_positionX - tX;
    m_dY = m_positionY - tY;

    
    for(int i = sdlEvents.size() - 1; i >= 0; --i)
        SDL_PushEvent(&sdlEvents[i]);
    
    sdlEvents.clear();
}
int Mouse::getdX() { return m_dX; }
int Mouse::getdY() { return m_dY; }
int Mouse::getX() { return m_positionX; }
int Mouse::getY() { return m_positionY; }

InputState Mouse::GetButtonState(char _button)
{
	if (_button < MOUSEBUTTONS)
	{
		bool	tLastFrame = m_lastFrameButtons[_button];
		bool	tThisFrame = m_thisFrameButtons[_button];

		if (!tLastFrame && tThisFrame)
			return InputState::Pressed;

		else if (tLastFrame && tThisFrame)
			return InputState::Down;

		else if (tLastFrame && !tThisFrame)
			return InputState::Released;
	}

	return InputState::Up;
}
#pragma endregion
