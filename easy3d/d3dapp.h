//-----------------------------------------------------------------------------
// File: D3DApp.h
//
// Desc: Application class for the Direct3D samples framework library.
//
// Copyright (c) 1998-2000 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef D3DAPP_H
#define D3DAPP_H

// basetsd.h defines INT_PTR (used below).  It is not included by default
// under VC 5.0.  If you are using VC6 or later, it is included via Windows.h.
#include <basetsd.h> 

#include <D3DX9.h>

//-----------------------------------------------------------------------------
// Error codes
//-----------------------------------------------------------------------------
/***
//coef.hに移動
enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESIZEFAILED        0x8200000c
***/



//-----------------------------------------------------------------------------
// Name: struct D3DModeInfo
// Desc: Structure for holding information about a display mode
//-----------------------------------------------------------------------------
struct D3DModeInfo
{
    DWORD      Width;      // Screen width in this mode
    DWORD      Height;     // Screen height in this mode
    D3DFORMAT  Format;     // Pixel format in this mode
    DWORD      dwBehavior; // Hardware / Software / Mixed vertex processing
    D3DFORMAT  DepthStencilFormat; // Which depth/stencil format to use with this mode
};




//-----------------------------------------------------------------------------
// Name: struct D3DDeviceInfo
// Desc: Structure for holding information about a Direct3D device, including
//       a list of modes compatible with this device
//-----------------------------------------------------------------------------
struct D3DDeviceInfo
{
    // Device data
    D3DDEVTYPE   DeviceType;      // Reference, HAL, etc.
    D3DCAPS9     d3dCaps;         // Capabilities of this device
    const TCHAR* strDesc;         // Name of this device
    BOOL         bCanDoWindowed;  // Whether this device can work in windowed mode

    // Modes for this device
    DWORD        dwNumModes;
    D3DModeInfo  modes[150];

    // Current state
    DWORD        dwCurrentMode;
    BOOL         bWindowed;
    D3DMULTISAMPLE_TYPE MultiSampleType;
};




//-----------------------------------------------------------------------------
// Name: struct D3DAdapterInfo
// Desc: Structure for holding information about an adapter, including a list
//       of devices available on this adapter
//-----------------------------------------------------------------------------
struct D3DAdapterInfo
{
    // Adapter data
    D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier;
    D3DDISPLAYMODE d3ddmDesktop;      // Desktop display mode for this adapter

    // Devices for this adapter
    DWORD          dwNumDevices;
    D3DDeviceInfo  devices[5];

    // Current state
    DWORD          dwCurrentDevice;
};




//-----------------------------------------------------------------------------
// Name: class CD3DApplication
// Desc: A base class for creating sample D3D8 applications. To create a simple
//       Direct3D application, simply derive this class into a class (such as
//       class CMyD3DApplication) and override the following functions, as 
//       needed:
//          OneTimeSceneInit()    - To initialize app data (alloc mem, etc.)
//          InitDeviceObjects()   - To initialize the 3D scene objects
//          FrameMove()           - To animate the scene
//          Render()              - To render the scene
//          DeleteDeviceObjects() - To cleanup the 3D scene objects
//          FinalCleanup()        - To cleanup app data (for exitting the app)
//          MsgProc()             - To handle Windows messages
//-----------------------------------------------------------------------------
class CD3DFont;

class CD3DApplication
{
public:
	BOOL              m_bReady;
    D3DCAPS9          m_d3dCaps;           // Caps for the device

	CD3DFont*          m_pFont;
    CD3DFont*          m_pFontBig;

    D3DMULTISAMPLE_TYPE m_MultiSampleType;
    D3DPRESENT_PARAMETERS m_d3dpp;         // Parameters for CreateDevice/Reset
    LPDIRECT3DDEVICE9 m_pd3dDevice;        // The D3D rendering device

public:
    VOID    Cleanup3DEnvironment();
	int GetD3DDevice( LPDIRECT3DDEVICE9* ppdev );
	
	int TestCooperative();
	int TestCooperativeAndReset();

	int CreateLight( int* dstlid );

	int CheckFullScreenParams( int flag, SIZE chksize, int bits, int* flagptr, SIZE* validsize, int* bitsptr );// 新（C++用）
	int CheckFullScreenParams( int flag, DWORD w, DWORD h, int bits, int* flagptr, int* wptr, int* hptr, int* bitsptr );//HSP用

	int GetMaxMultiSampleNum( int bits, int iswindowmode, int* numptr );
	int CheckRTFormat( int srcfmt, int* okflag );
	int CheckHDR( int* okflag, int* hdrfmt, int* lumifmt );
	int CheckSpec( HINSTANCE hInstance, HWND srchwnd, int* okflag );

	HRESULT Resize3DEnvironment();


protected:
    // Internal variables for the state of the app
    D3DAdapterInfo    m_Adapters[10];
    DWORD             m_dwNumAdapters;
    DWORD             m_dwAdapter;
    BOOL              m_bWindowed;
    BOOL              m_bActive;
	int				  m_screensave;

    // Internal variables used for timing
    BOOL              m_bFrameMoving;
    BOOL              m_bSingleStep;

    // Internal error handling function
    HRESULT DisplayErrorMsg( HRESULT hr, DWORD dwType, char* mesptr = 0 );

    // Internal functions to manage and render the 3D scene
    HRESULT BuildDeviceList();
    BOOL    FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
                D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );
    HRESULT Initialize3DEnvironment( int gpuflag );
    HRESULT ToggleFullscreen();
    HRESULT ForceWindowed();
    HRESULT UserSelectNewDevice();
    //VOID    Cleanup3DEnvironment();
    //HRESULT Render3DEnvironment();
    virtual HRESULT AdjustWindowForChange();
    static INT_PTR CALLBACK SelectDeviceProc( HWND hDlg, UINT msg, 
                WPARAM wParam, LPARAM lParam );

	int SetFullscreenInfo( int flag, DWORD width, DWORD height, int bits );
	int CheckMultiSample( int winmode, D3DFORMAT fmt, int* numptr );


protected:
    // Main objects used for creating and rendering the 3D scene
    HWND              m_hWnd;              // The main app window
    HWND              m_hWndFocus;         // The D3D focus window (usually same as m_hWnd)
    LPDIRECT3D9       m_pD3D;              // The main D3D object
//    LPDIRECT3DDEVICE9 m_pd3dDevice;      publicへ  // The D3D rendering device
    D3DSURFACE_DESC   m_d3dsdBackBuffer;   // Surface desc of the backbuffer
    DWORD             m_dwCreateFlags;     // Indicate sw or hw vertex processing
    DWORD             m_dwWindowStyle;     // Saved window style for mode switches
    RECT              m_rcWindowBounds;    // Saved window bounds for mode switches
    RECT              m_rcWindowClient;    // Saved client area size for mode switches


	HBRUSH			m_bluebrush;

    // Variables for timing
    float             m_fTime;             // Current time in seconds
    float             m_fElapsedTime;      // Time elapsed since last frame
    float             m_fFPS;              // Instanteous frame rate
    TCHAR             m_strDeviceStats[90];// String to hold D3D device stats
    TCHAR             m_strFrameStats[40]; // String to hold frame stats

    // Overridable variables for the app
    TCHAR*            m_strWindowTitle;    // Title for the app's window
    BOOL              m_bUseDepthBuffer;   // Whether to autocreate depthbuffer
    DWORD             m_dwMinDepthBits;    // Minimum number of bits needed in depth buffer
    DWORD             m_dwMinStencilBits;  // Minimum number of bits needed in stencil buffer
    DWORD             m_dwCreationWidth;   // Width used to create window
    DWORD             m_dwCreationHeight;  // Height used to create window
    BOOL              m_bShowCursorWhenFullscreen; // Whether to show cursor when fullscreen

    // Overridable functions for the 3D scene created by the app
    virtual HRESULT ConfirmDevice(D3DCAPS9*,DWORD,D3DFORMAT)   { return S_OK; }
    virtual HRESULT OneTimeSceneInit()                         { return S_OK; }
    virtual HRESULT InitDeviceObjects();//                        { return S_OK; }
    virtual HRESULT FrameMove( int srcseri = 0, int tra_boneonly = 0 )                                { return S_OK; }
    virtual HRESULT Render()                                   { return S_OK; }
    virtual HRESULT DeleteDeviceObjects();//                      { return S_OK; }
    virtual HRESULT FinalCleanup();//                             { return S_OK; }

public:
    // Functions to create, run, pause, and clean up the application
    virtual HRESULT Create( HINSTANCE hInstance, HWND srchwnd, int fullscreenflag, int bits, int multisample, int gpuflag );
    virtual INT     Run();
    virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual VOID    Pause( BOOL bPause );
	
	HRESULT Render3DEnvironment( int skipflag = 0 );
    
	// Internal constructor
    CD3DApplication();


    virtual HRESULT InvalidateDeviceObjects( int resetflag );//                  { return S_OK; }
    virtual HRESULT RestoreDeviceObjects();//                     { return S_OK; }

public:
	int m_writebmpflag;

	int m_force_render;
	DWORD m_rendertime;

	int						m_lbutton;
	int						m_movebone;

	int						m_rbutton;
	int						m_shiftbone;

	IDirect3DSurface9*		m_pZSurf;
	IDirect3DSurface9*		m_pBackBuffer;


};




#endif



