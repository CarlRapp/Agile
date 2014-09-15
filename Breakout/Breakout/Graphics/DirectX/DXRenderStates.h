#pragma once
#ifndef DXRENDERSTATES_H
#define DXRENDERSTATES_H

#include "DXEffects.h"
#include <D3D11.h>
#include <DirectXMath.h>

class DXRenderStates
{
public:
	static void InitAll(ID3D11Device* _device);
	static void DestroyAll();

	static ID3D11RasterizerState* m_wireframeRS;
	static ID3D11RasterizerState* m_noCullRS;

	static ID3D11DepthStencilState* m_equalsDSS;
	static ID3D11DepthStencilState* m_lessDSS;
	static ID3D11DepthStencilState* m_noDSS;
	 
	static ID3D11BlendState* m_opaqueBS;
	static ID3D11BlendState* m_alphaToCoverageBS;
	static ID3D11BlendState* m_transparentBS;
};

#endif

