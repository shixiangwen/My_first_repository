/*****d3dFramework.h*****/
#include "d3dx9.h"

bool d3d9Initial(HINSTANCE hInstance,
				 WNDPROC WndProc,
				 int width,
				 int height,
				 bool fullScreen,
				 D3DDEVTYPE deviceType,
				 IDirect3DDevice9 ** device);

HWND d3d9CreateWindow(HINSTANCE hInstance,
					  WNDPROC WndProc,
					  int width,
					  int height,
					  bool fullScreen);

int d3d9CheckHardwareVP(IDirect3D9* d3d9,D3DDEVTYPE deviceType);

D3DPRESENT_PARAMETERS d3d9CreatePP(HWND hwnd,
								   int width,
								   int height,
								   bool fullScreen);

bool d3d9CreateDevice(IDirect3D9* d3d9,
					  D3DDEVTYPE deviceType,
					  HWND hwnd,
					  int vp,
					  D3DPRESENT_PARAMETERS d3dpp,
					  IDirect3DDevice9** device);

int d3d9MessageLoop(bool (* p_Display)(float time,IDirect3DDevice9* Device),
					IDirect3DDevice9* Device);

template<class T> void Release(T t)
{
	if( t )
	{
		t->Release();
		t = 0;
	}
}
		
template<class T> void Delete(T t)
{
	if( t )
	{
		delete t;
		t = 0;
	}
}
