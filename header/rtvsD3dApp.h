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
#include <AntTweakBar.h>


////////////////////////////////////////////////////////////////////////////////




// ---------- FVF - Flexible Vertex Formats ----------
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


struct meshVertex
{
    float x, y, z;
    float nx, ny, nz;
    float tu, tv;

    enum FVF
    {
        FVF_Flags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
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

	LPDIRECT3DVERTEXBUFFER9	pVertexBuffer;
	LPDIRECT3DTEXTURE9		waveTexture;
	D3DMATERIAL9			quadMtrl;
	D3DLIGHT9				surfaceLight;
	float					fSpinX;
	float					fSpinY;
	double					tSpin;
	int						tCount;
	D3DLIGHT9				sunLight;
	D3DLIGHT9				backLight;
	DWORD					currentKeyClicked;
	DWORD					previousKeyClicked;
	DWORD					vectorLineCount;
	char*					vectorName;

	// mesh

	DWORD               vertexFormat;	//!< FVF - flexible vertex format
	DWORD               vertexSize;		//!< size of vertex format (e.g. sizeof(vertex3x))
	DWORD               vertices;		//!< number of vertices
	DWORD               faces;			//!< number of faces
	DWORD               vtxCols;		//!< number of vertex columns
	DWORD               vtxRows;		//!< number of vertex rows
	DWORD               celCols;		//!< number of cell rows
	DWORD               celRows;		//!< number of cell rows
	LPD3DXMESH          pMesh;			//!< the d3d object
	float				meshParams[8];
	int					meshVertsWidth;
	int					meshVertsDepth;


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

	bool plane (LPDIRECT3DDEVICE9, int,	int, float*);   // mesh parameters
	bool meshUpdateY (float*, float);
	void meshNormal (meshVertex*, meshVertex*, meshVertex*, meshVertex*);

	float sineWaveParameters[7];
	float frame;
	bool wireframe;
};




////////////////////////////////////////////////////////////////////////////////




#endif // _rtvsD3dApp_




