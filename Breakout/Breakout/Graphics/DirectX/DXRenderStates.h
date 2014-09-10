#pragma once
#ifndef DXRENDERSTATES_H
#define DXRENDERSTATES_H

#include "DXEffects.h"
#include <D3D11.h>
#include <DirectXMath.h>

class DXRenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11RasterizerState* WireframeRS;
	static ID3D11RasterizerState* NoCullRS;

	static ID3D11DepthStencilState* EqualsDSS;
	static ID3D11DepthStencilState* LessDSS;
	static ID3D11DepthStencilState* NoDSS;
	 
	static ID3D11BlendState* OpaqueBS;
	static ID3D11BlendState* AlphaToCoverageBS;
	static ID3D11BlendState* TransparentBS;
};

#endif

