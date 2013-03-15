////////////////////////////////////////////////////////////////////////////////




// ---------- rtvsD3dApp.cpp ----------

/*!

\file rtvsD3dApp.cpp
\brief Implementation of the rtvsD3dApp class
\author Gareth Edwards

*/




// ---------- include ----------

#include "../header/rtvsD3dApp.h"
#include "../header/vector.h"




////////////////////////////////////////////////////////////////////////////////




// ---------- constructor ----------

/*!

\brief constructor
\author Gareth Edwards

\param int (id of this instance)


*/

rtvsD3dApp::rtvsD3dApp (int id)
{

	// initialise
    ZeroMemory( this, sizeof(rtvsD3dApp) );

	// store id
	_id = id;

	// key clicked
	currentKeyClicked = 1;

}




////////////////////////////////////////////////////////////////////////////////




// ---------- framework ----------




// ---------- framework : cleanup ----------

/*!

\brief framework : cleanup
\author Gareth Edwards

\return bool (TRUE if ok)

*/

bool rtvsD3dApp::cleanup ()
{

	// ok
	return true;

}




// ---------- framework : cleanup dx ----------

/*!

\brief framework : cleanup dx
\author Gareth Edwards

\param LPDIRECT3DDEVICE9	(device)

\return bool (TRUE if ok)

*/

bool rtvsD3dApp::cleanupDX (LPDIRECT3DDEVICE9 pd3dDevice)
{

     // ---- invalidate the font object ----

    if( pFont != NULL )
    {
        int nNewRefCount = pFont->Release();

        if( nNewRefCount > 0 )
        {
            static char strError[256];
            sprintf_s ( strError, 256,
				"The font object failed to cleanup properly.\n"
                "Release() returned a reference count of %d",
				nNewRefCount );
            MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
        }

        pFont = NULL;
    }


   // ---- invalidate the texture object ----

    if( pTexture != NULL )
    {
        int nNewRefCount = pTexture->Release();

        if( nNewRefCount > 0 )
        {
            static char strError[256];
            sprintf_s ( strError, 256,
				"The texture object failed to cleanup properly.\n"
                "Release() returned a reference count of %d",
				nNewRefCount );
            MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
        }

        pTexture = NULL;
    }


    // ---- invalidate the axis vertex buffer object ----

    if( pAxisVertexBuffer != NULL )
    {
        int nNewRefCount = pAxisVertexBuffer->Release();

        if( nNewRefCount > 0 )
        {
            static char strError[256];
            sprintf_s ( strError, 256,
				"The axis vertex buffer object failed to cleanup properly.\n"
                "Release() returned a reference count of %d",
				nNewRefCount );
            MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
        }
        pAxisVertexBuffer = NULL;
    }

	
	// ---- invalidate the quad vertex buffer object ----

    if( pQuadVertexBuffer != NULL )
    {
        int nNewRefCount = pQuadVertexBuffer->Release();

        if( nNewRefCount > 0 )
        {
            static char strError[256];
            sprintf_s ( strError, 256,
				"The quad vertex buffer object failed to cleanup properly.\n"
                "Release() returned a reference count of %d",
				nNewRefCount );
            MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
        }
        pQuadVertexBuffer = NULL;
    }


    // ---- invalidate the vector vertex buffer object ----

    if( pVectVertexBuffer != NULL )
    {
        int nNewRefCount = pVectVertexBuffer->Release();

        if( nNewRefCount > 0 )
        {
            static char strError[256];
            sprintf_s ( strError, 256,
				"The line vertex buffer object failed to cleanup properly.\n"
                "Release() returned a reference count of %d",
				nNewRefCount );
            MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
        }
        pVectVertexBuffer = NULL;
    }


	// ok
	return true;

}




// ---------- framework : display ----------

/*!

\brief framework : display
\author Gareth Edwards

\param LPDIRECT3DDEVICE9	(device)

\return bool (TRUE if ok)

*/

bool rtvsD3dApp::display (LPDIRECT3DDEVICE9 pd3dDevice)
{

 	// clear backbuffers
    pd3dDevice->Clear( 0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_COLORVALUE(0.0f,0.3f,0.7f,1.0f),
		1.0f,
		0);

   // local matrices
    D3DXMATRIX matView;
    D3DXMATRIX matRotation;
    D3DXMATRIX matTranslation;
    D3DXMATRIX matWorld;

	// display flags
	bool axis = true;
	bool quad = true;
	bool vectors = true;
	bool quadWire = true;
	bool quadSolid = true;


	// view matrix
    D3DXMatrixIdentity( &matView );
    pd3dDevice->SetTransform( D3DTS_VIEW, &matView );


	// rotation matrix
	D3DXMatrixRotationYawPitchRoll	( &matRotation, D3DXToRadian(fSpinX), D3DXToRadian(fSpinY), 0 );


	// set render states
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	// locate
	D3DXMatrixTranslation( &matTranslation, 0, -2, 15 );
	matWorld = matRotation * matTranslation;
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );


	// IF axis THEN
	if (axis)
	{
		pd3dDevice->SetMaterial( &lineMtrl );
		pd3dDevice->SetTexture( 0, 0 );
		pd3dDevice->SetStreamSource( 0, pAxisVertexBuffer, 0, sizeof(LineVertex) );
		pd3dDevice->SetFVF( LineVertex::FVF_Flags );
		pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, numAxisLines );
	}


	// IF quad THEN
	if (quad)
	{

		// display solid textured quad
		if (quadSolid)
		{
			pd3dDevice->SetMaterial( &quadMtrl );
			pd3dDevice->SetTexture( 0, pTexture );
			pd3dDevice->SetStreamSource( 0, pQuadVertexBuffer, 0, sizeof(QuadVertex) );
			pd3dDevice->SetFVF( QuadVertex::FVF_Flags );
			pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, numQuadTriangles );
		}

		// display wireframe quad
		if (quadWire)
		{
			pd3dDevice->SetMaterial( &lineMtrl );
			pd3dDevice->SetTexture( 0, 0 );
			pd3dDevice->SetStreamSource( 0, pQuadVertexBuffer, 0, sizeof(QuadVertex) );
			pd3dDevice->SetFVF( QuadVertex::FVF_Flags );
			pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, numQuadTriangles );
		}

	}


	// IF vectors THEN
	if (vectors)
	{

		// update current key clicked (if any)
		updateKeyboard ();

		// update??
		if (currentKeyClicked != previousKeyClicked)
		{
			// calculate
			switch (currentKeyClicked)
			{
				case 1:
					vectorLineCount = vectorAdd();
					vectorName = "Add";
					break;
				case 2:
					vectorLineCount = vectorSubtract();
					vectorName = "Subtract";
					break;
				case 3:
					vectorLineCount = vectorMultiply();
					vectorName = "Multiply";
					break;
				case 4:
					vectorLineCount = vectorDivide();
					vectorName = "Divide";
					break;
				case 5:
					vectorLineCount = vectorNegate();
					vectorName = "Negate";
					break;
				case 6:
					vectorLineCount = vectorNormalise();
					vectorName = "Normalise";
					break;
				case 7:
					vectorLineCount = vectorCross();
					vectorName = "Cross Product";
					break;
				case 8:
					vectorLineCount = vectorDot();
					vectorName = "Dot Product";
					break;
				default:
					break;
			}
			previousKeyClicked = currentKeyClicked;
		}

		// draw
		if (vectorLineCount > 0 || vectorLineCount < 4)
		{
			RECT rect = { 40, 40, 900, 100 };
			pFont->DrawText(0,vectorName,-1,&rect,0,fontCol);
			pd3dDevice->SetTexture( 0, 0 );
			pd3dDevice->SetStreamSource( 0, pVectVertexBuffer, 0, sizeof(LineVertex) );
			pd3dDevice->SetFVF( LineVertex::FVF_Flags );
			pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			pd3dDevice->SetMaterial( &vectorStartMtrl );
			pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, vectorLineCount-1 );
			pd3dDevice->SetMaterial( &vectorResultMtrl );
			pd3dDevice->DrawPrimitive( D3DPT_LINELIST, (vectorLineCount-1)*2, 1 );
		}
	}


	// ok
	return true;

}




// ---------- framework : setup ----------

/*!

\brief framework : setup
\author Gareth Edwards

\return bool (TRUE if ok)

*/

bool rtvsD3dApp::setup ()
{

    // setup a material for the lines
    ZeroMemory( &lineMtrl, sizeof(D3DMATERIAL9) );
	lineMtrl.Emissive.r = 0.5f;
	lineMtrl.Emissive.g = 0.5f;
	lineMtrl.Emissive.b = 1.0f;

	// setup a material for the start vectors
    ZeroMemory( &vectorStartMtrl, sizeof(D3DMATERIAL9) );
	vectorStartMtrl.Emissive.r = 0.3f;
	vectorStartMtrl.Emissive.g = 1.0f;
	vectorStartMtrl.Emissive.b = 0.3f;

	// setup a material for the result vectors
    ZeroMemory( &vectorResultMtrl, sizeof(D3DMATERIAL9) );
	vectorResultMtrl.Emissive.r = 1.0f;
	vectorResultMtrl.Emissive.g = 0.3f;
	vectorResultMtrl.Emissive.b = 0.3f;

	// setup a material for the textured quad
    ZeroMemory( &quadMtrl, sizeof(D3DMATERIAL9) );
	quadMtrl.Ambient.r = 1.0f;
	quadMtrl.Ambient.g = 1.0f;
	quadMtrl.Ambient.b = 1.0f;
	quadMtrl.Ambient.a = 1.0f;
    quadMtrl.Diffuse.r = 1.0f;
    quadMtrl.Diffuse.g = 1.0f;
    quadMtrl.Diffuse.b = 1.0f;
    quadMtrl.Diffuse.a = 1.0f;
    quadMtrl.Specular.r = 1.0f;
    quadMtrl.Specular.g = 1.0f;
    quadMtrl.Specular.b = 1.0f;
    quadMtrl.Specular.a = 1.0f;
    quadMtrl.Power = 40.0f;

    // setup directional sun light
	ZeroMemory( &sunLight, sizeof(D3DLIGHT9) );
	sunLight.Type = D3DLIGHT_DIRECTIONAL;
    sunLight.Direction = D3DXVECTOR3( -1.0f, -1.0f,  1.0f );
    sunLight.Diffuse.r = 1.0f;
    sunLight.Diffuse.g = 1.0f;
    sunLight.Diffuse.b = 1.0f;
    sunLight.Diffuse.a = 1.0f;
    sunLight.Specular.r = 1.0f;
    sunLight.Specular.g = 1.0f;
    sunLight.Specular.b = 1.0f;
    sunLight.Specular.a = 1.0f;

    // setup directional back light
	ZeroMemory( &backLight, sizeof(D3DLIGHT9) );
	backLight.Type = D3DLIGHT_DIRECTIONAL;
    backLight.Direction = D3DXVECTOR3( 1.0f,  0.0f,  0.0f );
    backLight.Diffuse.r = 0.0f;
    backLight.Diffuse.g = 0.2f;
    backLight.Diffuse.b = 0.5f;
    backLight.Diffuse.a = 1.0f;
    backLight.Specular.r = 0.3f;
    backLight.Specular.g = 0.3f;
    backLight.Specular.b = 0.3f;
    backLight.Specular.a = 1.0f;

	// ok
	return true;

}




// ---------- framework : setup dx ----------

/*!

\brief framework : setup dx
\author Gareth Edwards

\param LPDIRECT3DDEVICE9	(device)

\return bool (TRUE if ok)

*/

bool rtvsD3dApp::setupDX (LPDIRECT3DDEVICE9 pd3dDevice)
{

	// ---- turn lighting ON ----
	pd3dDevice->SetRenderState( D3DRS_LIGHTING , TRUE);


	// ---- ambient light ----
	pd3dDevice->SetRenderState( D3DRS_AMBIENT,D3DCOLOR_COLORVALUE( 0.3, 0.3, 0.3, 1.0));


	// ---- sun light ----
    pd3dDevice->SetLight( 0, &sunLight );
    pd3dDevice->LightEnable( 0, TRUE );


	// ---- back light ----
	pd3dDevice->SetLight( 1, &backLight );
    pd3dDevice->LightEnable( 1, TRUE );


    // ---- create a texture object ----
    D3DXCreateTextureFromFile( pd3dDevice, "image/baboon.jpg", &pTexture );

 	// ---- set texture sampling states ----
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );


	// ---- FONT ----

	fontCol = D3DCOLOR_COLORVALUE(1,1,1,1);
	D3DXCreateFont(
		pd3dDevice,
		30,								// height in pixels
		0,								// width in pixels (0 for default)
		400,							// thickness, 0-1000 OR FW_THIN (100), FW_NORMAL (400), FW_BOLD (700), FW_HEAVY (900)
		0,								// number of MipMaps to create. 0 creates a full chain - no scaling use 1
		false,							// 0/1 - true/false, do you want Italics
		DEFAULT_CHARSET,				// character Set - (Arabic, Greek, etc)
		OUT_DEFAULT_PRECIS,				// how precisely the output must match the font
		ANTIALIASED_QUALITY,			// ANTIALIASED_QUALITY, DEFAULT_QUALITY, DRAFT_QUALITY, and PROOF_QUALITY
		DEFAULT_PITCH | FF_DONTCARE,	// font pitch 
		"Arial",						// name of the required font or "" for system best match
		&pFont);


	// ---- AXIS ----


	// ---- initialise axis vertex data ----
 	LineVertex axisVertices[] =
	{
		//    x      y      z
		{  0.0f,  0.0f,  0.0f  },
		{  1.0f,  0.0f,  0.0f  },
		{  0.9f,  0.0f,  0.1f  },
		{  1.0f,  0.0f,  0.0f  },
		{  0.9f,  0.0f, -0.1f  },
		{  1.0f,  0.0f,  0.0f  },

		{  0.0f,  0.0f,  0.0f  },
		{  0.0f,  1.0f,  0.0f  },
		{ -0.1f,  0.9f,  0.0f  },
		{  0.0f,  1.0f,  0.0f  },
		{  0.1f,  0.9f,  0.0f  },
		{  0.0f,  1.0f,  0.0f  },

		{  0.0f,  0.0f,  0.0f  },
		{  0.0f,  0.0f,  1.0f  },
		{ -0.1f,  0.0f,  0.9f  },
		{  0.0f,  0.0f,  1.0f  },
		{  0.1f,  0.0f,  0.9f  },
		{  0.0f,  0.0f,  1.0f  },
	};

	// ---- create axis vertex buffer ----
	int numAxisVertices = sizeof(axisVertices) / ( sizeof(float) * 3 );
	numAxisLines = numAxisVertices / 2;
	pd3dDevice->CreateVertexBuffer( numAxisVertices*sizeof(LineVertex),
                                      D3DUSAGE_WRITEONLY,
                                      LineVertex::FVF_Flags,
                                      //D3DPOOL_MANAGED, // does not have to be properly Released before calling IDirect3DDevice9::Reset
                                      D3DPOOL_DEFAULT,   // must be Released properly before calling IDirect3DDevice9::Reset
                                      &pAxisVertexBuffer, NULL );


	// ---- scale and add offset to axis ----
	float aScale  = 5;
	float yOffset = 0.02f;
 	LineVertex *pAxisVertices = axisVertices;
	for (int av=0; av<numAxisVertices; av++)
	{
		pAxisVertices->x *= aScale;
		pAxisVertices->y  = pAxisVertices->y * aScale + yOffset;
		pAxisVertices->z *= aScale;
		pAxisVertices++;
	}

	// ---- block copy into axis vertex buffer ----
	void *pVertices = NULL;
	pAxisVertexBuffer->Lock( 0, sizeof(axisVertices), (void**)&pVertices, 0 );
	memcpy( pVertices, axisVertices, sizeof(axisVertices) );
	pAxisVertexBuffer->Unlock();


	// ---- QUAD ----


    // ---- initialise quad vertex data ----
 	QuadVertex quadVertices[] =
	{
		//    x      y      z     nx     ny     nz     d,   tu     tv
		{  5.0f,  0.0f,  5.0f,  0.0f,  1.0f,  0.0f,    0,  0.0f,  0.0f },
		{ -5.0f,  0.0f,  5.0f,  0.0f,  1.0f,  0.0f,    0,  1.0f,  0.0f },
		{  5.0f,  0.0f, -5.0f,  0.0f,  1.0f,  0.0f,    0,  0.0f,  1.0f },
		{ -5.0f,  0.0f, -5.0f,  0.0f,  1.0f,  0.0f,    0,  1.0f,  1.0f }
	};

	// ---- create quad vertex buffer ----
	int numQuadVertices = sizeof(quadVertices) / ( sizeof(float) * 8 +  sizeof(DWORD));
	numQuadTriangles = numQuadVertices / 2;
	pd3dDevice->CreateVertexBuffer( numQuadVertices*sizeof(QuadVertex),
                                      D3DUSAGE_WRITEONLY,
                                      QuadVertex::FVF_Flags,
                                      //D3DPOOL_MANAGED, // does not have to be properly Released before calling IDirect3DDevice9::Reset
                                      D3DPOOL_DEFAULT,   // must be Released properly before calling IDirect3DDevice9::Reset
                                      &pQuadVertexBuffer, NULL );


	// ---- block copy into quad vertex buffer ----
	pVertices = NULL;
	pQuadVertexBuffer->Lock( 0, sizeof(quadVertices), (void**)&pVertices, 0 );
	memcpy( pVertices, quadVertices, sizeof(quadVertices) );
	pQuadVertexBuffer->Unlock();


	// ---- VECTORS ----


	// ---- initialise vector vertex data ----
 	LineVertex vectVertices[] =
	{
		//    x      y      z
		{  0.0f,  0.0f,  0.0f  },
		{  1.0f,  0.0f,  0.0f  },
		{  0.0f,  0.0f,  0.0f  },
		{  1.0f,  0.0f,  0.0f  },
		{  0.0f,  0.0f,  0.0f  },
		{  1.0f,  0.0f,  0.0f  }
	};

	// ---- create vector vertex buffer ----
	int numVectVertices = sizeof(vectVertices) / ( sizeof(float) * 3 );
	numVectLines = numVectVertices / 2;
	pd3dDevice->CreateVertexBuffer( numVectVertices*sizeof(LineVertex),
                                      D3DUSAGE_WRITEONLY,
                                      LineVertex::FVF_Flags,
                                      //D3DPOOL_MANAGED, // does not have to be properly Released before calling IDirect3DDevice9::Reset
                                      D3DPOOL_DEFAULT,   // must be Released properly before calling IDirect3DDevice9::Reset
                                      &pVectVertexBuffer, NULL );

	// ---- block copy into vector vertex buffer ----
	pVertices = NULL;
	pVectVertexBuffer->Lock( 0, sizeof(vectVertices), (void**)&pVertices, 0 );
	memcpy( pVertices, vectVertices, sizeof(vectVertices) );
	pVectVertexBuffer->Unlock();


	// ok
	return true;

}




////////////////////////////////////////////////////////////////////////////////




// ---- vector demos ----

#define VLEN 4

DWORD rtvsD3dApp::vectorAdd()
{
	Vector3D o( 0, 0, 0);
	Vector3D a( VLEN, VLEN, VLEN);
	Vector3D b( VLEN, VLEN,-VLEN);
	Vector3D c = a + b;
	vectorLine(0, o, a);
	vectorLine(1, o, b);
	vectorLine(2, o, c);
	return 3;
}


DWORD rtvsD3dApp::vectorSubtract()
{
	Vector3D o( 0, 0, 0);
	Vector3D a( VLEN, VLEN, VLEN);
	Vector3D b( VLEN, VLEN,-VLEN);
	Vector3D c = a - b;
	vectorLine(0, o, a);
	vectorLine(1, o, b);
	vectorLine(2, o, c);
	return 3;
}


DWORD rtvsD3dApp::vectorMultiply()
{
	Vector3D o( 0, 0, 0);
	Vector3D a( VLEN, VLEN, 0);
	Vector3D b = a * 2;
	vectorLine(0, o, b);
	vectorLine(1, o, a);
	return 2;
}


DWORD rtvsD3dApp::vectorDivide()
{
	Vector3D o( 0, 0, 0);
	Vector3D a( VLEN, VLEN, 0);
	Vector3D b = a / 2;
	vectorLine(0, o, a);
	vectorLine(1, o, b);
	return 2;
}


DWORD rtvsD3dApp::vectorNegate()
{
	Vector3D o( 0, 0, 0);
	Vector3D a( VLEN, VLEN, 0);
	Vector3D b( VLEN, VLEN, 0);
	b.negate();
	vectorLine(0, o, a);
	vectorLine(1, o, b);
	return 2;
}


DWORD rtvsD3dApp::vectorNormalise()
{
	Vector3D o( 0, 0, 0);
	Vector3D a( VLEN, VLEN, 0);
	Vector3D b( VLEN, VLEN, 0);
	b.normalize();
	vectorLine(0, o, a);
	vectorLine(1, o, b);
	return 2;
}


DWORD rtvsD3dApp::vectorCross()
{
	Vector3D o( 0, 0,-0);
	Vector3D a( VLEN, VLEN, VLEN);
	Vector3D b( VLEN, VLEN,-VLEN);
	Vector3D c( VLEN, VLEN,-VLEN);
	c.crossProduct(a);
	vectorLine(0, o, a);
	vectorLine(1, o, b);
	vectorLine(2, o, c);
	return 3;
}


DWORD rtvsD3dApp::vectorDot()
{
	Vector3D o( 0, 0,-0);
	Vector3D a( VLEN, VLEN, VLEN);
	Vector3D b( VLEN, VLEN,-VLEN);
	Vector3D c;

	double d = b.dotProduct(a);

	c.x = (a.x + b.x) / 2;
	c.y = (a.y + b.y) / 2;
	c.z = (a.z + b.z) / 2;

	double l =c.length();
	c.x *= d/l;
	c.y *= d/l;
	c.z *= d/l;

	vectorLine(0, o, a);
	vectorLine(1, o, b);
	vectorLine(2, o, c);
	return 3;
}


DWORD rtvsD3dApp::vectorLine(int lineIndex, Vector3D s, Vector3D e)
{
	// catch illegal line index
	if (lineIndex < 0 || lineIndex > 2)
		return 0;

	// ---- update vector vertex buffer ----
	LineVertex *pVertices = NULL;
	pVectVertexBuffer->Lock( 0, sizeof(LineVertex) * 6, (void**)&pVertices, 0 );
	pVertices += lineIndex*2;
	pVertices->x = (float)s.x;
	pVertices->y = (float)s.y;
	pVertices->z = (float)s.z;
	pVertices++;
	pVertices->x = (float)e.x;
	pVertices->y = (float)e.y;
	pVertices->z = (float)e.z;
	pVectVertexBuffer->Unlock();

	// ok
	return 1;
}



// ---------- update keyboard ----------

/*!

\brief update keyboard
\author Gareth Edwards

\param rtvsD3dSdk::stage* (pointer to RTVS Stage)

\return bool (TRUE if key updated)

*/

bool rtvsD3dApp::updateKeyboard ()
{

	// get key clicked
	if(GetAsyncKeyState('1') & 0x8000f)
		currentKeyClicked = 1;
	else if(GetAsyncKeyState('2') & 0x8000f)
		currentKeyClicked = 2;
	else if(GetAsyncKeyState('3') & 0x8000f)
		currentKeyClicked = 3;
	else if(GetAsyncKeyState('4') & 0x8000f)
		currentKeyClicked = 4;
	else if(GetAsyncKeyState('5') & 0x8000f)
		currentKeyClicked = 5;
	else if(GetAsyncKeyState('6') & 0x8000f)
		currentKeyClicked = 6;
	else if(GetAsyncKeyState('7') & 0x8000f)
		currentKeyClicked = 7;
	else if(GetAsyncKeyState('8') & 0x8000f)
		currentKeyClicked = 8;

	// ok
	return true;
}


	
	
////////////////////////////////////////////////////////////////////////////////




// ---------- getSpin ----------

/*!

\brief get x and y spin 
\author Gareth Edwards
\param float* ([out] spin around x axis)
\param float* ([out] spin around y axis)

*/

void rtvsD3dApp::getSpin (float* spinX, float* spinY)
{
	*spinX = fSpinX;
	*spinY = fSpinY;
}




// ---------- setSpin ----------

/*!

\brief set x and y spin 
\author Gareth Edwards
\param float (spin around x axis)
\param float (spin around y axis)

*/

void rtvsD3dApp::setSpin (float spinX, float spinY)
{
	fSpinX = spinX;
	fSpinY = spinY;
}




////////////////////////////////////////////////////////////////////////////////
