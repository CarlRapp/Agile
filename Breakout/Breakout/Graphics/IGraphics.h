#ifndef IGRAPHICS_H
#define IGRAPHICS_H
#include "../stdafx.h"
#include "ICamera.h"

enum DisplayMode
{
	Fullscreen, Windowed, BorderlessWindow
};

class IGraphics
{
private:
        IGraphics* m_graphics;

public:


	virtual ~IGraphics(void){};

	virtual bool InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode) = 0;
	virtual bool Init3D(DisplayMode _displayMode) = 0;
	//IGraphics* GetIGraphics();

	virtual void LoadModel(std::string _path)=0;
	virtual void LoadTexture(std::string _path){};


	virtual void Render(float _dt, ICamera* _camera) = 0;
	virtual void Update(float _dt) = 0; //DELTA TIME NYTT SHIT, LÄGG TILL I DX

	virtual void AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose,float* _explosion) = 0;

       // virtual void AddLight(VECTOR3 _worldPos, VECTOR3 _intensity, VECTOR3 _color, float _range) = 0;

	virtual void AddPointLight(int _id, VECTOR3 *_worldPos, VECTOR3 *_intensity, VECTOR3 *_color, float *_range) = 0;
	virtual void RemovePointLight(int _id) = 0;

        
	virtual void RemoveObject(int _id) = 0;

	virtual void Add2DTexture(int _id, std::string _path, float *_x, float *_y, float *_width, float *_height) = 0;
	virtual void Remove2DTexture(int _id) = 0;


	virtual void AddParticleEffect(int _id, std::string _effect, VECTOR3 *_pos, VECTOR3 *_vel) = 0;
	virtual void RemoveParticleEffect(int _id) = 0;

	virtual void AddTextObject(int _id, std::string *_text, float *_x, float *_y, float *_scale, VECTOR3 *_color, float *_effect) = 0;
	virtual void RemoveTextObject(int _id) = 0;

	virtual void ShowMouseCursor(bool _value) = 0;

	virtual void Clear(void) = 0;

	virtual void SetSky(std::string _name) = 0;
	virtual void ClearSky() = 0;


	static IGraphics* GetIGraphics();
};




#endif