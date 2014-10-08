#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include "IGraphics.h"
#include "ICamera.h"
#include "../stdafx.h"

#ifdef WINDOWS
#include <windows.h>
#endif

#ifdef DIRECTX
#include <D3D11.h>
#include <d3dCompiler.h>
#endif

struct Texture2DData
{
public:
	Texture2DData()
	{
		m_imageWidth = m_imageHeight = 0;
		m_positionX = m_positionY = 0;
		m_textureName = "";
	}

	float m_imageWidth, m_imageHeight;
	float m_positionX, m_positionY;
	std::string m_textureName;
};

class GraphicsManager
{
private:
	GraphicsManager();
	static GraphicsManager* m_GMInstance;

	IGraphics *m_IGraphics;
	ICamera	*m_ICamera;

public:

    ~GraphicsManager(void);
    static GraphicsManager* GetInstance();
    static void Destroy();


    bool        InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode);
    IGraphics*  GetIGraphics() { return m_IGraphics; }
	void        Render(float _dt);
    void        Update();
    ICamera*    GetICamera();

	void        AddPointLight(int _id, VECTOR3 *_worldPos, VECTOR3 *_intensity, VECTOR3 *_color, float *_range);
    void        AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose,float* _explosion);
    void        RemoveObject(int _id);



	void Add2DTexture(int _id, std::string _path, float *_x, float *_y, float *_width, float *_height);
	void Remove2DTexture(int _id);
	void RemovePointLight(int _id);
	void AddTextObject(int _id, std::string *_text, float *_x, float *_y, float *_scale, VECTOR3 *_color, float *_effect);

	void RemoveTextObject(int _id);

	void		AddEffect(int _id, std::string _effect, VECTOR3 *_pos, VECTOR3 *_vel);
	void		RemoveEffect(int _id);

    bool        Init3D(DisplayMode _displayMode);
};



#endif