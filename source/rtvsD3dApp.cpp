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
#include <string>
using namespace std;
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>




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
	currentKeyClicked = 9;

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
	 if( pVertexBuffer != NULL )
    {
        int nNewRefCount = pVertexBuffer->Release();

        if( nNewRefCount > 0 )
        {
            static char strError[256];
            sprintf_s ( strError, 256,
				"The line vertex buffer object failed to cleanup properly.\n"
                "Release() returned a reference count of %d",
				nNewRefCount );
            MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
        }
        pVertexBuffer = NULL;
    }

	  if( pMesh != NULL )
    {
        int nNewRefCount = pMesh->Release();

        if( nNewRefCount > 0 )
        {
            static char strError[256];
            sprintf_s ( strError, 256,
				"The line vertex buffer object failed to cleanup properly.\n"
                "Release() returned a reference count of %d",
				nNewRefCount );
            MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
        }
        pMesh = NULL;
    }
	 
	  if( waveTexture != NULL )
    {
        int nNewRefCount = waveTexture->Release();

        if( nNewRefCount > 0 )
        {
            static char strError[256];
            sprintf_s ( strError, 256,
				"The line vertex buffer object failed to cleanup properly.\n"
                "Release() returned a reference count of %d",
				nNewRefCount );
            MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
        }
        waveTexture = NULL;
    }

	TwTerminate();//cleanup anttweakbar

	// ok
	return true;

}

void TW_CALL wfButton(void *clientData) //wireframe button for AntTweak Bar
	{ 
    bool* wireframe = static_cast<bool*> (clientData);
	*wireframe = !*wireframe;
	}
void TW_CALL saveButton(void *clientData)
{
	rtvsD3dApp* applicationPointer = (rtvsD3dApp*)clientData;
	applicationPointer->saveFile();
}
void TW_CALL homeButton(void *clientData)
{
	rtvsD3dApp* homePointer = (rtvsD3dApp*)clientData;
	homePointer->currentKeyClicked = 9;
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
	//check keyboard input
	updateKeyboard();
	for(int i=0; i<9; i++){
		if(i == currentKeyClicked){
			waveEnabled[i] = true;
		} else {
			waveEnabled[i] = false;
		}
	}
	if(currentKeyClicked == 9){
		for(int i=0; i<9; i++){
			waveEnabled[i] = true;
		}
	}

 	// clear backbuffers
    pd3dDevice->Clear( 0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f),
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
	D3DXMatrixTranslation( &matTranslation, 0, -5, 40 );
	matWorld = matRotation * matTranslation;
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	
	// mesh
	

	if (wireframe == true) {
	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	} else { 
		
			pd3dDevice->SetMaterial( &quadMtrl );
			pd3dDevice->SetTexture( 0, waveTexture );
			pd3dDevice->SetFVF( QuadVertex::FVF_Flags );
			pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	
	frame++;
	meshUpdateY(frame);							//frame increments thus the function does
	pMesh->DrawSubset(0); //draw mesh

	TwDraw();  // draw the tweak bar(s)

	/*for (int i = 0; i < faces; i++)
		meshNormal(vertices */


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

	/*sineWaveParameters[0] = 0.0;
	sineWaveParameters[1] = 0.0;
	sineWaveParameters[2] = 1.0;
	sineWaveParameters[3] = 90.0;
	sineWaveParameters[4] = 0.5;
	sineWaveParameters[5] = 0.5;
	sineWaveParameters[6] = 0.0;*/
	
	frame = 0;
	wireframe = false;
	
	
	//File Manager
	int currentWave;

	string wav_line1_id;
	string wav_line2_emX;
	string wav_line3_emZ;
	string wav_line4_amp;
	string wav_line5_prd;
	string wav_line6_phS;
	string wav_line7_phI;
	string wav_line8_Yoff;

	for(int i=0; i<9; i++){
		waveEnabled[i] = false;
	}
	waveEnabled[9] = true;

	// Collects all the data from the config files

	ifstream ifile;

			//Collects all the data from the config files

			ifile.open("config/wave1.txt");

			getline(ifile, wav_line1_id);
			getline(ifile, wav_line2_emX);
			getline(ifile, wav_line3_emZ);
			getline(ifile, wav_line4_amp);
			getline(ifile, wav_line5_prd);
			getline(ifile, wav_line6_phS);
			getline(ifile, wav_line7_phI);
			getline(ifile, wav_line8_Yoff);

			wave[1].id   = ::atoi(wav_line1_id.c_str());
			wave[1].emX  = ::atof(wav_line2_emX.c_str());
			wave[1].emZ  = ::atof(wav_line3_emZ.c_str());
			wave[1].amp  = ::atof(wav_line4_amp.c_str());
			wave[1].prd  = ::atof(wav_line5_prd.c_str());
			wave[1].phS  = ::atof(wav_line6_phS.c_str());
			wave[1].phI  = ::atof(wav_line7_phI.c_str());
			wave[1].Yoff = ::atof(wav_line8_Yoff.c_str());
			
			ifile.close();
			ifile.open("config/wave2.txt");

			getline(ifile, wav_line1_id);
			getline(ifile, wav_line2_emX);
			getline(ifile, wav_line3_emZ);
			getline(ifile, wav_line4_amp);
			getline(ifile, wav_line5_prd);
			getline(ifile, wav_line6_phS);
			getline(ifile, wav_line7_phI);
			getline(ifile, wav_line8_Yoff);

			wave[2].id   = ::atoi(wav_line1_id.c_str());
			wave[2].emX  = ::atof(wav_line2_emX.c_str());
			wave[2].emZ  = ::atof(wav_line3_emZ.c_str());
			wave[2].amp  = ::atof(wav_line4_amp.c_str());
			wave[2].prd  = ::atof(wav_line5_prd.c_str());
			wave[2].phS  = ::atof(wav_line6_phS.c_str());
			wave[2].phI  = ::atof(wav_line7_phI.c_str());
			wave[2].Yoff = ::atof(wav_line8_Yoff.c_str());
			
			ifile.close();
			ifile.open("config/wave3.txt");

			getline(ifile, wav_line1_id);
			getline(ifile, wav_line2_emX);
			getline(ifile, wav_line3_emZ);
			getline(ifile, wav_line4_amp);
			getline(ifile, wav_line5_prd);
			getline(ifile, wav_line6_phS);
			getline(ifile, wav_line7_phI);
			getline(ifile, wav_line8_Yoff);

			wave[3].id   = ::atoi(wav_line1_id.c_str());
			wave[3].emX  = ::atof(wav_line2_emX.c_str());
			wave[3].emZ  = ::atof(wav_line3_emZ.c_str());
			wave[3].amp  = ::atof(wav_line4_amp.c_str());
			wave[3].prd  = ::atof(wav_line5_prd.c_str());
			wave[3].phS  = ::atof(wav_line6_phS.c_str());
			wave[3].phI  = ::atof(wav_line7_phI.c_str());
			wave[3].Yoff = ::atof(wav_line8_Yoff.c_str());

			ifile.close();
			ifile.open("config/wave4.txt");

			getline(ifile, wav_line1_id);
			getline(ifile, wav_line2_emX);
			getline(ifile, wav_line3_emZ);
			getline(ifile, wav_line4_amp);
			getline(ifile, wav_line5_prd);
			getline(ifile, wav_line6_phS);
			getline(ifile, wav_line7_phI);
			getline(ifile, wav_line8_Yoff);

			wave[4].id   = ::atoi(wav_line1_id.c_str());
			wave[4].emX  = ::atof(wav_line2_emX.c_str());
			wave[4].emZ  = ::atof(wav_line3_emZ.c_str());
			wave[4].amp  = ::atof(wav_line4_amp.c_str());
			wave[4].prd  = ::atof(wav_line5_prd.c_str());
			wave[4].phS  = ::atof(wav_line6_phS.c_str());
			wave[4].phI  = ::atof(wav_line7_phI.c_str());
			wave[4].Yoff = ::atof(wav_line8_Yoff.c_str());
			
			ifile.close();
			ifile.open("config/wave5.txt");

			getline(ifile, wav_line1_id);
			getline(ifile, wav_line2_emX);
			getline(ifile, wav_line3_emZ);
			getline(ifile, wav_line4_amp);
			getline(ifile, wav_line5_prd);
			getline(ifile, wav_line6_phS);
			getline(ifile, wav_line7_phI);
			getline(ifile, wav_line8_Yoff);

			wave[5].id   = ::atoi(wav_line1_id.c_str());
			wave[5].emX  = ::atof(wav_line2_emX.c_str());
			wave[5].emZ  = ::atof(wav_line3_emZ.c_str());
			wave[5].amp  = ::atof(wav_line4_amp.c_str());
			wave[5].prd  = ::atof(wav_line5_prd.c_str());
			wave[5].phS  = ::atof(wav_line6_phS.c_str());
			wave[5].phI  = ::atof(wav_line7_phI.c_str());
			wave[5].Yoff = ::atof(wav_line8_Yoff.c_str());

			ifile.close();
			ifile.open("config/wave6.txt");

			getline(ifile, wav_line1_id);
			getline(ifile, wav_line2_emX);
			getline(ifile, wav_line3_emZ);
			getline(ifile, wav_line4_amp);
			getline(ifile, wav_line5_prd);
			getline(ifile, wav_line6_phS);
			getline(ifile, wav_line7_phI);
			getline(ifile, wav_line8_Yoff);

			wave[6].id   = ::atoi(wav_line1_id.c_str());
			wave[6].emX  = ::atof(wav_line2_emX.c_str());
			wave[6].emZ  = ::atof(wav_line3_emZ.c_str());
			wave[6].amp  = ::atof(wav_line4_amp.c_str());
			wave[6].prd  = ::atof(wav_line5_prd.c_str());
			wave[6].phS  = ::atof(wav_line6_phS.c_str());
			wave[6].phI  = ::atof(wav_line7_phI.c_str());
			wave[6].Yoff = ::atof(wav_line8_Yoff.c_str());

			ifile.close();
			ifile.open("config/wave7.txt");

			getline(ifile, wav_line1_id);
			getline(ifile, wav_line2_emX);
			getline(ifile, wav_line3_emZ);
			getline(ifile, wav_line4_amp);
			getline(ifile, wav_line5_prd);
			getline(ifile, wav_line6_phS);
			getline(ifile, wav_line7_phI);
			getline(ifile, wav_line8_Yoff);

			wave[7].id   = ::atoi(wav_line1_id.c_str());
			wave[7].emX  = ::atof(wav_line2_emX.c_str());
			wave[7].emZ  = ::atof(wav_line3_emZ.c_str());
			wave[7].amp  = ::atof(wav_line4_amp.c_str());
			wave[7].prd  = ::atof(wav_line5_prd.c_str());
			wave[7].phS  = ::atof(wav_line6_phS.c_str());
			wave[7].phI  = ::atof(wav_line7_phI.c_str());
			wave[7].Yoff = ::atof(wav_line8_Yoff.c_str());
			
			ifile.close();
			ifile.open("config/wave8.txt");

			getline(ifile, wav_line1_id);
			getline(ifile, wav_line2_emX);
			getline(ifile, wav_line3_emZ);
			getline(ifile, wav_line4_amp);
			getline(ifile, wav_line5_prd);
			getline(ifile, wav_line6_phS);
			getline(ifile, wav_line7_phI);
			getline(ifile, wav_line8_Yoff);

			wave[8].id   = ::atoi(wav_line1_id.c_str());
			wave[8].emX  = ::atof(wav_line2_emX.c_str());
			wave[8].emZ  = ::atof(wav_line3_emZ.c_str());
			wave[8].amp  = ::atof(wav_line4_amp.c_str());
			wave[8].prd  = ::atof(wav_line5_prd.c_str());
			wave[8].phS  = ::atof(wav_line6_phS.c_str());
			wave[8].phI  = ::atof(wav_line7_phI.c_str());
			wave[8].Yoff = ::atof(wav_line8_Yoff.c_str());

			ifile.close();


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

	// ---- Setup the font ----
	fontCol = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCreateFont(
		pd3dDevice,
		17,								// height in pixels
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

	// ---- turn lighting ON ----

	pd3dDevice->SetRenderState( D3DRS_LIGHTING , TRUE);


	// ---- ambient light ----

	pd3dDevice->SetRenderState( D3DRS_AMBIENT,D3DCOLOR_COLORVALUE( 0.3, 0.3, 0.3, 1.0));


	// ---- sun light ----

    pd3dDevice->SetLight( 0, &sunLight );
    pd3dDevice->LightEnable( 0, TRUE );


	// ---- back light ----

	pd3dDevice->SetLight( 1, &surfaceLight );
  pd3dDevice->LightEnable( 1, TRUE );


    // ---- create a texture object ----

    D3DXCreateTextureFromFile( pd3dDevice, "image/wave.jpg", &waveTexture );

    pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

    // ---- create a vertex buffer ----

 	QuadVertex quadVertices[] =
	{
		//    x      y      z     nx     ny     nz     d,   tu     tv
		{  5.0f,  0.0f, -5.0f,  0.0f,  1.0f,  0.0f,    0,  0.0f,  0.0f },
		{  5.0f,  0.0f,  5.0f,  0.0f,  1.0f,  0.0f,    0,  1.0f,  0.0f },
		{ -5.0f,  0.0f, -5.0f,  0.0f,  1.0f,  0.0f,    0,  0.0f,  1.0f },
		{ -5.0f,  0.0f,  5.0f,  0.0f,  1.0f,  0.0f,    0,  1.0f,  1.0f }
	};

	pd3dDevice->CreateVertexBuffer( 4*sizeof(QuadVertex),
                                      D3DUSAGE_WRITEONLY,
                                      QuadVertex::FVF_Flags,
                                      //D3DPOOL_MANAGED, // does not have to be properly Released before calling IDirect3DDevice9::Reset
                                      D3DPOOL_DEFAULT,   // must be Released properly before calling IDirect3DDevice9::Reset
                                      &pVertexBuffer, NULL );

	void *pVertices = NULL;

	pVertexBuffer->Lock( 0, sizeof(quadVertices), (void**)&pVertices, 0 );
	memcpy( pVertices, quadVertices, sizeof(quadVertices) );
	pVertexBuffer->Unlock();


	// ---- Create a plane ----

	meshParams[0] = -15;
	meshParams[1] = -15;
	meshParams[2] = 15;
	meshParams[3] = 15;
	meshParams[4] = 0;
	meshParams[5] = 0;
	meshParams[6] = 1;
	meshParams[7] = 1;

	meshVertsWidth = 100;
	meshVertsDepth = 100;

	plane(pd3dDevice, meshVertsWidth, meshVertsDepth, meshParams); //the mesh


	//anttweakbar
	TwInit(TW_DIRECT3D9, pd3dDevice); // for Direct3D 9
	TwWindowSize(1200, 800);
	
		TwBar *myBar;
	myBar = TwNewBar("Ocean Wave Settings");

	TwAddButton(myBar, "Home", homeButton, this, " label='Home-(NumKey9)' ");
	TwAddButton(myBar, "Mode", wfButton, &wireframe, " label='Wireframe' ");
	TwAddButton(myBar, "SaveF", saveButton, this, " label='Save All' ");

	TwAddVarRW(myBar, "wave1emx", TW_TYPE_FLOAT, &wave[1].emX, " min=-25 max=25 step=0.02 group=Wave-1-(NumKey1) label='Emmiter X' ");
	TwAddVarRW(myBar, "wave1emz", TW_TYPE_FLOAT, &wave[1].emZ, " min=-25 max=25 step=0.02 group=Wave-1-(NumKey1) label='Emmiter Z' ");
	TwAddVarRW(myBar, "wave1amp",TW_TYPE_FLOAT, &wave[1].amp, " min=-3 max=3 step=0.02 group=Wave-1-(NumKey1) label='Amplitude' ");
	TwAddVarRW(myBar, "wave1prd", TW_TYPE_FLOAT, &wave[1].prd, " min=0 max=100 step=0.05 group=Wave-1-(NumKey1) label='Period' ");
	TwAddVarRW(myBar, "wave1phs", TW_TYPE_FLOAT, &wave[1].phS, " min=-5 max=5 step=0.01 group=Wave-1-(NumKey1) label='Phase Shift' ");
	TwAddVarRW(myBar, "wave1phi", TW_TYPE_FLOAT, &wave[1].phI, " min=-360 max=360 step=0.1 group=Wave-1-(NumKey1) label='Phase Incr' ");
	TwAddVarRW(myBar, "wave1yoff", TW_TYPE_FLOAT, &wave[1].Yoff, " min=-1.5 max=1.5 step=0.02 group=Wave-1-(NumKey1) label='Y Offset' ");

	TwAddVarRW(myBar, "wave2emx", TW_TYPE_FLOAT, &wave[2].emX, " min=-25 max=25 step=0.02 group=Wave-2-(NumKey2) label='Emmiter X' ");
	TwAddVarRW(myBar, "wave2emz", TW_TYPE_FLOAT, &wave[2].emZ, " min=-25 max=25 step=0.02 group=Wave-2-(NumKey2) label='Emmiter Z' ");
	TwAddVarRW(myBar, "wave2amp", TW_TYPE_FLOAT, &wave[2].amp, " min=-3 max=3 step=0.02 group=Wave-2-(NumKey2) label='Amplitude' ");
	TwAddVarRW(myBar, "wave2prd", TW_TYPE_FLOAT, &wave[2].prd, " min=0 max=100 step=0.05 group=Wave-2-(NumKey2) label='Period' ");
	TwAddVarRW(myBar, "wave2phs", TW_TYPE_FLOAT, &wave[2].phS, " min=-5 max=5 step=0.01 group=Wave-2-(NumKey2) label='Phase Shift' ");
	TwAddVarRW(myBar, "wave2psi", TW_TYPE_FLOAT, &wave[2].phI, " min=-360 max=360 step=0.1 group=Wave-2-(NumKey2) label='Phase Incr' ");
	TwAddVarRW(myBar, "wave2yoff", TW_TYPE_FLOAT, &wave[2].Yoff, " min=-1.5 max=1.5 step=0.02 group=Wave-2-(NumKey2) label='Y Offset' ");

	TwAddVarRW(myBar, "wave3emx", TW_TYPE_FLOAT, &wave[3].emX, " min=-25 max=25 step=0.02 group=Wave-3-(NumKey3) label='Emmiter X' ");
	TwAddVarRW(myBar, "wave3emz", TW_TYPE_FLOAT, &wave[3].emZ, " min=-25 max=25 step=0.02 group=Wave-3-(NumKey3) label='Emmiter Z' ");
	TwAddVarRW(myBar, "wave3amp", TW_TYPE_FLOAT, &wave[3].amp, " min=-3 max=3 step=0.02 group=Wave-3-(NumKey3) label='Amplitude' ");
	TwAddVarRW(myBar, "wave3prd", TW_TYPE_FLOAT, &wave[3].prd, " min=0 max=100 step=0.05 group=Wave-3-(NumKey3) label='Period' ");
	TwAddVarRW(myBar, "wave3phs", TW_TYPE_FLOAT, &wave[3].phS, " min=-5 max=5 step=0.01 group=Wave-3-(NumKey3) label='Phase Shift' ");
	TwAddVarRW(myBar, "wave3phi", TW_TYPE_FLOAT, &wave[3].phI, " min=-360 max=360 step=0.1 group=Wave-3-(NumKey3) label='Phase Incr' ");
	TwAddVarRW(myBar, "wave3yoff", TW_TYPE_FLOAT, &wave[3].Yoff, " min=-1.5 max=1.5 step=0.02 group=Wave-3-(NumKey3) label='Y Offset' ");

	TwAddVarRW(myBar, "wave4emx", TW_TYPE_FLOAT, &wave[4].emX, " min=-25 max=25 step=0.02 group=Wave-4-(NumKey4) label='Emmiter X' ");
	TwAddVarRW(myBar, "wave4emz", TW_TYPE_FLOAT, &wave[4].emZ, " min=-25 max=25 step=0.02 group=Wave-4-(NumKey4) label='Emmiter Z' ");
	TwAddVarRW(myBar, "wave4amp", TW_TYPE_FLOAT, &wave[4].amp, " min=-3 max=3 step=0.02 group=Wave-4-(NumKey4) label='Amplitude' ");
	TwAddVarRW(myBar, "wave4prd", TW_TYPE_FLOAT, &wave[4].prd, " min=0 max=100 step=0.05 group=Wave-4-(NumKey4) label='Period' ");
	TwAddVarRW(myBar, "wave4phs", TW_TYPE_FLOAT, &wave[4].phS, " min=-5 max=5 step=0.01 group=Wave-4-(NumKey4) label='Phase Shift' ");
	TwAddVarRW(myBar, "wave4phi", TW_TYPE_FLOAT, &wave[4].phI, " min=-360 max=360 step=0.1 group=Wave-4-(NumKey4) label='Phase Incr' ");
	TwAddVarRW(myBar, "wave4yoff", TW_TYPE_FLOAT, &wave[4].Yoff, " min=-1.5 max=1.5 step=0.02 group=Wave-4-(NumKey4) label='Y Offset' ");

	TwAddVarRW(myBar, "wave5emx", TW_TYPE_FLOAT, &wave[5].emX, " min=-25 max=25 step=0.02 group=Wave-5-(NumKey5) label='Emmiter X' ");
	TwAddVarRW(myBar, "wave5emz", TW_TYPE_FLOAT, &wave[5].emZ, " min=-25 max=25 step=0.02 group=Wave-5-(NumKey5) label='Emmiter Z' ");
	TwAddVarRW(myBar, "wave5amp", TW_TYPE_FLOAT, &wave[5].amp, " min=-3 max=3 step=0.02 group=Wave-5-(NumKey5) label='Amplitude' ");
	TwAddVarRW(myBar, "wave5prd", TW_TYPE_FLOAT, &wave[5].prd, " min=0 max=100 step=0.05 group=Wave-5-(NumKey5) label='Period' ");
	TwAddVarRW(myBar, "wave5phs", TW_TYPE_FLOAT, &wave[5].phS, " min=-5 max=5 step=0.01 group=Wave-5-(NumKey5) label='Phase Shift' ");
	TwAddVarRW(myBar, "wave5phi", TW_TYPE_FLOAT, &wave[5].phI, " min=-360 max=360 step=0.1 group=Wave-5-(NumKey5) label='Phase Incr' ");
	TwAddVarRW(myBar, "wave5yoff", TW_TYPE_FLOAT, &wave[5].Yoff, " min=-1.5 max=1.5 step=0.02 group=Wave-5-(NumKey5) label='Y Offset' ");

	TwAddVarRW(myBar, "wave6emx", TW_TYPE_FLOAT, &wave[6].emX, " min=-25 max=25 step=0.02 group=Wave-6-(NumKey6) label='Emmiter X' ");
	TwAddVarRW(myBar, "wave6emz", TW_TYPE_FLOAT, &wave[6].emZ, " min=-25 max=25 step=0.02 group=Wave-6-(NumKey6) label='Emmiter Z' ");
	TwAddVarRW(myBar, "wave6amp", TW_TYPE_FLOAT, &wave[6].amp, " min=-3 max=3 step=0.02 group=Wave-6-(NumKey6) label='Amplitude' ");
	TwAddVarRW(myBar, "wave6prd", TW_TYPE_FLOAT, &wave[6].prd, " min=0 max=100 step=0.05 group=Wave-6-(NumKey6) label='Period' ");
	TwAddVarRW(myBar, "wave6phs", TW_TYPE_FLOAT, &wave[6].phS, " min=-5 max=5 step=0.01 group=Wave-6-(NumKey6) label='Phase Shift' ");
	TwAddVarRW(myBar, "wave6phi", TW_TYPE_FLOAT, &wave[6].phI, " min=-360 max=360 step=0.1 group=Wave-6-(NumKey6) label='Phase Incr' ");
	TwAddVarRW(myBar, "wave6yoff", TW_TYPE_FLOAT, &wave[6].Yoff, " min=-1.5 max=1.5 step=0.02 group=Wave-6-(NumKey6) label='Y Offset' ");

	TwAddVarRW(myBar, "wave7emx", TW_TYPE_FLOAT, &wave[7].emX, " min=-25 max=25 step=0.02 group=Wave-7-(NumKey7) label='Emmiter X' ");
	TwAddVarRW(myBar, "wave7emz", TW_TYPE_FLOAT, &wave[7].emZ, " min=-25 max=25 step=0.02 group=Wave-7-(NumKey7) label='Emmiter Z' ");
	TwAddVarRW(myBar, "wave7amp", TW_TYPE_FLOAT, &wave[7].amp, " min=-3 max=3 step=0.02 group=Wave-7-(NumKey7) label='Amplitude' ");
	TwAddVarRW(myBar, "wave7prd", TW_TYPE_FLOAT, &wave[7].prd, " min=0 max=100 step=0.05 group=Wave-7-(NumKey7) label='Period' ");
	TwAddVarRW(myBar, "wave7phs", TW_TYPE_FLOAT, &wave[7].phS, " min=-5 max=5 step=0.01 group=Wave-7-(NumKey7) label='Phase Shift' ");
	TwAddVarRW(myBar, "wave7phi", TW_TYPE_FLOAT, &wave[7].phI, " min=-360 max=360 step=0.1 group=Wave-7-(NumKey7) label='Phase Incr' ");
	TwAddVarRW(myBar, "wave7yoff", TW_TYPE_FLOAT, &wave[7].Yoff, " min=-1.5 max=1.5 step=0.02 group=Wave-7-(NumKey7) label='Y Offset' ");

	TwAddVarRW(myBar, "wave8emx", TW_TYPE_FLOAT, &wave[8].emX, " min=-25 max=25 step=0.02 group=Wave-8-(NumKey8) label='Emmiter X' ");
	TwAddVarRW(myBar, "wave8emz", TW_TYPE_FLOAT, &wave[8].emZ, " min=-25 max=25 step=0.02 group=Wave-8-(NumKey8) label='Emmiter Z' ");
	TwAddVarRW(myBar, "wave8amp", TW_TYPE_FLOAT, &wave[8].amp, " min=-3 max=3 step=0.02 group=Wave-8-(NumKey8) label='Amplitude' ");
	TwAddVarRW(myBar, "wave8prd", TW_TYPE_FLOAT, &wave[8].prd, " min=0 max=100 step=0.05 group=Wave-8-(NumKey8) label='Period' ");
	TwAddVarRW(myBar, "wave8phs", TW_TYPE_FLOAT, &wave[8].phS, " min=-5 max=5 step=0.01 group=Wave-8-(NumKey8) label='Phase Shift' ");
	TwAddVarRW(myBar, "wave8phi", TW_TYPE_FLOAT, &wave[8].phI, " min=-360 max=360 step=0.1 group=Wave-8-(NumKey8) label='Phase Incr' ");
	TwAddVarRW(myBar, "wave8yoff", TW_TYPE_FLOAT, &wave[8].Yoff, " min=-1.5 max=1.5 step=0.02 group=Wave-8-(NumKey8) label='Y Offset' ");
	
	


	return true;

}


// ---------- create a mesh in the xz plane ----------

/*!

\brief create a mesh in the xz plane
\author Gareth Edwards

\param LPDIRECT3DDEVICE9	(device (IDirect3DDevice9*) )
\param int					(row start in height field)
\param int					(column start in height field)
\param float*				(mesh parameters array - see below)

\return bool (TRUE if ok)

Mesh parameters array must contain eight values.

These are:

0: x minimum\n
1: z minimum\n
2: x maximum\n
3: z maximum\n
4: u minimum\n
5: v minimum\n
6: u maximum\n
7: v maximum\n


*/

bool rtvsD3dApp::plane (
			LPDIRECT3DDEVICE9	device,		// device
			int					rows,		// row start in height field
			int					cols,		// column start in height field
			float*				meshParam)   // mesh parameters

{

	// catch allowed minima
	rows = rows < 2 ? 2 : rows;
	cols = cols < 2 ? 2 : cols;

	// local mesh
	vtxCols = cols;
	vtxRows = rows;
	celCols = cols-1;
	celRows = rows-1;
	unsigned int col,row;

	// initialise mesh properties
	vertexFormat = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	vertexSize   = 0;
	vertices     = vtxRows * vtxCols;
	faces        = celCols * celRows * 2;

	// local D3DX
    HRESULT		hr;
	meshVertex*	pVertexData;
	WORD*		pwIndexData;
	DWORD*		pAttBuf;

	// create the mesh
	hr  = D3DXCreateMeshFVF(
		faces,
		vertices,
		0,
		vertexFormat,
		device,
		&pMesh);
		if (FAILED(hr))
		{
			::MessageBox(0, "Error : mesh::plane - D3DXCreateMeshFVF() - FAILED", 0, 0);
			return false;
		}

	// ---- INDEX BUFFER ----

	// lock and fill index buffer.
	hr = pMesh->LockIndexBuffer(0, (void**) &pwIndexData);

	// local
	unsigned int ro, ol, oh;
	for (row=0; row<celRows; row++)
	{
		ro = row * vtxCols;
		for (col=0; col<celCols; col++) {
			ol = ro + col;
			oh = ro + col + vtxCols;
			*(pwIndexData+0) = ol;
			*(pwIndexData+1) = ol+1;
			*(pwIndexData+2) = oh;
			*(pwIndexData+3) = oh;
			*(pwIndexData+4) = ol+1;
			*(pwIndexData+5) = oh+1;
			pwIndexData += 6;
		}
	}
	hr = pMesh->UnlockIndexBuffer();


	// ---- ATTRIBUTE BUFFER ----

	// lock and fill attribute buffer.
	hr = pMesh->LockAttributeBuffer(0, &pAttBuf);
	for(unsigned int f=0; f<faces; f++)
		pAttBuf[f]= 0;
	hr = pMesh->UnlockAttributeBuffer();


	// ---- VERTEX BUFFER ----

	// lock and fill vertex buffer.
	hr = pMesh->LockVertexBuffer(0, (void**) &pVertexData);


	// mesh parameters
	int   vindex = 0;

	// dimensions
	float xWidth = meshParam[2]-meshParam[0];
	float zDepth = meshParam[3]-meshParam[1];
	float xMin   = meshParam[0];
	float zMin   = meshParam[1];

	// mapping
	float uWidth = meshParam[6]-meshParam[4];
	float vDepth = meshParam[7]-meshParam[5];
	float uMin   = meshParam[4];
	float vMin   = meshParam[5];

	// for each row of the mesh
	for (row=0; row<=celRows; row++)
	{

		// y & v ordinates
		float rmu = float(row)/float(celRows);
		float z   = rmu * zDepth + zMin;
		float v   = (1-rmu) * vDepth + vMin;

		// for each column of the mesh
		for (col=0; col<=celCols; col++)
		{
			// vertex
			float cmu = float(col)/float(celCols);
		    pVertexData[vindex].x   = (1-cmu) * xWidth + xMin;
			pVertexData[vindex].y   = 0.0;
			pVertexData[vindex].z   = z;

			// calculate y and surface normals
			pVertexData[vindex].nx  =  0.0;
			pVertexData[vindex].ny  = 1.0;
			pVertexData[vindex].nz  =  0.0;

			// texture vertex
			pVertexData[vindex].tu  = cmu * uWidth + uMin;
			pVertexData[vindex].tv  = v;

			// increment vertex index
			vindex++;
		}
	}

	// unlock
	hr = pMesh->UnlockVertexBuffer();


	// done
    return true;
}

// ---------- update mesh in y using a sine wave ----------
/*!

\brief update mesh in y using a sine wave
\author Gareth Edwards
\param float* (sin wave parameters array - see below)

\return bool (TRUE if ok)

Sine wave parameters array must contain six values.

These are:

0: x emitter
1: z emitter
2: amplitude
3: period
4: phaseShift
5: yOffset

*/
bool rtvsD3dApp::meshUpdateY (float step)

{

    // ---- lock vertex buffer ----
    meshVertex*    pVertexData;
    HRESULT hr = pMesh->LockVertexBuffer(0, (void**) &pVertexData);

    // for each vertex
	int rowVerts = 100; //vertices per row

    for (DWORD v=0; v<vertices; v++)
    {
		pVertexData->y = 0;
		int enable = 0;
		for (int i = 0; i < 9; i++) {
			if(waveEnabled[i]){
				// wave
				float xd = pVertexData->x - wave[i].emX;
				float zd = pVertexData->z - wave[i].emZ;
				float d  = sqrt(xd*xd+zd*zd);
				pVertexData->y += wave[i].amp * sin( (wave[i].prd*d - step*wave[i].phS) * float(0.01745329252) ) + wave[i].Yoff;
				enable++;
			}
		}
		pVertexData->y /= enable;

		//normal calculation
		if (v%rowVerts==rowVerts-1){
			meshNormal(pVertexData-1, pVertexData+(rowVerts-+1), pVertexData+rowVerts, pVertexData);
		} else if (v>=vertices-vtxRows){
			meshNormal(pVertexData-1, pVertexData-(rowVerts-+1), pVertexData-rowVerts, pVertexData);
		} else {
			meshNormal(pVertexData+1, pVertexData+(rowVerts+1), pVertexData+rowVerts, pVertexData);
		}

        // next
        pVertexData++;

    }

    // ---- unlock ----
    hr = pMesh->UnlockVertexBuffer();

    // done
    return true;
}

// ---------- calculate mesh normal ----------

/*!

\brief calculate mesh normal
\author Gareth Edwards

\param meshVertex* (first vertex)
\param meshVertex* (second vertex)
\param meshVertex* (third vertex) 
\param meshVertex* (returned normal)

\return bool (TRUE if ok)

*/

void rtvsD3dApp::meshNormal (
    meshVertex* v1,    //<! first vertex
    meshVertex* v2,    //<! second vertex
    meshVertex* v3,    //<! third vertex
    meshVertex* v4)    //<! returned normal
{

    // local
    float len;
    float v1x, v1y, v1z;
    float v2x, v2y, v2z;
    float x,y,z;

    // normal
    v1x = v2->x - v1->x;
    v1y = v2->y - v1->y;
    v1z = v2->z - v1->z;

    v2x = v3->x - v1->x;
    v2y = v3->y - v1->y;
    v2z = v3->z - v1->z;

    x = v1y*v2z - v1z*v2y;
    y = v1z*v2x - v1x*v2z;
    z = v1x*v2y - v1y*v2x;

    // normalise
    len = (float) sqrt (x*x + y*y + z*z);
    v4->nx = 0;
	v4->ny = 0;
	v4->nz = 0;
	v4->nx += x / len;
    v4->ny += y / len;
    v4->nz += z / len;

}

bool rtvsD3dApp::saveFile() {

	ofstream oFile;

	oFile.open("config/wave1.txt");
	oFile <<  wave[1].id << "\n" << wave[1].emX << "\n" << wave[1].emZ << "\n" << wave[1].amp << "\n" << wave[1].prd << "\n" << wave[1].phS << "\n" << wave[1].phI << "\n" << wave[1].Yoff;
	oFile.close();
	oFile.open("config/wave2.txt");
	oFile <<  wave[2].id << "\n" << wave[2].emX << "\n" << wave[2].emZ << "\n" << wave[2].amp << "\n" << wave[2].prd << "\n" << wave[2].phS << "\n" << wave[2].phI << "\n" << wave[2].Yoff;
	oFile.close();
	oFile.open("config/wave3.txt");
	oFile <<  wave[3].id << "\n" << wave[3].emX << "\n" << wave[3].emZ << "\n" << wave[3].amp << "\n" << wave[3].prd << "\n" << wave[3].phS << "\n" << wave[3].phI << "\n" << wave[3].Yoff;
	oFile.close();
	oFile.open("config/wave4.txt");
	oFile <<  wave[4].id << "\n" << wave[4].emX << "\n" << wave[4].emZ << "\n" << wave[4].amp << "\n" << wave[4].prd << "\n" << wave[4].phS << "\n" << wave[4].phI << "\n" << wave[4].Yoff;
	oFile.close();
	oFile.open("config/wave5.txt");
	oFile <<  wave[5].id << "\n" << wave[5].emX << "\n" << wave[5].emZ << "\n" << wave[5].amp << "\n" << wave[5].prd << "\n" << wave[5].phS << "\n" << wave[5].phI << "\n" << wave[5].Yoff;
	oFile.close();
	oFile.open("config/wave6.txt");
	oFile <<  wave[6].id << "\n" << wave[6].emX << "\n" << wave[6].emZ << "\n" << wave[6].amp << "\n" << wave[6].prd << "\n" << wave[6].phS << "\n" << wave[6].phI << "\n" << wave[6].Yoff;
	oFile.close();
	oFile.open("config/wave7.txt");
	oFile <<  wave[7].id << "\n" << wave[7].emX << "\n" << wave[7].emZ << "\n" << wave[7].amp << "\n" << wave[7].prd << "\n" << wave[7].phS << "\n" << wave[7].phI << "\n" << wave[7].Yoff;
	oFile.close();
	oFile.open("config/wave8.txt");
	oFile <<  wave[8].id << "\n" << wave[8].emX << "\n" << wave[8].emZ << "\n" << wave[8].amp << "\n" << wave[8].prd << "\n" << wave[8].phS << "\n" << wave[8].phI << "\n" << wave[8].Yoff;
	oFile.close();


	return true;
}


////////////////////////////////////////////////////////////////////////////////


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
	else if(GetAsyncKeyState('9') & 0x8000f)
		currentKeyClicked = 9;

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
