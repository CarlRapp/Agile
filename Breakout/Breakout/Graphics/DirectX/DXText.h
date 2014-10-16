#pragma once
#ifndef DXTEXT_H
#define DXTEXT_H

#include <D3D11.h>
#include <vector>
#include <limits>
#include "../../stdafx.h"

namespace DXText
{

	struct String
	{
		std::string *Text;
		float *X = NULL;
		float *Y = NULL;
		float *Scale = NULL;
		VECTOR3 *Color = NULL;
		float *Effect = NULL;
		std::vector<int> Indices;
	};

	const static float x = 1.0f;
	const static float e = 33.0f / 126.0f;
	const static float AA = 65.0f / 126.0f;
	const static float a = 97.0f / 126.0f;

	static float E[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
							0, x, x, x, x, x, x, 0,
							0, x, 0, 0, 0, 0, 0, 0,
							0, x, 0, 0, 0, 0, 0, 0,
							0, x, x, x, x, 0, 0, 0,
							0, x, 0, 0, 0, 0, 0, 0,
							0, x, x, x, x, x, x, 0,
							0, 0, 0, 0, 0, 0, 0, 0, };

	static float A[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
							0, x, 0, 0, 0, 0, x, 0,
							0, x, 0, 0, 0, 0, x, 0,
							0, x, x, x, x, x, x, 0,
							0, x, 0, 0, 0, 0, x, 0,
							0, 0, x, 0, 0, x, 0, 0,
							0, 0, 0, x, x, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0, };

	static float B[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float C[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float D[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float F[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float G[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, x, x, x, x, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float H[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, x, x, x, x, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float I[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, x, x, x, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float J[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float K[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, x, 0, 0,
	0, x, 0, 0, x, 0, 0, 0,
	0, x, x, x, 0, 0, 0, 0,
	0, x, 0, 0, x, 0, 0, 0,
	0, x, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float L[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float M[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, x, x, 0, x, 0,
	0, x, x, 0, 0, x, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float N[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, x, x, 0,
	0, x, 0, 0, x, 0, x, 0,
	0, x, 0, x, 0, 0, x, 0,
	0, x, x, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float O[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float P[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float Q[64] = { 0, 0, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, x, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float R[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float S[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, x, x, 0, 0,
	0, 0, x, x, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float T[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, x, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float U[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float V[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, x, 0, 0, 0,
	0, 0, x, 0, 0, x, 0, 0,
	0, 0, x, 0, 0, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float W[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, 0, 0, x, 0, 0,
	0, x, 0, x, x, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float X[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, x, 0, 0, x, 0, 0,
	0, 0, 0, x, x, 0, 0, 0,
	0, 0, x, 0, 0, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float Y[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, x, 0, x, 0, 0,
	0, 0, x, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float Z[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _1[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, x, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, x, 0, x, 0, 0, 0,
	0, 0, 0, x, x, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _2[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _3[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, x, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _4[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, 0, x, 0, 0, x, 0, 0,
	0, 0, 0, x, 0, x, 0, 0,
	0, 0, 0, 0, x, x, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _5[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, x, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _6[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _7[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _8[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _9[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, x, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _0[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, x, x, 0, 0,
	0, 0, x, 0, 0, 0, x, 0,
	0, 0, x, 0, 0, 0, x, 0,
	0, 0, x, 0, 0, 0, x, 0,
	0, 0, x, 0, 0, 0, x, 0,
	0, 0, 0, x, x, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _colon[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _semicolon[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _lessthan[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _equal[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _morethan[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _question[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, x, 0, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, x, 0, 0, 0, x, 0, 0,
	0, 0, x, x, x, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _at[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, x, x, x, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, x, 0, x, x, x, x, x,
	0, x, 0, x, 0, 0, x, x,
	0, x, 0, 0, x, x, 0, x,
	0, 0, x, 0, 0, 0, 0, x,
	0, 0, 0, x, x, x, x, 0, };

	static float _space[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _exclamation[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _quote[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, x, 0, 0,
	0, 0, x, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _number[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, 0, 0, x, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, 0, x, 0, 0, x, 0, 0,
	0, 0, x, 0, 0, x, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, 0, x, 0, 0, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _dollar[64] = { 0, 0, x, 0, 0, x, 0, 0,
	0, 0, x, x, x, x, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, x, x, 0, 0,
	0, 0, x, x, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, x, 0,
	0, 0, x, x, x, x, 0, 0,
	0, 0, 0, x, 0, x, 0, 0, };

	static float _percent[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, x, 0, 0,
	0, 0, x, 0, x, 0, x, 0,
	0, 0, 0, x, 0, x, 0, 0,
	0, 0, x, 0, x, 0, 0, 0,
	0, x, 0, x, 0, x, 0, 0,
	0, 0, x, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _ampersand[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, x, 0, x, 0,
	0, x, 0, 0, 0, x, 0, 0,
	0, x, 0, 0, 0, x, x, 0,
	0, 0, x, x, x, 0, 0, 0,
	0, x, 0, 0, 0, x, 0, 0,
	0, 0, x, x, x, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _apostrophe[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _leftbrace[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _rightbrace[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _asterisk[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, 0, x, 0, x, 0,
	0, 0, 0, x, x, x, 0, 0,
	0, 0, 0, x, x, x, 0, 0,
	0, 0, x, 0, x, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _plus[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, x, x, x, x, x, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _comma[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _minus[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, x, x, x, x, x, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _dot[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, x, x, 0, 0, 0, 0,
	0, 0, x, x, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _slash[64] = { 0, 0, 0, 0, 0, 0, 0, 0,
	0, x, 0, 0, 0, 0, 0, 0,
	0, 0, x, 0, 0, 0, 0, 0,
	0, 0, 0, x, 0, 0, 0, 0,
	0, 0, 0, 0, x, 0, 0, 0,
	0, 0, 0, 0, 0, x, 0, 0,
	0, 0, 0, 0, 0, 0, x, 0,
	0, 0, 0, 0, 0, 0, 0, 0, };

	static float _expArrow[64] = { e, AA, a, x, x, 0, 0, 0,
		0, e, AA, a, x, x, 0, 0,
		0, 0, e, AA, a, x, x, 0,
		0, 0, 0, e, AA, a, x, x,
		0, 0, 0, e, AA, a, x, x,
		0, 0, e, AA, a, x, x, 0,
		0, e, AA, a, x, x, 0, 0,
		e, AA, a, x, x, 0, 0, 0, };

}
#endif