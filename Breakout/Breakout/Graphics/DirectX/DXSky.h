//***************************************************************************************
// Sky.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Simple class that renders a sky using a cube map.
//***************************************************************************************

#ifndef DXSKY_H
#define DXSKY_H

#include <D3D11.h>
#include <string>
#include "DXTextureManager.h"
#include "../ICamera.h"

class DXSky
{
public:
	DXSky(ID3D11Device* _device, std::string _cubemapFilename, DXTextureManager* _texMgr);
	~DXSky();

	ID3D11ShaderResourceView* CubeMapSRV();

	void Draw(ID3D11DeviceContext* _dc, ICamera* _camera);

private:
	DXSky(const DXSky& _rhs);
	DXSky& operator=(const DXSky& rhs);

private:
	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	ID3D11ShaderResourceView* m_cubeMapSRV;

	void GenerateSphear(ID3D11Device* _device);

	UINT m_indexCount;
};

#endif // SKY_H