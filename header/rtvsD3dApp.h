////////////////////////////////////////////////////////////////////////////////




// ---------- rtvsD3dApp.h ----------

/*!

\file rtvsD3dWinLite.h
\brief interface for the rtvsD3dApp class
\author Gareth Edwards

*/


#ifndef _rtvsD3dApp_
#define _rtvsD3dApp_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




// ---------- include ----------

#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../header/vector.h"



////////////////////////////////////////////////////////////////////////////////




// ---------- FVF - Flexible Vertex Formats ----------

struct LineVertex
{
    float x , y, z;
 
    enum FVF
    {
        FVF_Flags = D3DFVF_XYZ
    };
};


struct QuadVertex
{
    float x , y, z;
    float nx, ny, nz;
	DWORD diffuse;
    float tu, tv;

    enum FVF
    {
        FVF_Flags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1
    };
};




// ---------- rtvsD3dApp class interface ----------


class rtvsD3dApp  
{

	// ---- properties ----

	DWORD	_id;


public:


	// ---- properties ----

	ID3DXFont*				pFont;
	D3DCOLOR				fontCol;
	LPDIRECT3DVERTEXBUFFER9	pAxisVertexBuffer;
	LPDIRECT3DVERTEXBUFFER9	pQuadVertexBuffer;
	LPDIRECT3DVERTEXBUFFER9	pVectVertexBuffer;
	DWORD					numAxisLines;
	DWORD					numQuadTriangles;
	DWORD					numVectLines;
	LPDIRECT3DTEXTURE9		pTexture;
	D3DMATERIAL9			lineMtrl;
	D3DMATERIAL9			vectorStartMtrl;
	D3DMATERIAL9			vectorResultMtrl;
	D3DMATERIAL9			quadMtrl;
	D3DLIGHT9				sunLight;
	D3DLIGHT9				backLight;
	DWORD					currentKeyClicked;
	DWORD					previousKeyClicked;
	DWORD					vectorLineCount;
	float					fSpinX;
	float					fSpinY;
	char*					vectorName;

	// ---- methods ----

	// constructor/destructor
	rtvsD3dApp (int);


	// framework
	bool cleanup	();
	bool cleanupDX	(LPDIRECT3DDEVICE9);
	bool display	(LPDIRECT3DDEVICE9);
	bool setup		();
	bool setupDX	(LPDIRECT3DDEVICE9);

	// vector
	DWORD vectorAdd			();
	DWORD vectorSubtract	();
	DWORD vectorMultiply	();
	DWORD vectorDivide		();
	DWORD vectorNegate		();
	DWORD vectorNormalise	();
	DWORD vectorCross		();
	DWORD vectorDot			();
	DWORD vectorLine		(int, Vector3D, Vector3D);

	// gui
	bool updateKeyboard ();

	// other
	void getSpin	(float*, float*);
	void setSpin	(float, float);

};




////////////////////////////////////////////////////////////////////////////////




#endif // _rtvsD3dApp_




